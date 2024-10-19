#pragma once

#include <config/Parameter.h>

#include <Eigen/Dense>
#include <stdexcept>
#include <string>

#include "Vector.h"

namespace flash {
namespace config {
namespace parameters {

/**
 * @brief Similar to Parameter but saves a matrix.
 * @tparam T Type of the values saved to the matrix that Parameter holds
 * @tparam Rows is the number of rows
 * @tparam Cols is the number of columns
 */
template <ArithmeticType T, std::size_t Rows, std::size_t Cols>
class Matrix : public Parameter<Eigen::Matrix<T, Rows, Cols>> {
   public:
    /// @brief Initialze with Matrix#name
    /// @param[in] name
    Matrix(const std::string& name) : Parameter<Eigen::Matrix<T, Rows, Cols>>(name) {}

    /// @brief Initilize with Matrix#name and Parameter#defaultValue
    /// @param[in] name
    /// @param[in] defaultValue
    Matrix(const std::string& name, const Eigen::Matrix<T, Rows, Cols>& defaultValue)
        : Parameter<Eigen::Matrix<T, Rows, Cols>>(name, defaultValue) {}

    /**
     * @brief Setup Parameter#value
     * @details If \p node has no key Parameter#name, Parameter#value is set to Parameter#defaultValue
     * @param[in] node contains configuration
     * @throws runtime_error if \p node has no key Parameter#name and Parameter#defaultValue is not set or it does
     * not contain Cols elements
     */
    void load(const YAML::Node& node) {
        if (node[this->name]) {
            try {
                this->value = parseMatrixFromNode(node[this->name]);
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
    Eigen::Matrix<T, Rows, Cols> parseMatrixFromNode(const YAML::Node& matrixNode) {
        if (matrixNode.size() != Rows) {
            throw std::runtime_error(this->name + " has a not " + std::to_string(Rows) + " rows but " +
                                     std::to_string(matrixNode.size()) + ".");
        }
        Eigen::Matrix<T, Rows, Cols> matrix;
        for (std::size_t row = 0; row < Rows; ++row) {
            if (matrixNode[row].size() != Cols) {
                throw std::runtime_error(this->name + "[" + std::to_string(row) + "]" + " has a not " +
                                         std::to_string(Cols) + " cols but " + std::to_string(matrixNode[row].size()) +
                                         ".");
            }
            if (matrixNode[row].IsScalar()) {
                throw std::runtime_error(this->name + "[" + std::to_string(row) + "]" +
                                         " is a scalar, expected a vector.");
            }
            for (std::size_t col = 0; col < Cols; ++col) {
                try {
                    matrix(row, col) = matrixNode[row][col].template as<T>();
                } catch (...) {
                    throw std::runtime_error(this->name + "[" + std::to_string(row) + ", " + std::to_string(col) +
                                             "] could not be loaded.");
                }
            }
        }
        return matrix;
    }
};

}  // namespace parameters
}  // namespace config
}  // namespace flash
