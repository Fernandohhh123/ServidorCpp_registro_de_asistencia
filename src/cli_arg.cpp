#include <string.h>
#include <iostream>
#include <cstdlib>

#include "../include/cli_arg.hpp"
#include "../include/version.hpp"


#define NORMAL_EXIT 0

Cli_args argument_parser(int argc, char **argv){
	Cli_args cliargs;

    if (argc < 2) {
        return cliargs;
    }

	for(int i = 1; i < argc; i++){

		std::string argument = argv[i];

		if(argument == "-h"){

            std::cout << "Este servidor se gestiona mediante el archivo de configuracion." << std::endl;
            std::cout << "El archivo de configuracion se encuentra en /etc/cerrusv.json" << std::endl;

            std::cout << "Argumentos disponibles:" << std::endl;
            std::cout << "-h | Muestra este menu de ayuda" << std::endl;
            std::cout << "-v | Muestra la version del programa" << std::endl;
            std::cout << "--version | Muestra informacion mas detallada" << std::endl;
            std::cout << "--gen-config-file | Genera el archivo de configuracion .toml" << std::endl;

            exit(0);

		}else if(argument == "-v"){

			std::cout << "SERVER CERRU version " << get_app_version() << std::endl;
			exit(NORMAL_EXIT);

		}else if(argument == "--version"){

			std::cout << "SERVER CERRU" << std::endl;
			std::cout << "App version " << get_app_version() << std::endl;
			std::cout << "Desarrollado por Fernando Hernandez V." << std::endl;

			exit(NORMAL_EXIT);
		} else if(argument == "--gen-config-file"){
            std::cout << "Deberia generarse el archivo de configuracion." << std::endl;
            exit(NORMAL_EXIT);
        }

		else{
			std::cout << "Unknow option: " << argument << std::endl;
			std::cout << "cerrusv -h" << std::endl;
			std::cout << "Para obtener ayuda." << std::endl;

			exit(1);
		}
	}

	return cliargs;
}

