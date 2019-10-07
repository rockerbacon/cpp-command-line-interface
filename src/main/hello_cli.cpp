#include <iostream>
#include "command_line_interface.h"

using namespace std;

int main (int argc, char **argv) {

	cli::RequiredArgument<string> first_name("first-name");
	cli::OptionalArgument<string> second_name("second-name", "", 's');
	cli::OptionalArgument<unsigned> age("age", 0);
	cli::FlagArgument knows_assertion("knows-assertion");

	cli::capture_all_arguments_from(argc, argv);

	cout << "Hello " << *first_name << ' ' << *second_name << endl;
	cout << "You told me you have " << *age << " years" << endl;
	if (*knows_assertion) {
		cout << "Oh! and you seem to be experienced with Assertions as well!" << endl;
	} else {
		cout << "Don't worry, Assertions is very easy and you'll get it with time" << endl;
	}

	return 0;

}
