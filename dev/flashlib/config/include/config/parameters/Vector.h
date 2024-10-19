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
    /// @brief Initialze with Vector#name
    /// @param[in] name
    Vector(const std::string& name) : Parameter<Eigen::Vector<T, N>>(name) {}

    /// @brief Initilize with Vector#name and Parameter#defaultValue
    /// @param[in] name
    /// @param[in] defaultValue
    Vector(const std::string& name, const Eigen::Vector<T, N>& defaultValue)
        : Parameter<Eigen::Vector<T, N>>(name, defaultValue) {}

    /**
     * @brief Setup Parameter#value
     * @details If \p node has no key Parameter#name, Parameter#value is set to Parameter#defaultValue
     * @param[in] cofigNode contains configuration
     * @throws runtime_error if \p node has no key Parameter#name and Parameter#defaultValue is not set or it does
     * not contain Cols elements
     */
    void load(const YAML::Node& node) {
        if (node[this->name]) {
            try {
                this->value = parseVectorFromNode(node[this->name]);
            } catch (std::runtime_error e) {
                throw std::runtime_error(e.what());
            } catch (...) {
                std::cout << "Caught an unknown exception!" << std::endl;
                throw std::runtime_error(this->name + " could not be loaded.");
            }
        } else if (this->defaultValue) {
            this->value = this->defaultValue.value();
        } else {
            throw std::runtime_error(this->name + " is not defined.");
        }
    }

   private:
    Eigen::Vector<T, N> parseVectorFromNode(const YAML::Node& vectorNode) {
        if (vectorNode.size() != N) {
            throw std::runtime_error(this->name + " has a not " + std::to_string(N) + " elements but " +
                                     std::to_string(vectorNode.size()) + ".");
        }
        Eigen::Vector<T, N> vector;
        for (std::size_t idx = 0; idx < N; ++idx) {
            try {
                vector(idx) = vectorNode[idx].template as<T>();
            } catch (...) {
                throw std::runtime_error(this->name + "[" + std::to_string(idx) + "] could not be loaded.");
            }
        }

        return vector;
    }
};

}  // namespace parameters
}  // namespace config
}  // namespace flash
