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
        config.MaxConnections = config_buffer["server"]["max_connections"].value_or(config.MaxConnections);

        config.db_host = config_buffer["database"]["host"].value_or(config.db_host);
        config.db_UserName = config_buffer["database"]["user"].value_or(config.db_UserName);
        config.db_Passwd = config_buffer["database"]["password"].value_or(config.db_Passwd);
        config.db_name = config_buffer["database"]["db_name"].value_or(config.db_name);
        config.db_TableName = config_buffer["database"]["table_name"].value_or(config.db_TableName);

        // Campos de la tabla
        if(auto campos_buffer = config_buffer["database"]["table_fields"].as_array()){

            for(auto&& campo : *campos_buffer){

                std::string nombre_campo = campo.value_or("");

                if(!nombre_campo.empty()){
                    config.TableFields.push_back(nombre_campo);
                }
            }
        }

        return config;
    }

}
