#include "command_line_interface.h"
#include <iostream>

cli_describe(
	"hello_cli",
	"v0.1",
	"Simple example program for CPP CLI"
)

cli_entry_point {
	
	using namespace std;
	using namespace cli;

	cli_arguments (
		RequiredArgument<string> first_name("first-name", '\0', "your first name");
		OptionalArgument<string> second_name("", "second-name", 's');
		OptionalArgument<unsigned> age(0, "age");
		FlagArgument knows_assertion("knows-assertion");
	);

	cout << "Hello " << *first_name << ' ' << *second_name << endl;
	cout << "You told me you have " << *age << " years" << endl;
	if (*knows_assertion) {
		cout << "Oh! and you seem to be experienced with Assertions as well!" << endl;
	} else {
		cout << "Don't worry, Assertions is very easy and you'll get it with time" << endl;
	}

	return 0;

}
