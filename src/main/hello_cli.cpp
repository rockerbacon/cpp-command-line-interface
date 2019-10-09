#include "command_line_interface.h"
#include <iostream>

#define PROGRAM_NAME "hello_cli"
#define PROGRAM_VERSION "v0.1"
#define PROGRAM_DESCRIPTION "Simple example program for CPP CLI"

cli_main (
	PROGRAM_NAME,
	PROGRAM_VERSION,
	PROGRAM_DESCRIPTION,

	cli::RequiredArgument<std::string> first_name("first-name", "your first name");
	cli::OptionalArgument<std::string> second_name("", "second-name");
	cli::create_alias("second-name", 's');
	cli::OptionalArgument<unsigned> age(0, "age");
	cli::FlagArgument knows_clicpp("knows-clicpp", "if present, tells that you know CLI CPP");

) {
	
	std::cout << "Hello " << *first_name << ' ' << *second_name << std::endl;
	std::cout << "You told me you have " << *age << " years" << std::endl;
	if (*knows_clicpp) {
		std::cout << "Oh! and you seem to be experienced with CLI CPP as well!" << std::endl;
	} else {
		std::cout << "Don't worry, CLI CPP is very easy and you'll get it with time" << std::endl;
	}

} end_cli_main
