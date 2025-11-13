#include <iostream>
#include "server.hpp"

int main(){

    // escrito por fernando herandez valverde

    std::cout << "Ingresa el puerto: ";
    int puerto = 0;
    std::cin >> puerto;

    Server servidor(puerto);
    servidor.run();

    return 0;
}
