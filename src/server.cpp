#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <thread>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <iomanip>
#include <netinet/tcp.h>  // Para configurar keepalive

#include "../include/server.hpp"

Server::Server(const config::Config_server& cfg) : sockfd(-1), config(cfg) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error en setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Activar TCP Keepalive
    int keepalive = 1;
    int idle = 10;      // segundos de inactividad antes de enviar probe
    int interval = 5;   // segundos entre probes
    int count = 3;      // número de probes fallidos antes de desconectar

    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config.port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Error en bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 100) < 0) {
        perror("Error en listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // std::cout << "Servidor iniciado en el puerto " << config.port << std::endl;
    std::cout << "[ * ] CTL + C para cerrar el servidor." << std::endl;
}


void Server::run() {

    while (true) {

        if(conexiones_actuales > config.MaxConnections){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(sockfd, (sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) {

            int error_guardado = errno;

            if(errno == EBADF || errno == EINVAL){
                std::cerr << "\n\n";
                std::cerr << "======================= FALLO CRITICO ====================" << std::endl;
                std::cerr << "[ x ] Socket principal cerrado" << std::endl;
                std::cerr << "[ " << obtener_hora_actual()
                          << " ] Fallo critico en el socket principal" << std::endl;
                std::cerr << "[ ! ] Codigo de error: " << error_guardado << std::endl;
                std::cerr << "Descripcion: " << strerror(error_guardado) << std::endl;
                std::cerr << "[ ! ] Deteniendo el servidor" << std::endl;
                std::cerr << "==========================================================" << std::endl;
                exit(1);
                break;
            }

            perror("[ x ] Error en accept");
            continue;
        }

        std::cout << "Cliente conectado ip: " << inet_ntoa(client_addr.sin_addr) << std::endl;

        ++ conexiones_actuales;

        // Crear un hilo por cliente
        std::thread(
            [this, client_fd]() {
                this -> handle_client_thread(client_fd);
                -- conexiones_actuales;
            }
        ).detach();
    }
}


void Server::handle_client_thread(int client_fd) {

    char buffer[2048] = {0};
    ClientData c_data;

    auto last_check_time = std::chrono::steady_clock::now();
    const int64_t CHECK_INTERVAL_SECONDS = 10;  // cada 10 segundos verificamos conexión

    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(client_fd, &readfds);

        struct timeval tv;
        tv.tv_sec = 1;   // revisamos cada 1 segundo
        tv.tv_usec = 0;

        int activity = select(client_fd + 1, &readfds, nullptr, nullptr, &tv);
        if (activity < 0) {
            perror("Error en select");
            break;
        }

        if (activity > 0) {
            // Cliente envió datos
            memset(buffer, 0, sizeof(buffer));
            int bytes_read = read(client_fd, buffer, sizeof(buffer));

            if (bytes_read <= 0) {
                std::cerr << "======================================================" << std::endl;
                std::cerr << "*** Un cliente se ha desconectado antes de enviar datos ***" << std::endl;
                std::cerr << "======================================================" << std::endl;
                break;
            }

            std::string received_data(buffer);

// Limpieza rápida de saltos de línea al final (por si usan netcat o telnet)
            if (!received_data.empty() && received_data.back() == '\n') received_data.pop_back();
            if (!received_data.empty() && received_data.back() == '\r') received_data.pop_back();

            // ==========================================
            // EL CAMBIO FUERTE: PARSEO DINÁMICO
            // ==========================================
            std::stringstream ss(received_data);
            std::string token;
            size_t field_index = 0;

            // Recorremos los datos usando ';' como delimitador
            // config.campos_server contiene los nombres leídos del TOML
            while (std::getline(ss, token, ';') && field_index < config.TableFields.size()) {
                std::string nombre_columna = config.TableFields[field_index];

                // Metemos al mapa asociativo dinámicamente
                c_data.datos_usuario[nombre_columna] = token;
                field_index++;
            }

            // Guardamos en la base de datos si recibimos los campos configurados
            if (field_index == config.TableFields.size()) {
                guardar_en_bd(c_data);

                std::string msg_usr = "- DATOS ENVIADOS -\n";
                ssize_t sent = write(client_fd, msg_usr.c_str(), msg_usr.size());
                if (sent <= 0) {
                    perror("Error al enviar datos al cliente");
                    break;
                }

                // Imprimimos la auditoría de consola de forma dinámica (Solo campos del TOML)
                for (const auto& [columna, valor] : c_data.datos_usuario) {
                    std::cout << columna << ": " << valor << std::endl;
                }
                std::cout << "-------------------------------------" << std::endl << std::endl;

                break; // Si ya procesamos exitosamente, rompemos el while para cerrar la conexión
            } else {
                std::string msg_error = "- ERROR: CAMPOS INCOMPLETOS -\n";
                write(client_fd, msg_error.c_str(), msg_error.size());
                break;
            }
        }

        // 🔹 Verificar si la conexión sigue viva cada CHECK_INTERVAL_SECONDS (Manteniendo tu lógica original)
        auto now = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - last_check_time).count();
        if (diff >= CHECK_INTERVAL_SECONDS) {
            ssize_t sent = send(client_fd, "", 0, MSG_NOSIGNAL); // envío de prueba de vida
            if (sent < 0) {
                perror("Cliente desconectado o error en conexión");
                break;
            }
            last_check_time = now;
        }
    }

    close(client_fd);
}

/*
            std::string received_data(buffer);

            // Parseo básico de datos
            size_t pos = 0;
            int field_index = 0;
            std::string token;
            std::string delimiter = ";";

            while ((pos = received_data.find(delimiter)) != std::string::npos && field_index < 5) {
                token = received_data.substr(0, pos);
                switch (field_index) {
                    case 0: c_data.nombre = token; break;
                    case 1: c_data.matricula = token; break;
                    case 2: c_data.carrera = token; break;
                    case 3: c_data.docente = token; break;
                    case 4: c_data.motivo = token; break;
                }
                received_data.erase(0, pos + delimiter.length());
                field_index++;
            }

            std::cout << "-> " << c_data.nombre_pc << " envió datos" << std::endl;

            c_data.hora = obtener_hora_actual();
            guardar_en_bd(c_data);

            std::string msg_usr = "- DATOS ENVIADOS -\n";
            ssize_t sent = write(client_fd, msg_usr.c_str(), msg_usr.size());
            if (sent <= 0) {
                perror("Error al enviar datos al cliente");
                break;
            }

            std::cout << c_data.hora << std::endl;
            std::cout << "Nombre: " << c_data.nombre << std::endl;
            std::cout << "Motivo: " << c_data.motivo << std::endl;
            std::cout << "Nombre de la PC: " << c_data.nombre_pc << std::endl;
            std::cout << "-------------------------------------" << std::endl << std::endl;
        }

        // 🔹 Verificar si la conexión sigue viva cada CHECK_INTERVAL_SECONDS
        auto now = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - last_check_time).count();
        if (diff >= CHECK_INTERVAL_SECONDS) {
            ssize_t sent = send(client_fd, "", 0, MSG_NOSIGNAL); // envío de prueba
            if (sent < 0) {
                perror("Cliente desconectado o error en conexión");
                break;
            }
            last_check_time = now;
        }
    }

    close(client_fd);
}



*/

void Server::guardar_en_bd(const ClientData& c_data) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        
        // Ahora el puerto se concatena dinámicamente desde el TOML
        std::string connection_string = "tcp://" + config.db_host + ":" + config.db_port;
        
        std::unique_ptr<sql::Connection> conn(
            driver->connect(connection_string, config.db_UserName, config.db_Passwd));
            
        conn->setSchema(config.db_name);

        // ==========================================
        //  CONSTRUCCIÓN DINÁMICA PURA DEL QUERY SQL
        // ==========================================
        std::string columnas = "";
        std::string valores = ""; 
        bool primero = true;

        for (const auto& [columna, valor] : c_data.datos_usuario) {
            if (!primero) {
                columnas += ", ";
                valores += ", ";
            }
            columnas += columna;
            valores += "'" + valor + "'"; // Envolvemos el valor en comillas simples para SQL
            primero = false;
        }

        // Si por alguna razón el mapa está vacío, evitamos lanzar un query roto
        if (columnas.empty()) {
            std::cerr << "[ DB ERROR ] No hay campos configurados para insertar." << std::endl;
            return;
        }

        std::string query_final = "INSERT INTO " + config.db_TableName + " (" + columnas + ") VALUES (" + valores + ")";

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute(query_final);

        std::cout << "[ DB ] Datos guardados dinámicamente en la tabla " << config.db_TableName << "." << std::endl;

    } catch (sql::SQLException& e) {
        std::cerr << "[ DB ERROR ] Error al guardar en la base de datos: " << e.what() << std::endl;
    }
}

/*
void Server::guardar_en_bd(const ClientData& c_data) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> conn(
            driver->connect("tcp://127.0.0.1:3306", "root", "root"));
        conn->setSchema("registro_de_asistencia");

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO usuarios(nombre, matricula, carrera, docente, motivo, nombre_pc, hora) VALUES (?, ?, ?, ?, ?, ?, ?)"));

        pstmt->setString(1, c_data.nombre);
        pstmt->setString(2, c_data.matricula);
        pstmt->setString(3, c_data.carrera);
        pstmt->setString(4, c_data.docente);
        pstmt->setString(5, c_data.motivo);
        pstmt->setString(6, c_data.nombre_pc);
        pstmt->setString(7, c_data.hora);
        pstmt->execute();

        std::cout << "Datos guardados en la base de datos." << std::endl;
    } catch (sql::SQLException& e) {
        std::cerr << "Error al guardar en la base de datos: " << e.what() << std::endl;
    }
}

*/


std::string Server::obtener_hora_actual() const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}
