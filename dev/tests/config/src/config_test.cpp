#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cstdint>
#include <config/config.h>

using namespace flash;

TEST_CASE("Test Config construction from a valid YAML::node that defines all parameters.", "[valid-config]"){
    // Define reference configuration
    YAML::Node validNode;
    validNode["n_isdf_vexc"] = 2;
    validNode["n_isdf_wscr_occupied"] = 3;
    validNode["n_isdf_wscr_unoccupied"] = 4;
    validNode["max_lanczos_iterations"] = 20;
    validNode["omega_range"].push_back(1.0);
    validNode["omega_range"].push_back(2.0);
    validNode["n_omega"] = 1000;
    validNode["max_cvt_iterations"] = 333;
    validNode["cvt_convergence_criterium"] = 1e-6;
    validNode["seed_source"] = "fixed";

    // Initialize Config instance from yaml node
    const Config params(validNode);

    // Verify that the attributes are correct
    REQUIRE(params.n_isdf_vexc == 2);
    REQUIRE(params.n_isdf_wscr_occupied == 3);
    REQUIRE(params.n_isdf_wscr_unoccupied == 4);
    REQUIRE(params.max_lanczos_iterations == 20);
    REQUIRE(params.omega_range == std::array<real, 2>{1.0, 2.0});
    REQUIRE(params.n_omega == 1000);
    REQUIRE(params.max_cvt_iterations == 333);
    REQUIRE(params.cvt_convergence_criterium == 1e-6);
    REQUIRE(params.seed_source == Seed::fixed);
}

TEST_CASE("Test Config construction from a valid YAML::node that defines not all parameters.", "[valid-config-default]"){
    // Define reference configuration
    YAML::Node validNodeDefaults;
    validNodeDefaults["n_isdf_vexc"] = 2;
    validNodeDefaults["n_isdf_wscr_occupied"] = 3;
    validNodeDefaults["n_isdf_wscr_unoccupied"] = 4;
    validNodeDefaults["max_lanczos_iterations"] = 20;
    validNodeDefaults["omega_range"].push_back(1.0);
    validNodeDefaults["omega_range"].push_back(2.0);
    validNodeDefaults["n_omega"] = 1000;

    // Initialize Config instance from yaml node
    const Config params(validNodeDefaults);

    // Verify that the attributes are correct
    REQUIRE(params.n_isdf_vexc == 2);
    REQUIRE(params.n_isdf_wscr_occupied == 3);
    REQUIRE(params.n_isdf_wscr_unoccupied == 4);
    REQUIRE(params.max_lanczos_iterations == 20);
    REQUIRE(params.omega_range == std::array<real, 2>{1.0, 2.0});
    REQUIRE(params.n_omega == 1000);
    REQUIRE(params.max_cvt_iterations == 1000);
    REQUIRE(params.cvt_convergence_criterium == 1e-5);
    REQUIRE(params.seed_source == Seed::clock);
}

TEST_CASE("Test Config construction from an invalid YAML::node does not define all mandatory parameters.", "[invalid-config-missing-param]"){
    // Define reference configuration
    YAML::Node invalidNodeMissingKey;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    // Verify that the correct exception is thrown
    auto bla = Config(invalidNodeMissingKey);
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

     // Define reference configuration
    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    // Verify that the correct exception is thrown
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

     // Define reference configuration
    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    // Verify that the correct exception is thrown
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

     // Define reference configuration
    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    // Verify that the correct exception is thrown
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

     // Define reference configuration
    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["n_omega"] = 1000;
    // Verify that the correct exception is thrown
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

     // Define reference configuration
    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    // Verify that the correct exception is thrown
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);
}
