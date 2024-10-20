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

auto configFile = YAML::Load(
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

TEST_CASE("Test flash::config::parameters::Scalar class initialization for integers") {
    // Load integer parameter from config file with matching key
    fcp::Scalar<int> three("three");
    three.load(configFile);
    REQUIRE(three == configFile["three"].as<int>());

    // Integer with a default value, loaded from config file
    three = fcp::Scalar<int>("three", 5);
    three.load(configFile);
    REQUIRE(three == configFile["three"].as<int>());

    // Integer with default value when key is missing
    fcp::Scalar<int> five("notAKey", 5);
    five.load(configFile);
    REQUIRE(five == 5);

    // Missing key should throw std::runtime_error
    fcp::Scalar<int> notAkey("notAKey");
    REQUIRE_THROWS_AS(notAkey.load(configFile), std::runtime_error);

    // Wrong type should throw std::runtime_error (real number for integer)
    fcp::Scalar<int> pi_20("pi_20");
    REQUIRE_THROWS_AS(pi_20.load(configFile), std::runtime_error);

    // Wrong type should throw std::runtime_error (string for integer)
    fcp::Scalar<int> hello("hello");
    REQUIRE_THROWS_AS(hello.load(configFile), std::runtime_error);

    // Loading boolean as integer should throw
    fcp::Scalar<int> True("True");
    REQUIRE_THROWS_AS(True.load(configFile), std::runtime_error);

    // Loading boolean lowercase as integer should also throw
    True = fcp::Scalar<int>("true");
    REQUIRE_THROWS_AS(True.load(configFile), std::runtime_error);

    // Testing with boolean 'false'
    fcp::Scalar<int> False("False");
    REQUIRE_THROWS_AS(False.load(configFile), std::runtime_error);

    // Lowercase 'false' should also fail for integer
    False = fcp::Scalar<int>("false");
    REQUIRE_THROWS_AS(False.load(configFile), std::runtime_error);

    // Integer parameter tries to load list throws std::runtime_error
    fcp::Scalar<int> isList("list");
    REQUIRE_THROWS_AS(isList.load(configFile), std::runtime_error);
}

TEST_CASE("Test flash::config::parameters::Scalar class for float type") {
    // Float parameter initialized from config file
    fcp::Scalar<float> pi("pi_20");
    pi.load(configFile);
    REQUIRE(pi == Catch::Approx(configFile["pi_20"].as<float>()));

    // Float parameter with default value, loaded from config file
    fcp::Scalar<float> threeDefault("three", 2.71f);  // Default value is 2.71
    threeDefault.load(configFile);
    REQUIRE(threeDefault == Catch::Approx(static_cast<float>(configFile["three"].as<int>())));

    // Float parameter with missing key, should load the default value
    fcp::Scalar<float> missingFloat("notAKey", 2.71f);
    missingFloat.load(configFile);
    REQUIRE(missingFloat == Catch::Approx(2.71f));

    // Float parameter with value of wrong type (integer), should load correctly
    fcp::Scalar<float> fromInt("three");
    fromInt.load(configFile);
    REQUIRE(fromInt == Catch::Approx(static_cast<float>(configFile["three"].as<int>())));

    // Float parameter with missing key, should throw std::runtime_error
    fcp::Scalar<float> missingKey("notAKey");
    REQUIRE_THROWS_AS(missingKey.load(configFile), std::runtime_error);

    // Float parameter with value of wrong type (boolean), should throw std::runtime_error
    fcp::Scalar<float> wrongTypeTrue("true");
    REQUIRE_THROWS_AS(wrongTypeTrue.load(configFile), std::runtime_error);

    fcp::Scalar<float> wrongTypeFalse("False");
    REQUIRE_THROWS_AS(wrongTypeFalse.load(configFile), std::runtime_error);

    // Float parameter with value of wrong type (string), should throw std::runtime_error
    fcp::Scalar<float> wrongTypeString("hello");
    REQUIRE_THROWS_AS(wrongTypeString.load(configFile), std::runtime_error);

    // Float parameter with value of wrong type (complex), should throw std::runtime_error
    fcp::Scalar<float> wrongTypeComplex("complex");
    REQUIRE_THROWS_AS(wrongTypeComplex.load(configFile), std::runtime_error);

    // Float parameter tries to load list throws std::runtime_error
    fcp::Scalar<float> isList("list");
    REQUIRE_THROWS_AS(isList.load(configFile), std::runtime_error);
}

TEST_CASE("Test flash::config::parameters::Scalar class for double type") {
    // Double parameter initialized from config file
    fcp::Scalar<double> pi("pi_20");
    pi.load(configFile);
    REQUIRE(pi == Catch::Approx(configFile["pi_20"].as<double>()));

    // Double parameter with default value, loaded from config file
    fcp::Scalar<double> threeDefault("three", 2.71);
    threeDefault.load(configFile);
    REQUIRE(threeDefault == Catch::Approx(static_cast<double>(configFile["three"].as<int>())));

    // Double parameter with missing key, should load the default value
    fcp::Scalar<double> missingDouble("notAKey", 2.71);
    missingDouble.load(configFile);
    REQUIRE(missingDouble == Catch::Approx(2.71));

    // Double parameter with value of wrong type (integer), should load correctly
    fcp::Scalar<double> fromInt("three");
    fromInt.load(configFile);
    REQUIRE(fromInt == Catch::Approx(static_cast<double>(configFile["three"].as<int>())));

    // Double parameter with missing key, should throw std::runtime_error
    fcp::Scalar<double> missingKey("notAKey");
    REQUIRE_THROWS_AS(missingKey.load(configFile), std::runtime_error);

    // Double parameter with value of wrong type (boolean), should throw std::runtime_error
    fcp::Scalar<double> wrongTypeTrue("true");
    REQUIRE_THROWS_AS(wrongTypeTrue.load(configFile), std::runtime_error);

    fcp::Scalar<double> wrongTypeFalse("False");
    REQUIRE_THROWS_AS(wrongTypeFalse.load(configFile), std::runtime_error);

    // Double parameter with value of wrong type (string), should throw std::runtime_error
    fcp::Scalar<double> wrongTypeString("hello");
    REQUIRE_THROWS_AS(wrongTypeString.load(configFile), std::runtime_error);

    // Double parameter with value of wrong type (complex), should throw std::runtime_error
    fcp::Scalar<double> wrongTypeComplex("complex");
    REQUIRE_THROWS_AS(wrongTypeComplex.load(configFile), std::runtime_error);

    // Double parameter tries to load list throws std::runtime_error
    fcp::Scalar<double> isList("list");
    REQUIRE_THROWS_AS(isList.load(configFile), std::runtime_error);
}