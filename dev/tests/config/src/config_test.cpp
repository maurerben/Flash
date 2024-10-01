#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cstdint>
#include <config/config.h>

using namespace config;



TEST_CASE("Test Config constructor from YAML::node", "[config]"){

    // Reference cofiguration
    Config reference(
        Seed::fixed, typ::SizeTriple{2, 3, 5}, typ::SizeTriple{5, 6, 7}, 
        20, 30, 1e-5, 4, 1e-6, 2, 3, 4, 5
    );
  
    // Define reference configuration
    YAML::Node yamlNode;
    yamlNode["seed"] = "fixed";
    yamlNode["rSampling"] = typ::sizeTripleToNode(typ::SizeTriple{2, 3, 5});
    yamlNode["kSampling"] = typ::sizeTripleToNode(typ::SizeTriple{5, 6, 7});
    yamlNode["nOccupied"] = 20;
    yamlNode["nUnoccupied"] = 30;
    yamlNode["cISDF"] = 1e-5;
    yamlNode["cvtMaximumSteps"] = 4;
    yamlNode["cvtConvergenceTolerance"] = 1e-6;
    yamlNode["nISDFOccupiedUnoccupied"] = 2;
    yamlNode["nISDFOccupied"] = 3;
    yamlNode["nISDFUnoccupied"] = 4;
    yamlNode["nLanczosIterations"] = 5;
    Config test = Config(yamlNode);
  
    bool test_result = (test == reference);
  
    REQUIRE(
      test_result
    );

    // Create YAML::Node from reference 
    yamlNode = reference.toYamlNode();

    // Read test from yamlNode
    test = Config(yamlNode);
  
    test_result = (test == reference);
  
    REQUIRE(
      test_result
    );

}
