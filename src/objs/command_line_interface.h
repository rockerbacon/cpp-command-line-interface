#pragma once

#include "base_arguments.h"
#include <functional>

namespace cli {

	template<typename T>	
	class RequiredArgument : public ValuedArgument<T> {
		public:
			RequiredArgument(const std::string &argument_label, char argument_abbreviation='\0') 
				:	ValuedArgument<T>(argument_label, argument_abbreviation) {}

			bool is_required (void) const {
				return true;
			}

	};

	template<typename T>
	class OptionalArgument : public ValuedArgument<T> {
		public:
			OptionalArgument(const std::string &argument_label, const T& default_value, char argument_abbreviation='\0')
			   :	ValuedArgument<T>(argument_label, argument_abbreviation)
			{
				this->value = default_value;
			}

			bool is_required (void) const {
				return false;
			}

	};

	class FlagArgument : public UnvaluedArgument {
		public:
			FlagArgument(const std::string &argument_label, char argument_abbreviation='\0');

			bool is_required(void) const;

			bool operator*(void) const;
	};

	class FunctionArgument : public UnvaluedArgument {
		private:
			::std::function<void(void)> function;
		public:
			FunctionArgument(const decltype(function) &function, const std::string &argument_label, char argument_abbreviation='\0');

			bool is_required(void) const;
			void set_as_present(void);
	};

	class HelpArgument : public FunctionArgument {
		public:
			HelpArgument(void);
	};

	void capture_all_arguments_from(int argc, char **argv);

}
