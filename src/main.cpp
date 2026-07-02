#include <iostream>
#include "../include/server.hpp"
#include "../include/cli_arg.hpp"
#include "../include/version.hpp"
#include "../include/config.hpp"

//numero que indica puerto no definido
#define UNDEF_PORT 0

//codigo que indica el estado en el que termino el programa (normal = 0)
#define NORMAL_EXIT 0

#define DEFAULT_PORT 12345

int main(int argc, char *argv[]){

	int16_t status_code = NORMAL_EXIT;

	// escrito por Fernando Herandez Valverde

	Cli_args arguments = argument_parser(argc, argv);

    std::cout << "+----------------+" << std::endl;
	std::cout << "| SERVIDOR CERRU |" << std::endl;
    std::cout << "+----------------+" << std::endl;

    // obtenemos la configuracion del archivo de configuracion
    config::Config_server config = config::get_config();

    if(!config::validate_configuration(&config)){
        std::cout << "[ x ] Configuracion invalida" << std::endl;
        std::cout << "Cargando la configuracion por defecto" << std::endl;
        load_default_config(&config);
    }

    if (arguments.verbose != 0){
        std::cout << "# Configuracion del servidor #" << std::endl;
        std::cout << "Puerto: " << config.port << std::endl;
        std::cout << "Maximas conecciones: " << config.MaxConnections << std::endl;
        std::cout << "IP Data Base: " << config.db_host << std::endl;
        std::cout << "Nombre de la base de datos: " << config.db_name << std::endl;
        std::cout << "Tabla: " << config.db_TableName << std::endl;

        std::cout << "Delimitador: " << config.delimiter << std::endl;
    }

    Server servidor(config);

	servidor.run();

	return status_code;
}
