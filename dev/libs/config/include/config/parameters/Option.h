#pragma once

#include <config/Parameter.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>

namespace flash {
namespace config {
namespace parameters {

typedef std::unordered_set<std::string> options_t;

/// @class Option parameter class
class Option : public Parameter<std::string> {
   public:
    /// @brief Initialze with Node#key
    /// @param[in] key Name that is the key of the value that will be loaded from the config file.
    /// @param[in] options Option#value need to be initialized as one of the options.
    Option(const std::string& key, const options_t& options) : Parameter<std::string>(key), options(options) {}

    /// @brief Initilize with Node#key and Parameter#defaultValue
    /// @param[in] key
    /// @param[in] defaultValue
    Option(const std::string& key, const options_t& options, const std::string& defaultValue)
        : Parameter<std::string>(key, defaultValue), options(options) {
        if (options.find(defaultValue) == options.end()) {
            std::string error_message =

                this->key + " was initialized with defaultValue '" + this->defaultValue.value() +
                "' which is none of the allowed options: " + optionsToString();
            throw std::runtime_error(error_message);
        }
    }

    /// @brief Setup Parameter#value
    /// @details If \p node has no key Parameter#key, Parameter#value is options to Parameter#defaultValue
    /// @param[in] node contains configuration
    /// @throws runtime_error if \p node has no key Parameter#key and Parameter#defaultValue is not options
    void load(const YAML::Node& node) override {
        if (node[this->key]) {
            if (node[this->key].IsScalar()) {
                try {
                    this->value = node[this->key].as<std::string>();
                } catch (std::runtime_error e) {
                    std::cout << e.what() << std::endl;
                    throw std::runtime_error(this->key + " could not be loaded.");
                } catch (...) {
                    std::cout << "Caught an unknown exception!" << std::endl;
                    throw std::runtime_error(this->key + " could not be loaded.");
                }
            } else {
                throw std::runtime_error(this->key + " could not be loaded. Expected a scalar node.");
            }
        } else if (this->defaultValue) {
            this->value = this->defaultValue.value();
        } else {
            throw std::runtime_error(this->key + " is not defined.");
        }
        if (options.find(this->value) == options.end()) {
            std::string error_message = this->key + " loaded value '" + this->value +
                                        "' which is none of the allowed options: " + optionsToString();
            throw std::runtime_error(error_message);
        }
    }

   protected:
    options_t options;

   private:
    std::string optionsToString() {
        const std::string delimiter = "', '";
        const std::string first = "{'";
        const std::string last = "'}";
        auto options_string =
            std::accumulate(std::next(options.begin()), options.end(), *options.begin(),
                            [&delimiter](std::string a, const std::string& b) { return std::move(a) + delimiter + b; });

        return first + options_string + last;
    }
};

}  // namespace parameters
}  // namespace config
}  // namespace flash