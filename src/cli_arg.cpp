#include <string.h>
#include <iostream>
#include <cstdlib>

#include "../include/cli_arg.hpp"
#include "../include/version.hpp"


#define NORMAL_EXIT 0

Cli_args argument_parser(int argc, char **argv){
	Cli_args cliargs;

	for(int i = 1; i < argc; i++){
		std::string argument = argv[i];

		if(argument == "-h"){

            std::cout << "Este servidor se gestiona mediante el archivo de configuracion." << std::endl;
            std::cout << "El archivo de configuracion se encuentra en /etc/cerrusv.json" << std::endl;

            exit(0);

		}else if(argument == "-v"){

			std::cout << "SERVER CERRU version " << get_app_version() << std::endl;
			exit(NORMAL_EXIT);

		}else if(argument == "--version"){

			std::cout << "SERVER CERRU" << std::endl;
			std::cout << "App version " << get_app_version() << std::endl;
			std::cout << "Desarrollado por Fernando Hernandez V." << std::endl;

			exit(NORMAL_EXIT);
		}

		else{
			std::cout << "Unknow option: " << argument << std::endl;
			std::cout << "servidor -h\nservidor --help" << std::endl;
			std::cout << "Para obtener ayuda." << std::endl;

			exit(1);
		}
	}

	return cliargs;
}
