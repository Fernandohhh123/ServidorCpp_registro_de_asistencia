#include <stdio.h>
#include <fstream>
#include <iostream>

#include "../include/config.hpp"
#include "../include/toml.hpp"


namespace config{
    Config_server get_config(void){

        Config_server config;

        toml::table config_buffer;

        try{
            config_buffer = toml::parse_file(CONFIG_PATH);
        }
        catch (const toml::parse_error& e){
            std::cerr << "[ x ] Error en el archivo de configuracion: "
                      << e.description() << std::endl
                      << "Linea: " << e.source().begin.line << std::endl
                      << "Columna: " << e.source().begin.column << std::endl;

            return config;
        }

        config.port = config_buffer["server"]["port"].value_or(config.port);

        return config;
    }

}
