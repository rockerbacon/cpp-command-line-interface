#pragma once

#include "base_arguments.h"
#include <functional>

namespace cli {

	struct Program {
		string title;
		string version;
		string description;
	};

	extern Program this_program;

	template<typename T>	
	class RequiredArgument : public ValuedArgument<T> {
		public:
			RequiredArgument(const string &argument_label, char argument_abbreviation='\0', const string &description="required") 
				:	ValuedArgument<T>(argument_label, description, argument_abbreviation) {}

			bool is_required (void) const {
				return true;
			}

	};

	template<typename T>
	class OptionalArgument : public ValuedArgument<T> {
		public:
			OptionalArgument(const T& default_value, const string &argument_label, char argument_abbreviation='\0', const string &description="optional")
			   :	ValuedArgument<T>(argument_label, description, argument_abbreviation)
			{
				this->value = default_value;
			}

			bool is_required (void) const {
				return false;
			}

	};

	class FlagArgument : public UnvaluedArgument {
		public:
			FlagArgument(const string &argument_label, char argument_abbreviation='\0', const string &description="flag");

			bool is_required(void) const;

			bool operator*(void) const;
	};

	class FunctionArgument : public UnvaluedArgument {
		private:
			function<void(void)> execute;
		public:
			FunctionArgument(const decltype(execute) &function_to_execute, const string &argument_label, char argument_abbreviation='\0', const string &description="optional execution flow");

			bool is_required(void) const;
			void set_as_present(void);
	};

	class HelpArgument : public FunctionArgument {
		public:
			HelpArgument(void);
	};

	void capture_all_arguments_from(int argc, char **argv);

}
