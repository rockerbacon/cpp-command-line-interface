#include "command_line_interface.h"

#include <iostream>

using namespace cli;

FlagArgument::FlagArgument(const std::string &argument_label, char argument_abbreviation, const string &description)
	:	UnvaluedArgument(argument_label, description, argument_abbreviation)
{}

bool FlagArgument::is_required(void) const {
	return false;
}

bool FlagArgument::operator*(void) const {
	return this->is_present();
}

FunctionArgument::FunctionArgument(const decltype(execute) &function_to_execute, const string &argument_label, char argument_abbreviation, const string &description)
	:	UnvaluedArgument(argument_label, description, argument_abbreviation),
		execute(function_to_execute)
{}

bool FunctionArgument::is_required(void) const {
	return false;
}

void FunctionArgument::set_as_present(void) {
	this->execute();
}

HelpArgument::HelpArgument(void)
	:	FunctionArgument([&]() {

		cout << "Usage: " << "pname" << " [OPTIONS...]" << endl;
		cout << "OPTIONS:" << endl;
		cout << cli::arguments_help_message.rdbuf();

		exit(0);

	}, "help", 'h', "print this help message")
{}
