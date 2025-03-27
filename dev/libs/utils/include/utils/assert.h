//
// Created by bene on 3/27/25.
//

#pragma once

#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

#include "utils/types.h"

namespace flashlight {
namespace utils {

inline std::string formatArgName(const std::string& name) { return name.empty() ? "" : " " + name + " = "; }

inline std::string formatCallerName(const std::string& name) { return name.empty() ? "" : name + ": "; }

template <typename T>
concept ScalarType = std::is_arithmetic<T>::value || std::is_same<T, bool>::value;

/**
 * @brief Asserts that a given condition is true.
 *
 * Throws a std::runtime_error if the condition is false.
 * This function is useful for enforcing runtime constraints with a default error message.
 *
 * @param condition The boolean condition to check.
 * @throws std::runtime_error if the condition is false.
 */
inline void assertTrue(bool condition) {
    if (!condition) {
        throw std::runtime_error("Problem!");
    }
}

/**
 * @brief Asserts that a given condition is true, with a custom exception.
 *
 * Throws the provided exception if the condition is false.
 * This is useful when you want to throw specific exception types or messages.
 *
 * @tparam E Type of the exception to be thrown.
 * @param condition The boolean condition to check.
 * @param ex The exception object to throw if the condition is false.
 * @throws The provided exception if the condition is false.
 */
template <typename E>
inline void assertTrue(bool condition, E&& ex) {
    if (!condition) {
        throw std::forward<E>(ex);
    }
}

/**
 * @brief Asserts that a given value is greater than or equal to another.
 *
 * Throws a std::out_of_range exception if the value is less than the comparison target.
 *
 * @tparam T A scalar type that supports the `>=` and `std::to_string` operations.
 * @param val The value to check.
 * @param other The minimum acceptable value (inclusive).
 * @param callerName Name of the calling function or context (used for error reporting).
 *
 * @throws std::out_of_range if `val < other`.
 */
template <ScalarType T>
inline void assertLargerEq(const T& val, const T& other, std::string callerName = "") {
    auto condition = val >= other;

    auto msg =
        formatCallerName(callerName) + "Assertion failed: " + std::to_string(val) + " >= " + std::to_string(other);

    assertTrue(condition, std::out_of_range(msg));
}

/**
 * @brief Asserts that a given value is strictly smaller than another.
 *
 * Throws a std::out_of_range exception if the value is greater than or equal to the comparison target.
 *
 * @tparam T A scalar type that supports the `<` and `std::to_string` operations.
 * @param val The value to check.
 * @param other The upper limit (exclusive).
 * @param callerName Name of the calling function or context (used for error reporting).
 *
 * @throws std::out_of_range if `val >= other`.
 */
template <ScalarType T>
inline void assertSmaller(const T& val, const T& other, std::string callerName = "") {
    auto condition = val < other;
    auto msg =
        formatCallerName(callerName) + "Assertion failed: " + std::to_string(val) + " < " + std::to_string(other);
    assertTrue(condition, std::out_of_range(msg));
}

/**
 * @brief Asserts that a value lies within a specified half-open range [min, max).
 *
 * Throws a std::out_of_range exception if the value is outside the range.
 *
 * @tparam T A scalar type that supports `>=`, `<`, and `std::to_string`.
 * @param val The value to check.
 * @param range A pair defining the valid range [min, max), where `range.first` is inclusive and `range.second` is
 * exclusive.
 * @param callerName Name of the calling function or context (used for error reporting).
 *
 * @throws std::out_of_range if `val < range.first || val >= range.second`.
 */
template <ScalarType T>
inline void assertInRange(const T& val, const std::pair<T, T>& range, std::string callerName = "") {
    auto condition = range.first <= val && val < range.second;
    auto msg = formatCallerName(callerName) + "Assertion failed: " + std::to_string(range.first) +
               " <= " + std::to_string(val) + " < " + std::to_string(range.second);
    assertTrue(condition, std::out_of_range(msg));
}

// === Debug assertion macros ===
#ifdef NDEBUG
#define DEBUG_ONLY(x)  // for testing
#define ASSERT_TRUE(cond) ((void)0)
#define ASSERT_TRUE_MSG(cond, msg) ((void)0)
#define ASSERT_LARGER_EQ(val, other) ((void)0)
#define ASSERT_SMALLER(val, other) ((void)0)
#define ASSERT_IN_RANGE(val, range) ((void)0)
#else
#define DEBUG_ONLY(x) x

#define ASSERT_TRUE(cond)                                                                                          \
    assertTrue((cond), std::runtime_error(std::string("Assertion failed: '") + #cond + "' in " + __func__ + " [" + \
                                          __FILE__ + ":" + std::to_string(__LINE__) + "]"))

#define ASSERT_TRUE_MSG(cond, ex) assertTrue((cond), (ex))

#define ASSERT_LARGER_EQ(val, other) \
    assertLargerEq(val, other, std::string(__func__) + " [" + __FILE__ + ":" + std::to_string(__LINE__) + "] " + #val)

#define ASSERT_SMALLER(val, other) \
    assertSmaller(val, other, std::string(__func__) + " [" + __FILE__ + ":" + std::to_string(__LINE__) + "] " + #val)

#define ASSERT_IN_RANGE(val, range) \
    assertInRange(val, range, std::string(__func__) + " [" + __FILE__ + ":" + std::to_string(__LINE__) + "] " + #val)
#endif

}  // namespace utils
}  // namespace flashlight