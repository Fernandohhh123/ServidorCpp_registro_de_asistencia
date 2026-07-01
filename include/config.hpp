#pragma once

#include "cli_arg.hpp"

#include <vector>
#include <string>

#define CONFIG_PATH "/etc/cerrucfg.toml"

namespace config {

    struct Config_server {
        int64_t port = 0;
        int32_t MaxConnections = 0;

        std::string db_host = "";
        std::string db_UserName = "";
        std::string db_Passwd = "";
        std::string db_name = "";
        std::string db_TableName = "";
        std::vector<std::string> TableFields = {};

        std::string delimiter = ";";
    };

    Config_server get_config(void);
    bool validate_configuration(Config_server*);
    void load_default_config(Config_server*);
}
