#pragma once

#pragma once

#include <basic/types.h>

#include <Eigen/Dense>
#include <ranges>
#include <stdexcept>
#include <vector>
// #include <basic/tensors.h>

#include "Node.h"
#include "Parameter.h"
#include "ScalarParameter.h"

namespace flash {
namespace configParameters {

/**
 * @brief Similar to Parameter but saves a matrix.
 * @tparam T Type of the values saved to the matrix that Parameter holds
 * @tparam Rows is the number of rows
 * @tparam Cols is the number of columns
 */
template <Scalar T, std::size_t Rows, std::size_t Cols>
class MatrixParameter : public Parameter<Eigen::Matrix<T, Rows, Cols>> {
   public:
    /// @brief Initialze with Node#name
    /// @param[in] name
    MatrixParameter(const std::string& name) : Parameter<Eigen::Matrix<T, Rows, Cols>>(name) {}

    /// @brief Initilize with Node#name and Parameter#defaultValue
    /// @param[in] name
    /// @param[in] defaultValue
    MatrixParameter(const std::string& name, const Eigen::Matrix<T, Rows, Cols>& defaultValue)
        : Parameter<Eigen::Matrix<T, Rows, Cols>>(name, defaultValue) {}

    /**
     * @brief Setup Parameter#value
     * @details If \p configNode has no key Parameter#name, Parameter#value is set to Parameter#defaultValue
     * @param[in] cofigNode contains configuration
     * @throws runtime_error if \p configNode has no key Parameter#name and Parameter#defaultValue is not set or it does
     * not contain Cols elements
     */
    void load(const YAML::Node& configNode);

};

template<Scalar T, std::size_t N>
using VectorParameter = MatrixParameter<T, N, 1>;

template <Scalar T, std::size_t Rows, std::size_t Cols>
Eigen::Matrix<T, Rows, Cols> parseArrayFromNode(const YAML::Node& matrixNode) {

    Eigen::Matrix<T, Rows, Cols> matrix;
    for (std::size_t row=0; row<Rows; ++row) {
        for (std::size_t col=0; col<Cols; ++col) {
            matrix(row, col) = matrixNode[row][col].template as<T>();
        }
    }

    return matrix;
}

template <Scalar T, std::size_t Rows, std::size_t Cols>
void MatrixParameter<T, Rows, Cols>::load(const YAML::Node& configNode) {
    if (configNode[this->name]) {
        try {
            this->value = parseArrayFromNode<T, Rows, Cols>(configNode[this->name]);
        } catch (std::runtime_error e) {
            std::cout << e.what() << std::endl;
            throw std::runtime_error(this->name + " could not be loaded.");
        }
    } else if (this->defaultValue) {
        this->value = this->defaultValue.value();
    } else {
        throw std::runtime_error(this->name + " is not defined.");
    }
}

// if (!configNode[this->name].IsSequence()) {
//     throw std::runtime_error(this->name + " is not a sequence as expected.");
// }
// if (configNode[this->name].size() != Cols) {
//     throw std::runtime_error(this->name + " has size " + std::to_string(configNode[this->name].size()) +
//                              " but expected is " + std::to_string(Cols) + ".");
// }
// for (const auto& idx : std::ranges::iota_view(0, int(Cols))) {
//     this->value[idx] = configNode[this->name][idx].template as<T>();
// }

}  // namespace configParameters
}  // namespace flash
