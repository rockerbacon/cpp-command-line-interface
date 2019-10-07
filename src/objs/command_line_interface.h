#pragma once

#include <unordered_map>
#include <sstream>

namespace cli {


	class ArgumentInterface {
		public:
			virtual void read_value_from(const char* arg) = 0;
			virtual bool is_required(void) = 0;
			virtual bool requires_value(void) = 0;
			virtual bool is_present(void) = 0;
			virtual void set_as_present(void) = 0;
	};

	extern std::unordered_map<std::string, ArgumentInterface*> argument_map;

	template<typename T>
	class Argument : public ArgumentInterface {
		protected:
			T value;
			bool present;
		public:
			Argument(const std::string &argument_label, char argument_abbreviation='\0') 
				:	present(false)
			{
				std::string prefixed_label = "--" + argument_label;

				if (cli::argument_map.find(prefixed_label) != cli::argument_map.end()) {
					throw std::invalid_argument("Cannot define two arguments with the same label");
				}
				cli::argument_map[prefixed_label] = this;

				if (argument_abbreviation != '\0') {
					std::string prefixed_abbreviation = "-" + std::string(1, argument_abbreviation);

					if (cli::argument_map.find(prefixed_abbreviation) != cli::argument_map.end()) {
						throw std::invalid_argument("Cannot define two arguments with the same abbreviation");
					}
					cli::argument_map[prefixed_abbreviation] = this;
				}
			}

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

			virtual bool is_present (void) {
				return this->present;
			}

			virtual void set_as_present(void) {
				this->present = true;
			}
	};

	template<typename T>	
	class RequiredArgument : public Argument<T> {
		public:
			RequiredArgument(const std::string &argument_label, char argument_abbreviation='\0') : Argument<T>(argument_label, argument_abbreviation) {}

			bool is_required (void) {
				return true;
			}

			bool requires_value (void) {
				return true;
			}
	};

	template<typename T>
	class OptionalArgument : public Argument<T> {
		public:
			OptionalArgument(const std::string &argument_label, const T& default_value, char argument_abbreviation='\0')
			   :	Argument<T>(argument_label, argument_abbreviation)
			{
				this->value = default_value;
			}

			bool is_required (void) {
				return false;
			}

			bool requires_value (void) {
				return true;
			}
	};

	class FlagArgument : public Argument<bool> {
		public:
			FlagArgument(const std::string &argument_label, char argument_abbreviation='\0');

			bool is_required(void);
			bool requires_value(void);
			void set_as_present(void);
	};

	void capture_all_arguments_from(int argc, char **argv);

}
