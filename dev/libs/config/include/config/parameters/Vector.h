#pragma once

#include <config/Parameter.h>

#include <Eigen/Dense>
#include <ranges>
#include <stdexcept>
#include <string>

namespace flash {
namespace config {
namespace parameters {

template <typename T>
concept ArithmeticType = std::is_arithmetic<T>::value;

/**
 * @brief Similar to Parameter but saves a matrix.
 * @tparam T Type of the values saved to the matrix that Parameter holds
 * @tparam N is the number of elements
 */
template <ArithmeticType T, std::size_t N>
class Vector : public Parameter<Eigen::Vector<T, N>> {
   public:
    /// @brief Initialze with Vector#key
    /// @param[in] key
    Vector(const std::string& key) : Parameter<Eigen::Vector<T, N>>(key) {}

    /// @brief Initilize with Vector#key and Parameter#defaultValue
    /// @param[in] key
    /// @param[in] defaultValue
    Vector(const std::string& key, const Eigen::Vector<T, N>& defaultValue)
        : Parameter<Eigen::Vector<T, N>>(key, defaultValue) {}

    /// @brief See Parameter#load
    /// @param[in] node
    /// @throws std::runtime_error
    void load(const YAML::Node& node) {
        if (node[this->key]) {
            try {
                this->value = parseVectorFromNode(node[this->key]);
            } catch (std::runtime_error e) {
                throw std::runtime_error(e.what());
            } catch (...) {
                std::cout << "Caught an unknown exception!" << std::endl;
                throw std::runtime_error(this->key + " could not be loaded.");
            }
        } else if (this->defaultValue) {
            this->value = this->defaultValue.value();
        } else {
            throw std::runtime_error(this->key + " is not defined.");
        }
    }

    // Overload < operator
    auto operator<(const T& other) const { return (this->value.array() < other); }

    // Overload <= operator
    auto operator<=(const T& other) const { return (this->value.array() <= other); }

    // Overload > operator
    auto operator>(const T& other) const { return (this->value.array() > other); }

    // Overload >= operator
    auto operator>=(const T& other) const { return (this->value.array() >= other); }

   private:
    Eigen::Vector<T, N> parseVectorFromNode(const YAML::Node& vectorNode) {
        if (vectorNode.size() != N) {
            throw std::runtime_error(this->key + " has a not " + std::to_string(N) + " elements but " +
                                     std::to_string(vectorNode.size()) + ".");
        }
        Eigen::Vector<T, N> vector;
        for (std::size_t idx = 0; idx < N; ++idx) {
            try {
                vector(idx) = vectorNode[idx].template as<T>();
            } catch (...) {
                throw std::runtime_error(this->key + "[" + std::to_string(idx) + "] could not be loaded.");
            }
        }

        return vector;
    }
};


/// @brief Parameter type for sampling
using Sampling = Vector<std::size_t, 3>;
/// @brief Parameter type for coordinate
using Coordinate = Vector<std::double_t, 3>;
/// @brief Paramter type for interval
using Interval = Vector<std::double_t, 2>;

}  // namespace parameters
}  // namespace config
}  // namespace flash
