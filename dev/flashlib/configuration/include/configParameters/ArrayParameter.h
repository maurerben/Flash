#pragma once

#include <basic/types.h>

#include <array>
#include <ranges>
#include <stdexcept>

#include "Node.h"
#include "Parameter.h"

namespace flash {
namespace configParameters {

/// @brief Similar to Parameter but saves a matrix.
/// @tparam Type of the value that Parameter holds
template <typename T, basic::types::mySize N>
class ArrayParameter : public Parameter<std::array<T, N>> {
   public:
    /// @brief Initialze with Node#name
    /// @param[in] name
    ArrayParameter(const std::string& name) : Parameter<std::array<T, N>>(name) {}

    /// @brief Initilize with Node#name and Parameter#defaultValue
    /// @param[in] name
    /// @param[in] defaultValue
    ArrayParameter(const std::string& name, const std::array<T, N>& defaultValue)
        : Parameter<std::array<T, N>>(name, defaultValue) {}

    /// @brief Setup Parameter#value
    /// @details If \p configNode has no key Parameter#name, Parameter#value is set to Parameter#defaultValue
    /// @param[in] cofigNode contains configuration
    /// @throws runtime_error if \p configNode has no key Parameter#name and Parameter#defaultValue is not set or it
    /// does not contain N elements
    void load(const YAML::Node& configNode);

    /// @brief Friend function to overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const ArrayParameter& param) {
        os << param.name << ": " << " [";
        for (const auto& idx : std::ranges::iota_view(0, int(N))) {
            auto seperator = (idx == N - 1) ? "]" : ", ";
            os << std::to_string(param.value[idx]) << seperator;
        }
        return os;
    }
};

template <typename T, basic::types::mySize N>
void ArrayParameter<T, N>::load(const YAML::Node& configNode) {
    if (configNode[this->name]) {
        if (!configNode[this->name].IsSequence()) {
            throw std::runtime_error(this->name + " is not a sequence as expected.");
        }
        if (configNode[this->name].size() != N) {
            throw std::runtime_error(this->name + " has size " + std::to_string(configNode[this->name].size()) +
                                     " but expected is " + std::to_string(N) + ".");
        }
        for (const auto& idx : std::ranges::iota_view(0, int(N))) {
            this->value[idx] = configNode[this->name][idx].template as<T>();
        }
    } else if (this->defaultValue) {
        this->value = this->defaultValue.value();
    } else {
        throw std::runtime_error("Key " + this->name + " is not defined.");
    }
}

}  // namespace configParameters
}  // namespace flash