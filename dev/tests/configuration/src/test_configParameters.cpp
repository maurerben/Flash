#define CATCH_CONFIG_MAIN
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip> 
#include <complex>
#include <numbers>
#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include <catch2/catch_all.hpp>
#include <configParameters/ScalarParameter.h>



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
    REQUIRE(myDefaultParam == 5); // set value to default
    

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


