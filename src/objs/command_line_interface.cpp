#include "command_line_interface.h"

#include <iostream>

using namespace std;
using namespace cli;

unordered_map<string, ArgumentInterface*> cli::argument_map;

void cli::capture_all_arguments_from(int argc, char **argv) {
	int i = 1;
	decltype(cli::argument_map)::iterator argument_index;
	ArgumentInterface *argument;
	string arg_label;

	while (i < argc) {
		arg_label = argv[i];

		argument_index = cli::argument_map.find(arg_label);
		if (argument_index == cli::argument_map.end()) {
			throw std::invalid_argument("Invalid argument '"+arg_label+"'");
		}

		argument = argument_index->second;
		if (argument->requires_value()) {
			i++;
			if (i == argc) {
				throw std::domain_error("Argument '"+arg_label+"' requires a valid value");
			}

			try {
				argument->read_value_from(argv[i]);
			} catch (const std::invalid_argument &e) {
				throw std::invalid_argument("Could not parse value for argument '"+arg_label+"': "+e.what());
			}
		} else {
			argument->set_as_present();
		}

		i++;
	}

	for (auto possible_argument : cli::argument_map) {
		arg_label = possible_argument.first;
		argument = possible_argument.second;
		if (argument->is_required() && !argument->is_present()) {
			throw std::domain_error("Missing required argument '"+arg_label+"'");
		}
	}

}
