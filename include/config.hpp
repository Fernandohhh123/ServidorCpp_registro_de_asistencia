#pragma once

#include "cli_arg.hpp"

#include <vector>
#include <string>

#define CONFIG_PATH "/etc/cerrucfg.toml"

namespace config {

    struct Config_server {
        int64_t port = 0;
        int32_t MaxConnections = 0;
        std::string db_UserName = " ";
        std::string db_Passwd = " ";
        std::string db_TableName = " ";
        std::vector<std::string> TableFields = {};
        std::string delimiter = " ";
    };

    struct Config_server_default {
        int64_t port = 12345;
        int32_t MaxConnections = 100;
        std::string db_UserName = "root";
        std::string db_Passwd = "12345";
        std::string db_TableName = "usuarios";
        std::vector<std::string> TableFields = {"nombre", "matricula"};
        std::string delimiter = ";";
    };

    Config_server get_config(void);
}
