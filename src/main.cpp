#include <iostream>
#include "server.hpp"

int main(){

    // escrito por fernando herandez valverde
    std::cout << "SERVIDOR CERRU" << std::endl;
    
    std::string app_ver = "0.2";
    std::cout << "app Version: " << app_ver << std::endl;

    std::cout << "Ingresa el puerto: ";
    int puerto = 0;
    std::cin >> puerto;

    Server servidor(puerto);
    servidor.run();

    return 0;
}
