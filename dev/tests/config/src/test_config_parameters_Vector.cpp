#define CATCH_CONFIG_MAIN
#include <config/parameters/Vector.h>
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

namespace fcp = flash::config::parameters;

auto configFile = YAML::Load(
    R"(
    vector: [1, 2, 3]
    scalar: 1
    vectorWrongElement: [1, 2, 3.4]
    vectorVectorElement:
        - 1 
        - 2
        - [3, 4, 5]
    matrix:
        - [1, 2]
        - [3, 4]
    )");

TEST_CASE("Test flash::config::parameters::Vector class for integer type") {
    // Vector parameter is loads from config file
    Eigen::Vector<int, 3> referenceVector{1, 2, 3};
    fcp::Vector<int, 3> vectorParam("vector");
    vectorParam.load(configFile);
    REQUIRE(vectorParam == referenceVector);

    // Vector parameter with default loads from config file
    Eigen::Vector<int, 3> defaultVector{4, 5, 6};
    vectorParam = fcp::Vector<int, 3>("vector", defaultVector);
    vectorParam.load(configFile);
    REQUIRE(vectorParam == referenceVector);

    // Vector parameter with default is loads from default file
    vectorParam = fcp::Vector<int, 3>("notAKey", defaultVector);
    vectorParam.load(configFile);
    REQUIRE(vectorParam == defaultVector);

    // Vector parameter initialized with size 4 attempt to load an vector with size 3 throws std::runtime_error
    fcp::Vector<int, 4> tooLong("vector");
    REQUIRE_THROWS_AS(tooLong.load(configFile), std::runtime_error);

    // Vector parameter attempt to load value from missing key throws std::runtime_exception
    fcp::Vector<int, 3> missingKey("notAKey");
    REQUIRE_THROWS_AS(missingKey.load(configFile), std::runtime_error);

    // Vector parameter attempt to load scalar value throws std::runtime_exception
    fcp::Vector<int, 3> scalarType("scalar");
    REQUIRE_THROWS_AS(scalarType.load(configFile), std::runtime_error);

    // Vector parameter attempt to load a vector where one element has the wrong type throws
    // std::runtime_exception
    fcp::Vector<int, 3> wrongTypeElement("vectorWrongElement");
    REQUIRE_THROWS_AS(wrongTypeElement.load(configFile), std::runtime_error);

    // Vector parameter attempt to load a vector where one element is a vector std::runtime_exception
    fcp::Vector<int, 3> oneElementIsVector("vectorVectorElement");
    REQUIRE_THROWS_AS(oneElementIsVector.load(configFile), std::runtime_error);

    // Vector parameter attempt to load a matrix throws std::runtime_exception
    fcp::Vector<int, 2> isAMatrix("matrix");
    REQUIRE_THROWS_AS(isAMatrix.load(configFile), std::runtime_error);
}