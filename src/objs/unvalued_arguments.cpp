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

		cout << cli::this_program.title << ' ' << cli::this_program.version << endl;
		cout << cli::this_program.description << endl << endl;
		cout << "Usage: " << cli::this_program.title << " [OPTIONS...]" << endl;
		cout << "OPTIONS:" << endl;
		cout << cli::arguments_help_message.rdbuf();

		exit(0);

	}, "help", 'h', "print this help message")
{}

VersionArgument::VersionArgument(void)
	:	FunctionArgument([&]() {
		
		cout << cli::this_program.version << endl;

		exit(0);

	}, "version", 'v', "print program version")
{}
