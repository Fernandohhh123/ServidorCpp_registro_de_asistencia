#ifndef CLI_ARG_HPP
#define CLI_ARG_HPP

#include <cstdint>
#include <string>

struct Cli_args{
	uint64_t puerto = 0;
	bool showVersion = false;
	bool showHelp = false;
	std::string app_version = " ";
};

Cli_args argument_parser(int, char**);

#endif //CLI_ARG_HPP
