#define CATCH_CONFIG_MAIN
#include <config/nodes/GroundState.h>
#include <constants/tensor.h>
#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <catch2/catch_all.hpp>
#include <complex>
#include <cstdint>
#include <iomanip>
#include <numbers>
#include <sstream>
#include <stdexcept>
#include <string>

namespace fcn = flash::config::nodes;

auto configFile = YAML::Load(
    R"(
    valid:
        groundState:
            k_grid:
                sampling: [1, 2, 3]
            n_states: 15
            occupied_part: 0.5

    invalid:
        missingKGrid:
            n_states: 15
            occupied_part: 0.5

        missingNStates:
            k_grid:
                sampling: [1, 2, 3]
            occupied_part: 0.5

        missingOccupiedPart:
            k_grid:
                sampling: [1, 2, 3]
            n_states: 15

        invalidKGrid:
            k_grid:
                sampling: [1, 0, 3]
            n_states: 15
            occupied_part: 0.5

        nStatesEqualOne:
            k_grid:
                sampling: [1, 2, 3]
            n_states: 1
            occupied_part: 0.5

        occupiedPartEqualZero:
            k_grid:
                sampling: [1, 2, 3]
            n_states: 15
            occupied_part: 0.0

        occupiedPartEqualOne:
            k_grid:
                sampling: [1, 2, 3]
            n_states: 15
            occupied_part: 1.0

        nOccupiedOrbitalsNotIntegral:
            k_grid:
                sampling: [1, 1, 1]
            n_states: 2
            occupied_part: 0.4

    )"
);

TEST_CASE("Test flash::config::nodes::GroundState class") {
    // GroundState loads from config file node with full definition
    Eigen::Vector<std::size_t, 3> referenceSampling{1, 2, 3};
    auto referenceOffset = flash::constants::tensor::origin;
    auto referenceParallelepiped = flash::constants::tensor::identity3D;
    fcn::GroundState groundState("groundState");
    groundState.load(configFile["valid"]);
    REQUIRE(groundState.kGrid.sampling == referenceSampling);
    REQUIRE(groundState.kGrid.offset == referenceOffset);
    REQUIRE(groundState.kGrid.parallelepiped == referenceParallelepiped);
    REQUIRE(groundState.nStates == static_cast<std::size_t>(15));
    REQUIRE(groundState.occupiedPart == 0.5);

    // GrounState attempts to load from undefined node throws std::runtime_error
    fcn::GroundState missingNode("notAKey");
    REQUIRE_THROWS_AS(missingNode.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with undefined kGrid throws std::runtime_error
    fcn::GroundState missingKGrid("missingKGrid");
    REQUIRE_THROWS_AS(missingKGrid.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with undefined nStates throws std::runtime_error
    fcn::GroundState missingNStates("missingNStates");
    REQUIRE_THROWS_AS(missingNStates.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with undefined occupiedPart throws std::runtime_error
    fcn::GroundState missingOccupiedPart("missingOccupiedPart");
    REQUIRE_THROWS_AS(missingOccupiedPart.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with invalid kGrid throws std::runtime_error
    fcn::GroundState invalidKGrid("invalidKGrid");
    REQUIRE_THROWS_AS(invalidKGrid.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with occupiedPart defined as 0.0 throws std::runtime_error
    fcn::GroundState occupiedPartEqualZero("occupiedPartEqualZero");
    REQUIRE_THROWS_AS(occupiedPartEqualZero.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with occupiedPart defined as 1.0 throws std::runtime_error
    fcn::GroundState occupiedPartEqualOne("occupiedPartEqualOne");
    REQUIRE_THROWS_AS(occupiedPartEqualOne.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from nodesetup such that the number of occupied orbitals would not be an integral number throws std::runtime_error
    fcn::GroundState nOccupiedOrbitalsNotIntegral("nOccupiedOrbitalsNotIntegral");
    REQUIRE_THROWS_AS(nOccupiedOrbitalsNotIntegral.load(configFile["invalid"]), std::runtime_error);
}
