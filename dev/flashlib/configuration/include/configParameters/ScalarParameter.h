#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <complex>

#include "Parameter.h"

namespace flash {
namespace configParameters {

template <typename T>
concept Scalar = 
    std::is_arithmetic<T>::value ||
    std::is_same<T, std::string>::value;

/// @class ScalarParameter
/// @brief Special Node that holds a scalar value
/// @tparam Type of the value that ScalarParameter holds
template <Scalar T>
class ScalarParameter : public Parameter<T> {
   public:
    /// @brief Initialze with Node#name
    /// @param[in] name
    ScalarParameter(const std::string& name) : Parameter<T>(name) {}

    /// @brief Initilize with Node#name and Parameter#defaultValue
    /// @param[in] name
    /// @param[in] defaultValue
    ScalarParameter(const std::string& name, const T& defaultValue) : Parameter<T>(name, defaultValue) {}

    /// @brief Setup Parameter#value
    /// @details If \p configNode has no key Parameter#name, Parameter#value is set to Parameter#defaultValue
    /// @param[in] cofigNode contains configuration
    /// @throws runtime_error if \p configNode has no key Parameter#name and Parameter#defaultValue is not set
    void load(const YAML::Node& cofigNode) override;
};

template <Scalar T>
void ScalarParameter<T>::load(const YAML::Node& configNode) {
    std::cout << 1 << std::endl;
    if (configNode[this->name]) {
        this->value = configNode[this->name].template as<T>();
    } else if (this->defaultValue) {
        this->value = this->defaultValue.value();
    } else {
        throw std::runtime_error("Key " + this->name + " is not defined.");
    }
}

}  // namespace configParameters
}  // namespace flash