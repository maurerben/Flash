#define CATCH_CONFIG_MAIN
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip> 
#include <complex>
#include <numbers>
#include <stdexcept>
#include <Eigen/Dense>
#include <yaml-cpp/yaml.h>
#include <catch2/catch_all.hpp>
#include <configParameters/ScalarParameter.h>
#include <configParameters/MatrixParameter.h>



TEST_CASE("Test ScalarParameter class") {

    // Define pi with 20 digits
    std::ostringstream oss;
    oss << std::setprecision(20) << std::numbers::pi;
    auto pi_20 = oss.str(); 


    flash::configParameters::ScalarParameter<int> myIntParam("int");
    myIntParam.load(YAML::Load("int: 3"));
    REQUIRE(myIntParam == 3);
    REQUIRE_THROWS_AS(myIntParam.load(YAML::Load("ont: 3")), std::runtime_error);


    flash::configParameters::ScalarParameter<int> myDefaultParam("int", 5);
    myDefaultParam.load(YAML::Load("int: 3"));
    REQUIRE(myDefaultParam == 3);
    myDefaultParam.load(YAML::Load("ont: 3"));
    REQUIRE(myDefaultParam == 5); // set value to defaultMatrix
    

    flash::configParameters::ScalarParameter<std::float_t> myFloatParam("float");
    myFloatParam.load(YAML::Load("float: " + pi_20));
    REQUIRE(myFloatParam == std::float_t(std::numbers::pi));


    flash::configParameters::ScalarParameter<double> myDoubleParam("double");
    myDoubleParam.load(YAML::Load("double: " + pi_20));
    REQUIRE(myDoubleParam == std::double_t(std::numbers::pi));


    flash::configParameters::ScalarParameter<std::string> myStringParam("string");
    myStringParam.load(YAML::Load("string: three"));
    REQUIRE(myStringParam == std::string("three"));

}


TEST_CASE("Test MatrixParameter class") {

    // Define pi with 20 digits
    std::ostringstream oss;
    oss << std::setprecision(20) << std::numbers::pi;
    auto pi_20 = oss.str(); 


    flash::configParameters::MatrixParameter<int, 2, 3> myIntMatrixParam("int");
    myIntMatrixParam.load(
        YAML::Load(
            R"(
                int: 
                    - [3, 5, 6]
                    - [2, 8, 9]
            )"
        )
    );
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> referenceMatrix(2, 3);
    referenceMatrix 
        << 3, 5, 6,
           2, 8, 9;
    REQUIRE(myIntMatrixParam == referenceMatrix);
    REQUIRE_THROWS_AS(myIntMatrixParam.load(YAML::Load("ont: 3")), std::runtime_error);
    REQUIRE_THROWS_AS(myIntMatrixParam.load(
        YAML::Load(
            R"(
                int: 
                    - [3, 5, 6]    
            )"
        )
    ), std::runtime_error);
    REQUIRE_THROWS_AS(myIntMatrixParam.load(
        YAML::Load(
            R"(
                int: 
                    - [3, 5]
                    - [4, 6]    
            )"
        )
    ), std::runtime_error);


    referenceMatrix.resize(4, 3);
    referenceMatrix 
        << 2, 4, 5,
           1, 2, 0,
           2, 4, 3,
           1, 3, 6;
    Eigen::Matrix<int, 4, 3> defaultMatrix;
    defaultMatrix 
        << 3, 5, 6,
           2, 8, 9,
           0, 2, 1,
           2, 3, 7;
           
    flash::configParameters::MatrixParameter<int, 4, 3> myDefaultMatrixParam("int", defaultMatrix);
    myDefaultMatrixParam.load(
        YAML::Load(
            R"(
                int: 
                    - [2, 4, 5]
                    - [1, 2, 0]
                    - [2, 4, 3]
                    - [1, 3, 6]
            )"
        )
    );
    REQUIRE(myDefaultMatrixParam == referenceMatrix);
    myDefaultMatrixParam.load(YAML::Load("ont: 3"));
    REQUIRE(myDefaultMatrixParam == defaultMatrix); // set value to defaultMatrix

}
