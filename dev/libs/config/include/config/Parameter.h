#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

namespace flash {
namespace config {

/// @class Parameter
/// @brief Special Node that holds a value
/// @tparam Type of the value that Parameter holds
template <class T>
class Parameter {
   public:
    /// @brief Initialze with Parameter#key
    /// @param[in] key
    Parameter(const std::string& key) : key(key) {}

    /// @brief Initilize with Parameter#key and Parameter#defaultValue
    /// @param[in] key
    /// @param[in] defaultValue
    Parameter(const std::string& key, const T& defaultValue) : key(key), defaultValue(defaultValue) {}

    /// @brief Load Parameter#value from \p node
    /// @param node contains cofiguration file
    /// @throws std::runtime_error if loading Parameter%value fails
    virtual void load(const YAML::Node& node) {};

    T Value() const {return value;}

    /// @brief Transform instance of Parameter to \p T.
    operator T() const { return value; }

    // Overload == operator
    bool operator==(const T& other) const { return this->value == other; }

    // Friend function to overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const Parameter& param) {
        os << param.key << ": " << param.format << param.value;
        return os;
    }

   protected:
    /// @brief Name of the key of the parameter in the input file.
    std::string key;
    /// @brief Value of the parameter.
    T value;
    /// @brief Default value of the parameter. Does not need to be defined. See Parameter#load.
    std::optional<T> defaultValue;

    std::string format = "\n";
};

}  // namespace config
}  // namespace flash