#pragma once

#include <cstdint>
#include <string>

struct Cli_args{
	uint64_t puerto = 0;
    uint64_t max_connections = 0;

    uint8_t verbose = 0;

	bool showVersion = false;
	bool showHelp = false;
	std::string app_version = " ";
};

Cli_args argument_parser(int, char**);
