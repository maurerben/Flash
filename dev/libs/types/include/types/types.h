#ifndef FLASH_TYPES_H
#define FLASH_TYPES_H

#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <array>
#include <complex>
#include <cstdint>
#include <string>

namespace flash {
namespace types {

/// Basic types
/// @brief Type for sizes.
typedef std::size_t my_size;
/// @brief Type for double precision floating point numbers
typedef std::double_t my_double;
/// @brief Type for double precision floating point complex numbers
typedef std::complex<my_double> complex_dp;

// Array types based on Eigen types
/// @brief Type for a dynamic vector
template<typename T>
using vector = Eigen::Vector<T, Eigen::Dynamic>;
/// @brief Type for a dynamic matrix
template<typename T>
using matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

}  // namespace types
}  // namespace flash

#endif  // FLASH_TYPES_H