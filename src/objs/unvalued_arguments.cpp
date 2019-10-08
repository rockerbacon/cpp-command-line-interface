#include "command_line_interface.h"

#include <iostream>

using namespace std;
using namespace cli;

FlagArgument::FlagArgument(const std::string &argument_label, char argument_abbreviation)
	:	UnvaluedArgument(argument_label, argument_abbreviation)
{}

bool FlagArgument::is_required(void) const {
	return false;
}

bool FlagArgument::operator*(void) const {
	return this->is_present();
}

FunctionArgument::FunctionArgument(const decltype(function) &function, const string &argument_label, char argument_abbreviation)
	:	UnvaluedArgument(argument_label, argument_abbreviation),
		function(function)
{}

bool FunctionArgument::is_required(void) const {
	return false;
}

void FunctionArgument::set_as_present(void) {
	this->function();
}

HelpArgument::HelpArgument(void)
	:	FunctionArgument([&]() {

		cout << "Usage: " << "pname" << " [OPTIONS...]" << endl;
		cout << "OPTIONS:" << endl;
		for (auto arg : cli::argument_map) {
			auto label = arg.first;
			cout << '\t' << label << ": " << "description" << endl;
		}

		exit(0);

	}, "help", 'h')
{}
