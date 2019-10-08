#pragma once

#include <unordered_map>
#include <sstream>

#ifdef __GNUC__
	#define UNUSED __attribute__((unused))
#else
	#define UNUSED
#endif

namespace cli {

	class ArgumentInterface {
		public:
			virtual void read_value_from(const char* arg) = 0;
			virtual bool is_required(void) const = 0;
			virtual bool requires_value(void) const = 0;
			virtual bool is_present(void) const = 0;
			virtual void set_as_present(void) = 0;
	};

	extern ::std::unordered_map<::std::string, ArgumentInterface*> argument_map;

	class Argument : public ArgumentInterface {
		protected:
			bool present;
		public:
			Argument(const std::string &argument_label, char argument_abbreviation='\0');

			virtual bool is_present (void) const;
			virtual void set_as_present(void);
	};

	template<typename T>
	class ValuedArgument : public Argument {
		protected:
			T value;
		public:
			ValuedArgument(const std::string &argument_label, char argument_abbreviation='\0')
				:	Argument(argument_label, argument_abbreviation)
			{}

			virtual T& operator* (void) {
				return this->value;
			}
			virtual const T& operator* (void) const {
				return this->value;
			}
			virtual T& operator-> (void) {
				return this->value;
			}
			virtual const T& operator-> (void) const {
				return this->value;
			}

			virtual void read_value_from(const char* arg) {
				std::istringstream stream(arg);
				stream >> this->value;
				if (stream.fail()) {
					throw std::invalid_argument("Could not parse '"+std::string(arg)+"' to the correct type");
				}
				this->present = true;
			}

			bool requires_value(void) const {
				return true;
			}
	};

	class UnvaluedArgument : public Argument {
		public:
			UnvaluedArgument(const std::string &argument_label, char argument_abbreviation='\0');
		
			void read_value_from(UNUSED const char *arg);
			bool requires_value(void) const;
	};
}
