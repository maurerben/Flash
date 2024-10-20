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
template <ScalarType T>
class Scalar : public Parameter<T> {
   public:
    /// @brief Initialze with Node#key
    /// @param[in] key
    Scalar(const std::string& key) : Parameter<T>(key) {}

    /// @brief Initilize with Node#key and Parameter#defaultValue
    /// @param[in] key
    /// @param[in] defaultValue
    Scalar(const std::string& key, const T& defaultValue) : Parameter<T>(key, defaultValue) {}

    /// @brief See Parameter#load
    /// @param[in] node
    /// @throws std::runtime_error
    void load(const YAML::Node& node) override;

    // Overload < operator
    bool operator<(const T& other) const { return this->value < other; }

    // Overload <= operator
    bool operator<=(const T& other) const { return this->value <= other; }

    // Overload > operator
    bool operator>(const T& other) const { return this->value > other; }

    // Overload >= operator
    bool operator>=(const T& other) const { return this->value >= other; }
};

template <ScalarType T>
void Scalar<T>::load(const YAML::Node& node) {
    if (node[this->key]) {
        if (node[this->key].IsScalar()) {
            try {
                this->value = node[this->key].template as<T>();
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
}

/// @brief Parameter for the count of something
using Size = Scalar<std::size_t>;
/// @brief Parameter for the factor of something
using Double = Scalar<std::double_t>;

}  // namespace parameters
}  // namespace config
}  // namespace flash