//
// Created by bene on 3/27/25.
//
#define CATCH_UTILS_MAIN

#include <utils/assert.h>
#include <utils/types.h>

#include <catch2/catch_all.hpp>
#include <utility>

using namespace flashlight::utils;

TEST_CASE("assert: Test assert") {
    // If condition is true, pass this call
    assertTrue(true);
    // Throws std::runtime_error if no exception is passed
    REQUIRE_THROWS_AS(assertTrue(false), std::runtime_error);

    // If condition is true, pass this call
    assertTrue(true, std::runtime_error("error"));
    // Throws std::runtime_error if std::runtime exception is passed
    REQUIRE_THROWS_AS(assertTrue(false, std::runtime_error("error")), std::runtime_error);

    // If condition is true, pass this call
    assertTrue(true, std::out_of_range("error"));
    // Throws std::out_of_range if std::out_of_range exception is passed
    REQUIRE_THROWS_AS(assertTrue(false, std::out_of_range("error")), std::out_of_range);

    // If first value >= second value, pass the call
    assertLargerEq<index_t>(4, 4);
    assertLargerEq<index_t>(5, 4);
    // If first value < second value, throw std::out_of_range
    REQUIRE_THROWS_AS(assertLargerEq<index_t>(4, 5), std::out_of_range);

    // If first value < second value, pass the calls
    assertSmaller<index_t>(4, 5);
    // If first value == second value, throw std::out_of_range
    REQUIRE_THROWS_AS(assertSmaller<index_t>(4, 4), std::out_of_range);
    // If first value > second value, throw std::out_of_range
    REQUIRE_THROWS_AS(assertSmaller<index_t>(5, 4), std::out_of_range);

    // If first range.first <= value < range.second value, pass the call
    assertInRange<index_t>(0, {0, 3});
    assertInRange<index_t>(2, {0, 3});
    // If value < range.first, throw std::out_of_range
    REQUIRE_THROWS_AS(assertInRange<index_t>(1, {2, 3}), std::out_of_range);
    // If value == range.second, throw std::out_of_range
    REQUIRE_THROWS_AS(assertInRange<index_t>(3, {0, 3}), std::out_of_range);
    // If value > range.second, throw std::out_of_range
    REQUIRE_THROWS_AS(assertInRange<index_t>(4, {0, 3}), std::out_of_range);
}