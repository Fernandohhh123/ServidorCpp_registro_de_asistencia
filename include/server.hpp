#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <string>
#include <thread>
#include "buffer_client.hpp"

// Clase principal del servidor
class Server {
    int sockfd;  // Descriptor del socket principal
    int port;    // Puerto del servidor

public:
    explicit Server(int p);  // Constructor con puerto
    void run();              // Inicia el servidor (acepta clientes)

private:
    // Manejo por cliente (un hilo por conexión)
    void handle_client_thread(int client_fd);

    // Devuelve hora actual formateada HH:MM:SS
    std::string obtener_hora_actual() const;
};

// Función global para guardar datos del cliente en la base de datos
void guardar_en_bd(const ClientData&);

#endif
