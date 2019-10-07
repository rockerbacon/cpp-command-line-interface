# CPP COMMAND LINE INTERFACE
CPP CLI is a simple library for easily creating Command Line Interfaces for C++ programs

#### Table of Contents
* [Building](#building)
* [Declaring arguments](#declaring-arguments)
	* [RequiredArgument](#requiredargument)
	* [OptionalArgument](#optionalargument)
	* [FlagArgument](#flagargument)

## Building the project
For building simply run the command:
```
./build.sh
```
For more informations see the documentation for the [assertions framework](https://github.com/rockerbacon/assertions)

## Declaring Arguments
Arguments are declared by using special Argument classes which wrap themselves around standard types. Arguments will always have a label and can also have an optional abbreviated label:
```
cli::RequiredArgument<int> x_number ("x-number"); // label without abbreviation
cli::RequiredArgument<float> y_number ("y-number", 'y'); // label with abbreviation
```

After the arguments are declared, the function _capture\_all\_arguments\_from_ is used to pass raw _main_ arguments into the declared arguments:
```
int main(int argc, char **argv) {
	/* argument declarations */
	cli::capture_all_arguments_from(argc, argv);
	/* ... */
```

A program built with the arguments above can be called like so, note that the arguments' order doesn't matter:
```
program --x-number 3 --y-number 4.25
program -y 4.25 --x-number 3
```

After values are captured inside the argument classes, their values can be easily accessed with the indirection operator ("*"):
```
int next_x_number = *x_number + 1
```

### RequiredArgument
An argument that must be passed to the program, otherwise an error is raised.
```
cli::RequiredArgument<int> required_integer("integer", 'i');
```

### OptionalArgument
An argument which doesn't need to be explicitly passed. A default value is used when the argument is not passed:
```
double low_precision_pi = 3.14;
cli::OptionalArgument<double> pi("pi", low_precision_pi, 'p');
```

### FlagArgument
This argument takes no value, it's merely a flag which tells whether the argument was present during the program call or not:
```
cli::FlagArgument should_print_hello("print_hello");
if (*should_print_hello)
	std::cout << "hello" << std::endl;
```

For the example above the call would be like so:
```
program --print_hello
```
Or:
```
program
```
