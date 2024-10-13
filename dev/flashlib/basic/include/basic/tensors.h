#ifndef FLASH_BASIC_TENSORS_H
#define FLASH_BASIC_TENSORS_H

#include <Eigen3/Dense>

namespace flash {
namespace basic {
namespace tensors {

/// @brief Mathematical vector of type \p T
/// @tparam type of the elements. Must be a numerical type.
template <typename T>
using vector<T> = Eigen::Vector<T, Eigen::Dynamic>;

/// @brief Linear algebra row vector of type \p T
/// @tparam type of the elements. Must be a numerical type.
template <typename T>
using rowVector<T> = Eigen::RowVector<T, Eigen::Dynamic>;

/// @brief Linear algebra matrix of type \p T
/// @tparam type of the elements. Must be a numerical type.
template <typename T>
using vector<T> = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

}  // namespace tensors
}  // namespace basic
}  // namespace flash

#endif  // FLASH_BASIC_TENSORS_H