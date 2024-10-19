#pragma once

#include <config/Parameter.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace flash {
namespace config {
namespace parameters {

template <typename T>
concept ScalarType = std::is_arithmetic<T>::value || std::is_same<T, bool>::value;

/// @class Scalar
/// @brief Parameterthat holds a scalar value
/// @tparam Type of the value that Scalar holds
template <ScalarType T>
class Scalar : public Parameter<T> {
   public:
    /// @brief Initialze with Node#name
    /// @param[in] name
    Scalar(const std::string& name) : Parameter<T>(name) {}

    /// @brief Initilize with Node#name and Parameter#defaultValue
    /// @param[in] name
    /// @param[in] defaultValue
    Scalar(const std::string& name, const T& defaultValue) : Parameter<T>(name, defaultValue) {}

    /// @brief Setup Parameter#value
    /// @details If \p node has no key Parameter#name, Parameter#value is set to Parameter#defaultValue
    /// @param[in] node contains configuration
    /// @throws runtime_error if \p node has no key Parameter#name and Parameter#defaultValue is not set
    void load(const YAML::Node& node) override;
};

template <ScalarType T>
void Scalar<T>::load(const YAML::Node& node) {
    if (node[this->name]) {
        if (node[this->name].IsScalar()) {
            try {
                this->value = node[this->name].template as<T>();
            } catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
                throw std::runtime_error(this->name + " could not be loaded.");
            } catch (...) {
                std::cout << "Caught an unknown exception!" << std::endl;
                throw std::runtime_error(this->name + " could not be loaded.");
            }
        } else {
            throw std::runtime_error(this->name + " could not be loaded. Expected a scalar node.");
        }
    } else if (this->defaultValue) {
        this->value = this->defaultValue.value();
    } else {
        throw std::runtime_error(this->name + " is not defined.");
    }
}

}  // namespace parameters
}  // namespace config
}  // namespace flash