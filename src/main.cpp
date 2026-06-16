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

	Cli_args config = argument_parser(argc, argv);

	std::cout << "SERVIDOR CERRU" << std::endl;

	config.app_version = get_app_version();

	//obtenemos la configuracion del archivo.cfg
	get_config(&config, "/etc/cerrusv.cfg");

	if(config.puerto <= UNDEF_PORT){
        	std::cout << "PUERTO NO ESPECIFICADO." << std::endl;
	        config.puerto = DEFAULT_PORT;
	}


	Server servidor(config.puerto);
	servidor.run();

	return status_code;
}
