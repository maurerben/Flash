#define CATCH_PHYSICS_MAIN
#include <physics/ElectronIndex.h>
#include <physics/TransitionIndex.h>

#include <Eigen/Dense>
#include <catch2/catch_all.hpp>
#include <cstdint>
#include <iostream>

using flashlight::physics::ElectronIndices;
using flashlight::physics::TransitionIndex;
using flashlight::physics::TransitionIndices;
using flashlight::utils::index_t;

TEST_CASE("Test flashlight::physics::TransitionIndex class") {
    TransitionIndex index(1, 2, 3, 4);
    TransitionIndex jndex(1, 2, 3, 4);
    TransitionIndex kndex(5, 6, 7, 8);
    TransitionIndex lndex(1, 0, 4, 2);

    SECTION("Test constructor") {
        REQUIRE(index.getTransition() == 1);
        REQUIRE(index.getKpoint() == 2);
        REQUIRE(index.getOccupied() == 3);
        REQUIRE(index.getUnoccupied() == 4);
    }

    SECTION("Test operator==") {
        REQUIRE(index == jndex);
        REQUIRE_FALSE(index == kndex);
    }

    SECTION("Test operator!=") {
        REQUIRE(index != kndex);
        REQUIRE_FALSE(index != jndex);
    }
}

TEST_CASE("TransitionIndices construction and size", "[TransitionIndices]") {
    auto occ = ElectronIndices(2, 2);
    auto uno = ElectronIndices(2, 3, 10);

    TransitionIndices transitions(occ, uno);

    REQUIRE(transitions.NumKpoints() == 2);
    REQUIRE(transitions.NumOccupied() == 2 * 2);
    REQUIRE(transitions.NumUnoccupied() == 2 * 3);
    REQUIRE(transitions.size() == 2 * 2 * 3);  // k-points * occupied * unoccupied
}

TEST_CASE("TransitionIndices indexing", "[TransitionIndices]") {
    auto occ = ElectronIndices(1, 2, 1);  // 1 k-point, occupied: 2
    auto uno = ElectronIndices(1, 5, 5);  // unoccupied: 5

    TransitionIndices transitions(occ, uno);
    REQUIRE(transitions.size() == 1 * 2 * 5);

    auto t = transitions[0];
    REQUIRE(t.getTransition() == 0);
    REQUIRE(t.getKpoint() == 0);
    REQUIRE(t.getOccupied() == 1);
    REQUIRE(t.getUnoccupied() == 5);

    REQUIRE(transitions.atTransition(0).getOccupied() == 1);
    std::cout << transitions.atOccupiedUnoccupied(2, 5).getKpoint() << std::endl;
    REQUIRE(transitions.atOccupiedUnoccupied(2, 5).getKpoint() == 0);
}

TEST_CASE("Filtering by k-point, occupied and unoccupied bands", "[TransitionIndices]") {
    auto occ = ElectronIndices(2, 2);
    auto uno = ElectronIndices(2, 2, 10);

    TransitionIndices transitions(occ, uno);

    auto atK0 = transitions.atKpoint(0);
    REQUIRE(atK0.size() == 2 * 2);

    auto atOcc0 = transitions.atOccupied(0);
    REQUIRE(atOcc0.size() == 2);  // 2 unoccupied bands over 2 k-points, only occ==0

    auto atUno10 = transitions.atUnoccupied(10);
    REQUIRE(atUno10.size() == 2);  // 2 occupied bands over 2 k-points, only uno==10
}

TEST_CASE("TransitionIndices invalid access throws", "[TransitionIndices]") {
    auto occ = ElectronIndices(1, 1);
    auto uno = ElectronIndices(1, 1, 10);
    TransitionIndices transitions(occ, uno);

#ifndef NDEBUG
    REQUIRE_THROWS_AS(transitions[1], std::runtime_error);
    REQUIRE_THROWS_AS(transitions.atKpoint(5), std::runtime_error);
    REQUIRE_THROWS_AS(transitions.atOccupied(2), std::runtime_error);
    REQUIRE_THROWS_AS(transitions.atUnoccupied(100), std::runtime_error);
    REQUIRE_THROWS_AS(transitions.atTransition(5), std::runtime_error);
    REQUIRE_THROWS_AS(transitions.atOccupiedUnoccupied(0, 100), std::runtime_error);
#endif
}

TEST_CASE("TransitionIndices iterators", "[TransitionIndices]") {
    auto occ = ElectronIndices(1, 2, 1);
    auto uno = ElectronIndices(1, 2, 5);

    TransitionIndices transitions(occ, uno);

    index_t count = 0;
    for (auto t : transitions) {
        REQUIRE(t.getKpoint() == 0);
        ++count;
    }
    REQUIRE(count == 4);  // 2 occ * 2 uno
}
