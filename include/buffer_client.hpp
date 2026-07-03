#pragma once
#include <string>
#include <unordered_map>

struct ClientData {
    std::unordered_map<std::string, std::string> datos_usuario;
    std::string hora;
};
