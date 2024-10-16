#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "Node.h"

namespace flash {
namespace configParameters {

/// @class Parameter
/// @brief Special Node that holds a value
/// @tparam Type of the value that Parameter holds
template <class T>
class Parameter {
   public:
    /// @brief Initialze with Node#name
    /// @param[in] name
    Parameter(const std::string& name) : name(name) {}

    /// @brief Initilize with Node#name and Parameter#defaultValue
    /// @param[in] name
    /// @param[in] defaultValue
    Parameter(const std::string& name, const T& defaultValue) : name(name), defaultValue(defaultValue) {}

    /// @brief Load configuration from \a configNode
    /// @param configNode contains cofiguration file
    /// @throws std::runtime_error if something goes wrong during loading
    virtual void load(const YAML::Node& configNode) {};

    /// @brief Transform instance of Parameter to \p T.
    operator T() const { return value; }

    /// @brief Overload == operator
    bool operator==(const T& other) const {
        return this->value == other;
    }

    /// @brief Friend function to overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const Parameter& param) {
        os << param.name << param.format << ": " << param.value;
        return os;
    }

   protected:
    /// @brief name of the key of the parameter in the input file
    std::string name;
    /// @brief nalue of the parameter
    T value;
    /// @brief Default value of the parameter. Does not need to be defined. See Parameter#load.
    std::optional<T> defaultValue;

    std::string format = "";
};

}  // namespace configParameters
}  // namespace flash