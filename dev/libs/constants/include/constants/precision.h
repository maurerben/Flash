//
// Created by benedikt on 26.05.24.
//

#ifndef FLASH_CONSTANTS_PRECISION_H
#define FLASH_CONSTANTS_PRECISION_H

#include <cstdint>


namespace flash {
namespace constants {
namespace precison {


/// @brief Size of an integer in bytes
inline constexpr int size_of_int = 4;
/// @brief Size of a long unsigned integer in bytes
inline constexpr int  size_of_long_uint = 8;
/// @brief Size of a float in bytes
inline constexpr int  size_of_float = 4;
/// @brief Size of a double in bytes
inline constexpr int  size_of_double = 8;

}

#endif  // FLASH_CONSTANTS_PRECISION_H