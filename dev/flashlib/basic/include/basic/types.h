#pragma once

#include <complex>
#include <cstdint>

namespace flash {
namespace basic {
namespace types {

/// @brief Define integer type
using myInt = std::int32_t;

/// @brief Define long uint type
using mySize = std::size_t;

/// @brief Define float type
using myFloat = std::float_t;

/// @brief Define double type
using myDouble = std::double_t;

/// @brief Define complex float type
using myComplex = std::complex<myFloat>;

/// @brief Define complex double size
using myZomplex = std::complex<myDouble>;  // the Z for double complex is an old LAPACK tradition

}  // namespace types
}  // namespace basic
}  // namespace flash