# CPP COMMAND LINE INTERFACE
CPP Command Line Interface (abbreviated CPP CLI) is a simple library for easy and clean creation of Command Line Interfaces for C++ programs

#### Table of Contents
* [Syntax Overview](#syntax-overview)
* [Arguments](#arguments)
	* [Interface](#interface)
	* [Valued Arguments](#valued-arguments)
		* [RequiredArgument](#requiredargument)
		* [OptionalArgument](#optionalargument)
	* [Unvalued Arguments](#unvalued-arguments)
		* [FlagArgument](#flagargument)
		* [FunctionArgument](#functionargument)
* [Building Manually](#building-manually)

## Syntax Overview
CLI utilizes a macro which automatically generates your program's entry point:
``` c++
#include <cppcli/command_line_interface.h>
#include <iostream>

cli_main(
	// program information
	"string with program name",
	"string with program version",
	"string with progarm description",
	
	// argument declarations
	cli::RequiredArgument<int> semicolon_terminated_argument1("arg1");
	cli::RequiredArgument<std::string> semicolon_terminated_argument2("arg2");
	cli::create_alias("arg1", '1');
) {
	std::cout << *semicolon_terminated_argument1 << *semicolon_terminated_argument2 << std::endl;
} end_cli_main
```
This construction will generate your program's _main_ entry point and all the code needed to capture the declared arguments, as well as _--help_ and _--version_ arguments.

The first three parameters are always required and are used to generate _--help_ and _--version_ arguments, the code following them should contain all argument declarations.

Note that the declaration block is the same as any other C++ code block, not being limited to declarations and requiring every command to be terminated with a semicolon.

## Arguments
Arguments are declared by using special classes, which have a label and an (optional) description:
``` c++
RequiredArgument<int> x_number ("x-number", "x number description");
RequiredArgument<float> y_number ("y-number");
```

Additionaly, aliases can be created so that the same argument can be passed with different labels:
``` c++
create_alias("y-number", 'y');
```
An alias can either be a _char_ or a _std::string_, if it's a char the argument is prefixed with a single dash instead of a double dash.

A program built with the aforementioned code be called like so:
```
program --x-number 3 --y-number 4.25
program -y 4.25 --x-number 3
```

A _--help_ argument is automatically generated based on the provided argument descriptions:
```
program --help
```
If an argument has no description, a generic one will be provided.

### Interface
All arguments are instances of _ArgumentInterface_, meaning all of them share a few useful methods:

#### is_present
``` c++
bool is_present(void) const;
```
Returns __true__ if the argument was passed and __false__ otherwise.


### Valued Arguments
Valued arguments are arguments which require a value to be passed along with the label. This value will be stored inside the argument instance and can be accessed with the dereference operator (prefix *).

#### RequiredArgument
A valued argument that MUST BE passed to the program. If the argument is not passed an exception will be raised during argument parsing.
``` c++
cli::RequiredArgument<int> required_integer("integer", "description");
```

#### OptionalArgument
A valued argument which doesn't need to be explicitly passed. A default value is used when the argument is not passed:
``` c++
double low_precision_pi = 3.14;
cli::OptionalArgument<double> pi(
	low_precision_pi,
	"pi",
	"value for pi, if not specified 3.14 will be used"
);
```

### Unvalued Arguments
Unvalued arguments are arguments which only require the label to be passed. Usage of these arguments is not standardized and varies for each implementation.

#### FlagArgument
This argument takes no value, it's merely a flag which tells whether the argument was present during the program call or not:
``` c++
cli::FlagArgument should_print_hello(
	"print_hello",
	"if present, the program will print 'hello' to stdout"
);
/* ... */
if (*should_print_hello)
	std::cout << "hello" << std::endl;
```
Dereferencing the argument returns __true__ if the label was passed and __false__ otherwise.

For the example above the call would be like so:
```
program --print_hello
```
Or:
```
program
```

#### FunctionArgument
A function argument is an argument which holds a function to be immediately executed after the argument's label is parsed:
``` c++
cli::FunctionArgument alternative_help_message([]() {
	std::cout << "This is an alternative help message" << std::endl;
	exit(0);
}, "alternative-help");
```
Since the function is executed during argument parsing, developers must be careful when using lambda captures.

Both _--help_ and _--version_ arguments are instances of _FunctionArgument_.

## Building Manually
CPP CLI is made available for x86-64 glibc Linux through static library releases. For different platforms a manual build will be necessary. A build can be easily generated with the command:
```
./build.sh
```
For prerequisites and further informations see the documentation for the [Assertions framework](https://github.com/rockerbacon/assertions)
