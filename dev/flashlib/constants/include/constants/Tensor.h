#pragma once

#include <Eigen/Dense>
#include <cstdint>

namespace flash {
namespace constants {
namespace Tensor {

/// @brief Singular sampling with 1 point per dimension
const auto singularSampling = Eigen::Vector<std::size_t, 3>::Constant(1);
/// @brief Zero offset
const auto zeroOffset = Eigen::Vector<std::double_t, 3>::Zero();
/// @brief Identity matrix for 3 dimensional space
const auto identity3D = Eigen::Matrix<std::double_t, 3, 3>::Identity();

}  // namespace Tensor
}  // namespace constants
}  // namespace flash