#pragma once

#include <netinet/in.h>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

#include "buffer_client.hpp"
#include "config.hpp"

// Clase principal del servidor
class Server {

private:
    int sockfd;  // Descriptor del socket principal
    config::Config_server config;
    std::mutex bd_mutex; // proteccion para inserciones simultaneas en la db
    std::atomic<int> conexiones_actuales {0}; // Contator para proteger el maximo
                                              // de conexiones

public:
    explicit Server(const config::Config_server& cfg);  // Constructor con configuracion
    void run();              // Inicia el servidor (acepta clientes)

    ~Server() = default;

private:
    // Manejo por cliente (un hilo por conexión)
    void handle_client_thread(int client_fd);

    // Devuelve hora actual formateada HH:MM:SS
    std::string obtener_hora_actual() const;

    // funcion para guardar los datos en la base
    void guardar_en_bd(const ClientData&);
};
