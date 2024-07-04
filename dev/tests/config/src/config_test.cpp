#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <iostream>
#include <cstdint>
#include <tuple>

#include <config/config.h>

TEST_CASE("Test AppConfig constructor from YAML::node", "[config]"){
  
  // Define reference data
  const config::grid RGridRef{1, 2, 3};

  YAML::Node ConfigNode = YAML::Load(
    "{RGrid: [1, 2, 3]}"
  );

  config::AppConfig TestConfig(ConfigNode);

  bool test_result = TestConfig.RGrid == RGridRef;

  REQUIRE(
    test_result
  );

  
  REQUIRE(true);
}