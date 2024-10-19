#define CATCH_CONFIG_MAIN
#include <config/parameters/Option.h>
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

auto configFileWithOptions = YAML::Load(
    R"(
    greeting: hello
    noGreeting: bye
    list: [1, 2, 3]
    )");

fcp::options_t greetings{"hello", "hi"};

TEST_CASE("Test flash::config::parameters::Option class for string type with exception checking") {
    // Option parameter initialized from config file
    fcp::Option hello("greeting", greetings);
    hello.load(configFileWithOptions);
    REQUIRE(hello == configFileWithOptions["greeting"].as<std::string>());

    // Option parameter with a default value, loaded from config file
    fcp::Option helloDefault("greeting", greetings, "hi");
    helloDefault.load(configFileWithOptions);
    REQUIRE(helloDefault == configFileWithOptions["greeting"].as<std::string>());

    // Option parameter with missing key and a default value
    fcp::Option missingKey("insults", greetings, "hi");
    missingKey.load(configFileWithOptions);
    REQUIRE(missingKey == "hi");

    // Option parameter trys to initialize with default value that is not in options throws std::runtime_error
    REQUIRE_THROWS_AS(fcp::Option("greeting", greetings, "Bonjour"), std::runtime_error);

    // Option parameter trys to load a value that is not an option throws std::runtime_error
    fcp::Option valueIsNotAnOption("noGreeting", greetings);
    REQUIRE_THROWS_AS(valueIsNotAnOption.load(configFileWithOptions), std::runtime_error);

    // Option parameter trys to load from key with a list as value throws std::runtime_error
    fcp::Option valueIsList("list", greetings);
    REQUIRE_THROWS_AS(valueIsList.load(configFileWithOptions), std::runtime_error);
}
