#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "server.hpp"

int main(){
    std::cout<<"Iniciando dervidor\n";
	uint16_t puerto;

    std::cout << "Ingresa el puerto del servidor: ";
    std::cin >> puerto;

    int64_t server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(puerto);
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    std::cout << "Servidor iniciado en puerto " << puerto << std::endl;
    int client_fd = accept(server_fd, nullptr, nullptr);

    std::cout << "Cliente encontrado." << std::endl;


	//end server
    close(client_fd);
    close(server_fd);

    return 0;
}
