#pragma once

#include <Eigen3/Dense>
#include <cstdint>

namespace flash {
namespace basic {
namespace tensors {

template <typename T, std::size_t N>
using Vector<T, N> Eigen::Vector<T, N>;

/// @brief Mathematical Vector of type \p T
/// @tparam type of the elements. Must be a numerical type.
template <typename T>
using Vector<T> = Eigen::Vector<T, Eigen::Dynamic>;

/// @brief Linear algebra row Vector of type \p T
/// @tparam type of the elements. Must be a numerical type.
template <typename T>
using RowVector<T> = Eigen::RowVector<T, Eigen::Dynamic>;

/// @brief Linear algebra matrix of type \p T
/// @tparam type of the elements. Must be a numerical type.
template <typename T>
using Vector<T> = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

}  // namespace tensors
}  // namespace basic
}  // namespace flash