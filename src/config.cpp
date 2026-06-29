#include <stdio.h>
#include <fstream>
#include <iostream>

#include "../include/config.hpp"
#include "../include/toml.hpp"


namespace config{
    Config_server get_config(void){
        std::ifstream archivo("/etc/cerrucfg.toml");

        Config_server config;

        if(!archivo.is_open()){
            std::cerr << "[ x ] Archivo de configuracion no encontrado." << std::endl;
        }

        return config;
    }

}
