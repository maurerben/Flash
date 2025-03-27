#define CATCH_CONFIG_MAIN
#include <config/nodes/ElectronicStates.h>
#include <constants/tensor.h>
#include <utils/types.h>
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

using namespace flashlight::utils;
namespace fcn = flashlight::config::nodes;

auto configFile = YAML::Load(
    R"(
    valid:
        electronicStates:
            k_grid:
                sampling: [1, 2, 3]
            n_bands: 15

    invalid:
        missingKGrid:
            n_bands: 15

        missingNStates:
            k_grid:
                sampling: [1, 2, 3]

        invalidKGrid:
            k_grid:
                sampling: [1, 0, 3]
            n_bands: 15

        nStatesEqualOne:
            k_grid:
                sampling: [1, 2, 3]
            n_bands: 1
    )");

TEST_CASE("Test flashlight::config::nodes::ElectronicStates class") {
    // ElectronicStates loads from config file node with full definition
    Eigen::Vector<index_t, 3> referenceSampling{1, 2, 3};
    auto referenceOffset = flashlight::constants::tensor::origin;
    auto referenceParallelepiped = flashlight::constants::tensor::identity3D;
    fcn::ElectronicStates electronicStates("electronicStates");
    electronicStates.load(configFile["valid"]);
    REQUIRE(electronicStates.kGrid.sampling == referenceSampling);
    REQUIRE(electronicStates.kGrid.offset == referenceOffset);
    REQUIRE(electronicStates.kGrid.parallelepiped == referenceParallelepiped);
    REQUIRE(electronicStates.nBands == static_cast<index_t>(15));

    // GrounState attempts to load from undefined node throws std::runtime_error
    fcn::ElectronicStates missingNode("notAKey");
    REQUIRE_THROWS_AS(missingNode.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with undefined kGrid throws std::runtime_error
    fcn::ElectronicStates missingKGrid("missingKGrid");
    REQUIRE_THROWS_AS(missingKGrid.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with undefined nBands throws std::runtime_error
    fcn::ElectronicStates missingNStates("missingNStates");
    REQUIRE_THROWS_AS(missingNStates.load(configFile["invalid"]), std::runtime_error);

    // GrounState attempts to load from node with invalid kGrid throws std::runtime_error
    fcn::ElectronicStates invalidKGrid("invalidKGrid");
    REQUIRE_THROWS_AS(invalidKGrid.load(configFile["invalid"]), std::runtime_error);
}
