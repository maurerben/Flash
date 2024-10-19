#define CATCH_CONFIG_MAIN
#include <config/parameters/Matrix.h>
#include <yaml-cpp/yaml.h>

#include <catch2/catch_all.hpp>
#include <complex>
#include <cstdint>
#include <iomanip>
#include <numbers>
#include <sstream>
#include <stdexcept>
#include <string>

namespace fcp = flash::config::parameters;

auto configFileWithMatrices = YAML::Load(
    R"(
    matrix: 
        - [1, 2, 3]
        - [4, 5, 6]
        - [7, 8, 9]
    scalar: 1
    vector: [1, 2, 3]
    matrixWrongElement:
        - [1,   2, 3 ]
        - [4, 5.6, 7 ]
        - [8,   9, 10]
    matrixWithWrongVector:
        - [1, 2, 3]
        - [4, 5]
        - [7, 8, 9]
    )");

TEST_CASE("Test flash::config::parameters::Matrix class for int type") {
    // Matrix parameter loads from config file
    Eigen::Matrix<int, 3, 3> referenceMatrix;
    referenceMatrix << 1, 2, 3,
                       4, 5, 6,
                       7, 8, 9;
    fcp::Matrix<int, 3, 3> matrixParam("matrix");
    matrixParam.load(configFileWithMatrices);
    REQUIRE(matrixParam == referenceMatrix);

    // Matrix parameter with default loads from config file
    Eigen::Matrix<int, 3, 3> defaultMatrix;
    defaultMatrix << 11, 12, 13,
                     14, 15, 16,
                     17, 18, 19;
    matrixParam = fcp::Matrix<int, 3, 3>("matrix", defaultMatrix);
    matrixParam.load(configFileWithMatrices);
    REQUIRE(matrixParam == referenceMatrix);

    // Matrix parameter with default loads from default
    matrixParam = fcp::Matrix<int, 3, 3>("notAKey", defaultMatrix);
    matrixParam.load(configFileWithMatrices);
    REQUIRE(matrixParam == defaultMatrix);

    // Matrix parameter initialized as 3 x 4 attempts to load matrix with 3 x 3 throws std::runtime_error
    fcp::Matrix<int, 3, 4> tooManyCols("matrix");
    REQUIRE_THROWS_AS(tooManyCols.load(configFileWithMatrices), std::runtime_error);

    // Matrix parameter initialized as 4 x 3 attempts to load matrix with 3 x 3 throws std::runtime_error
    fcp::Matrix<int, 4, 3> tooManyRows("matrix");
    REQUIRE_THROWS_AS(tooManyRows.load(configFileWithMatrices), std::runtime_error);

    // Matrix parameter attempt to load value from missing key throws std::runtime_error
    fcp::Matrix<int, 3, 3> missingKey("notAKey");
    REQUIRE_THROWS_AS(missingKey.load(configFileWithMatrices), std::runtime_error);

    // Matrix parameter attempt to load value from scalar throws std::runtime_error
    fcp::Matrix<int, 3, 3> scalarType("scalar");
    REQUIRE_THROWS_AS(scalarType.load(configFileWithMatrices), std::runtime_error);

    // Matrix parameter attempt to load value from vector throws std::runtime_error
    fcp::Matrix<int, 3, 3> vectorType("vector");
    REQUIRE_THROWS_AS(vectorType.load(configFileWithMatrices), std::runtime_error);

    // Matrix parameter attempt to load value from matrix where one element has the wrong type throws std::runtime_error
    fcp::Matrix<int, 3, 3> wrongTypeElement("matrixWrongElement");
    REQUIRE_THROWS_AS(wrongTypeElement.load(configFileWithMatrices), std::runtime_error);

    // Matrix parameter attempt to load value from matrix where one row is too short throws std::runtime_error
    fcp::Matrix<int, 3, 3> oneRowIsTooShort("matrixWithWrongVector");
    REQUIRE_THROWS_AS(oneRowIsTooShort.load(configFileWithMatrices), std::runtime_error);
}