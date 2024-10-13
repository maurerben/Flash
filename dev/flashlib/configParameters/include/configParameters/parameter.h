#ifndef FLASH_INPUTPARAMETERS_PARAMETER_H
#define FLASH_INPUTPARAMETERS_PARAMETER_H
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
#include <stdexcept>
#include <iostream>
#include <optional>
#include <string>

#include <yaml-cpp/yaml.h>

namespace flash {
namespace configParameters {

/// @brief Parameter for configParameters.
/// @tparam T Type of the parameter
template <typename T>
class Parameter {
   public:
    // typedef T valueType;

    Parameter(const std::string& paramKey) : key(paramKey) {}

    Parameter(const std::string& paramKey, const T& defaulVal) : key(paramKey), defaultValue(defaulVal) {}

    /// @brief
    /// @param cofigNode
    void loadValue(const YAML::Node& cofigNode);

    operator T() const { return value; }

    // Friend function to overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const Parameter& param) {
        os << param.key << ": " << param.value;
        return os;
    }

   private:
    std::string key;
    T value;
    std::optional<T> defaultValue;
};

}  // namespace configParameters
}  // namespace flash

// Include the template implementation
#include "parameter.tpp"

#endif  // FLASH_INPUTPARAMETERS_PARAMETER_H