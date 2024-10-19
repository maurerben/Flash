#define CATCH_CONFIG_MAIN
#include <config/parameters/Scalar.h>
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

auto configFileWithScalars = YAML::Load(
    R"(
    three: 3
    pi_20: 3.14159265358979323846
    hello: "hello"
    true: true
    True: True
    false: false
    False: False
    one: 1
    zero: 0
    complex: 1+2j
    list: [1, 2, 3]
    )");

TEST_CASE("Test flash::config::parameters::Scalar class for bool type") {
    // Boolean parameter initialized from config file
    fcp::Scalar<bool> isTrue("true");
    isTrue.load(configFileWithScalars);
    REQUIRE(isTrue == configFileWithScalars["true"].as<bool>());

    // Boolean parameter with a default value, loaded from config file
    fcp::Scalar<bool> isFalse("False", true);  // Default value is true
    isFalse.load(configFileWithScalars);
    REQUIRE(isFalse == configFileWithScalars["False"].as<bool>());

    // Boolean parameter with missing key and default value
    fcp::Scalar<bool> missingBool("notAKey", false);
    missingBool.load(configFileWithScalars);
    REQUIRE(missingBool == false);

    // Check that correct values are loaded from 'True' and 'False' strings
    fcp::Scalar<bool> isTrueUpper("True");
    isTrueUpper.load(configFileWithScalars);
    REQUIRE(isTrueUpper == configFileWithScalars["True"].as<bool>());

    fcp::Scalar<bool> isFalseLower("false");
    isFalseLower.load(configFileWithScalars);
    REQUIRE(isFalseLower == configFileWithScalars["false"].as<bool>());

    fcp::Scalar<bool> isFalseUpper("False");
    isFalseUpper.load(configFileWithScalars);
    REQUIRE(isFalseUpper == configFileWithScalars["False"].as<bool>());

    // Boolean parameter with missing key, should throw runtime error
    fcp::Scalar<bool> missingKey("notAKey");
    REQUIRE_THROWS_AS(missingKey.load(configFileWithScalars), std::runtime_error);

    // Boolean parameter with value of wrong type (integer), should throw
    fcp::Scalar<bool> wrongTypeInt("three");
    REQUIRE_THROWS_AS(wrongTypeInt.load(configFileWithScalars), std::runtime_error);

    // Boolean parameter with value of wrong type (string), should throw
    fcp::Scalar<bool> wrongTypeString("hello");
    REQUIRE_THROWS_AS(wrongTypeString.load(configFileWithScalars), std::runtime_error);

    // Boolean parameter with value of wrong type (float), should throw
    fcp::Scalar<bool> wrongTypeFloat("pi_20");
    REQUIRE_THROWS_AS(wrongTypeFloat.load(configFileWithScalars), std::runtime_error);

    // Boolean parameter with value of wrong type (complex), should throw
    fcp::Scalar<bool> wrongTypeComplex("complex");
    REQUIRE_THROWS_AS(wrongTypeComplex.load(configFileWithScalars), std::runtime_error);

    // Boolean parameter tries to load list throws std::runtime_error
    fcp::Scalar<bool> isList("list");
    REQUIRE_THROWS_AS(isList.load(configFileWithScalars), std::runtime_error);
}

TEST_CASE("Test flash::config::parameters::Scalar class initialization for integers") {
    // Load integer parameter from config file with matching key
    fcp::Scalar<int> three("three");
    three.load(configFileWithScalars);
    REQUIRE(three == configFileWithScalars["three"].as<int>());

    // Integer with a default value, loaded from config file
    three = fcp::Scalar<int>("three", 5);
    three.load(configFileWithScalars);
    REQUIRE(three == configFileWithScalars["three"].as<int>());

    // Integer with default value when key is missing
    fcp::Scalar<int> five("notAKey", 5);
    five.load(configFileWithScalars);
    REQUIRE(five == 5);

    // Missing key should throw std::runtime_error
    fcp::Scalar<int> notAkey("notAKey");
    REQUIRE_THROWS_AS(notAkey.load(configFileWithScalars), std::runtime_error);

    // Wrong type should throw std::runtime_error (real number for integer)
    fcp::Scalar<int> pi_20("pi_20");
    REQUIRE_THROWS_AS(pi_20.load(configFileWithScalars), std::runtime_error);

    // Wrong type should throw std::runtime_error (string for integer)
    fcp::Scalar<int> hello("hello");
    REQUIRE_THROWS_AS(hello.load(configFileWithScalars), std::runtime_error);

    // Loading boolean as integer should throw
    fcp::Scalar<int> True("True");
    REQUIRE_THROWS_AS(True.load(configFileWithScalars), std::runtime_error);

    // Loading boolean lowercase as integer should also throw
    True = fcp::Scalar<int>("true");
    REQUIRE_THROWS_AS(True.load(configFileWithScalars), std::runtime_error);

    // Testing with boolean 'false'
    fcp::Scalar<int> False("False");
    REQUIRE_THROWS_AS(False.load(configFileWithScalars), std::runtime_error);

    // Lowercase 'false' should also fail for integer
    False = fcp::Scalar<int>("false");
    REQUIRE_THROWS_AS(False.load(configFileWithScalars), std::runtime_error);

    // Integer parameter tries to load list throws std::runtime_error
    fcp::Scalar<int> isList("list");
    REQUIRE_THROWS_AS(isList.load(configFileWithScalars), std::runtime_error);
}

TEST_CASE("Test flash::config::parameters::Scalar class for float type") {
    // Float parameter initialized from config file
    fcp::Scalar<float> pi("pi_20");
    pi.load(configFileWithScalars);
    REQUIRE(pi == Catch::Approx(configFileWithScalars["pi_20"].as<float>()));

    // Float parameter with default value, loaded from config file
    fcp::Scalar<float> threeDefault("three", 2.71f);  // Default value is 2.71
    threeDefault.load(configFileWithScalars);
    REQUIRE(threeDefault == Catch::Approx(static_cast<float>(configFileWithScalars["three"].as<int>())));

    // Float parameter with missing key, should load the default value
    fcp::Scalar<float> missingFloat("notAKey", 2.71f);
    missingFloat.load(configFileWithScalars);
    REQUIRE(missingFloat == Catch::Approx(2.71f));

    // Float parameter with value of wrong type (integer), should load correctly
    fcp::Scalar<float> fromInt("three");
    fromInt.load(configFileWithScalars);
    REQUIRE(fromInt == Catch::Approx(static_cast<float>(configFileWithScalars["three"].as<int>())));

    // Float parameter with missing key, should throw std::runtime_error
    fcp::Scalar<float> missingKey("notAKey");
    REQUIRE_THROWS_AS(missingKey.load(configFileWithScalars), std::runtime_error);

    // Float parameter with value of wrong type (boolean), should throw std::runtime_error
    fcp::Scalar<float> wrongTypeTrue("true");
    REQUIRE_THROWS_AS(wrongTypeTrue.load(configFileWithScalars), std::runtime_error);

    fcp::Scalar<float> wrongTypeFalse("False");
    REQUIRE_THROWS_AS(wrongTypeFalse.load(configFileWithScalars), std::runtime_error);

    // Float parameter with value of wrong type (string), should throw std::runtime_error
    fcp::Scalar<float> wrongTypeString("hello");
    REQUIRE_THROWS_AS(wrongTypeString.load(configFileWithScalars), std::runtime_error);

    // Float parameter with value of wrong type (complex), should throw std::runtime_error
    fcp::Scalar<float> wrongTypeComplex("complex");
    REQUIRE_THROWS_AS(wrongTypeComplex.load(configFileWithScalars), std::runtime_error);

    // Float parameter tries to load list throws std::runtime_error
    fcp::Scalar<float> isList("list");
    REQUIRE_THROWS_AS(isList.load(configFileWithScalars), std::runtime_error);
}

TEST_CASE("Test flash::config::parameters::Scalar class for double type") {
    // Double parameter initialized from config file
    fcp::Scalar<double> pi("pi_20");
    pi.load(configFileWithScalars);
    REQUIRE(pi == Catch::Approx(configFileWithScalars["pi_20"].as<double>()));

    // Double parameter with default value, loaded from config file
    fcp::Scalar<double> threeDefault("three", 2.71);
    threeDefault.load(configFileWithScalars);
    REQUIRE(threeDefault == Catch::Approx(static_cast<double>(configFileWithScalars["three"].as<int>())));

    // Double parameter with missing key, should load the default value
    fcp::Scalar<double> missingDouble("notAKey", 2.71);
    missingDouble.load(configFileWithScalars);
    REQUIRE(missingDouble == Catch::Approx(2.71));

    // Double parameter with value of wrong type (integer), should load correctly
    fcp::Scalar<double> fromInt("three");
    fromInt.load(configFileWithScalars);
    REQUIRE(fromInt == Catch::Approx(static_cast<double>(configFileWithScalars["three"].as<int>())));

    // Double parameter with missing key, should throw std::runtime_error
    fcp::Scalar<double> missingKey("notAKey");
    REQUIRE_THROWS_AS(missingKey.load(configFileWithScalars), std::runtime_error);

    // Double parameter with value of wrong type (boolean), should throw std::runtime_error
    fcp::Scalar<double> wrongTypeTrue("true");
    REQUIRE_THROWS_AS(wrongTypeTrue.load(configFileWithScalars), std::runtime_error);

    fcp::Scalar<double> wrongTypeFalse("False");
    REQUIRE_THROWS_AS(wrongTypeFalse.load(configFileWithScalars), std::runtime_error);

    // Double parameter with value of wrong type (string), should throw std::runtime_error
    fcp::Scalar<double> wrongTypeString("hello");
    REQUIRE_THROWS_AS(wrongTypeString.load(configFileWithScalars), std::runtime_error);

    // Double parameter with value of wrong type (complex), should throw std::runtime_error
    fcp::Scalar<double> wrongTypeComplex("complex");
    REQUIRE_THROWS_AS(wrongTypeComplex.load(configFileWithScalars), std::runtime_error);

    // Double parameter tries to load list throws std::runtime_error
    fcp::Scalar<double> isList("list");
    REQUIRE_THROWS_AS(isList.load(configFileWithScalars), std::runtime_error);
}