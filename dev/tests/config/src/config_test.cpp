#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "config/config.h"

TEST_CASE("My class returns true", "[config]"){
  config::AppConfig MyConfig;
  REQUIRE(false);
}