#include "base_arguments.h"

using namespace std;
using namespace cli;


Argument::Argument(const std::string &argument_label, char argument_abbreviation) 
	:	present(false)
{
	std::string prefixed_label = "--" + argument_label;

	if (cli::argument_map.find(prefixed_label) != cli::argument_map.end()) {
		throw std::invalid_argument("Cannot define two arguments with the same label");
	}
	cli::argument_map[prefixed_label] = this;

	if (argument_abbreviation != '\0') {
		std::string prefixed_abbreviation = "-" + std::string(1, argument_abbreviation);

		if (cli::argument_map.find(prefixed_abbreviation) != cli::argument_map.end()) {
			throw std::invalid_argument("Cannot define two arguments with the same abbreviation");
		}
		cli::argument_map[prefixed_abbreviation] = this;
	}
}

bool Argument::is_present(void) const {
	return this->present;
}

void Argument::set_as_present(void) {
	this->present = true;
}

UnvaluedArgument::UnvaluedArgument(const std::string &argument_label, char argument_abbreviation)
	:	Argument(argument_label, argument_abbreviation)
{}

void UnvaluedArgument::read_value_from(UNUSED const char *arg) {
	// do nothing
}

bool UnvaluedArgument::requires_value(void) const {
	return false;
}
