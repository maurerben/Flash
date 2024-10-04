#define CATCH_CONFIG_MAIN

#include <data_processing/config.h>

#include <catch2/catch.hpp>
#include <cstdint>

using namespace flash;

TEST_CASE("Test Config construction from a valid YAML::node that defines all parameters.", "[valid-config]") {
    // Define reference configuration
    YAML::Node validNode;
    validNode["n_k_points"] = 50;
    validNode["n_occupied_total"] = 13;
    validNode["n_unoccupied_total"] = 37;
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
    const Config params(validNode);
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

TEST_CASE("Test Config construction from a valid YAML::node that defines not all parameters.",
          "[valid-config-default]") {
    // Define reference configuration
    YAML::Node validNodeDefaults;
    validNodeDefaults["n_k_points"] = 50;
    validNodeDefaults["n_occupied_total"] = 13;
    validNodeDefaults["n_unoccupied_total"] = 37;
    validNodeDefaults["n_isdf_vexc"] = 2;
    validNodeDefaults["n_isdf_wscr_occupied"] = 3;
    validNodeDefaults["n_isdf_wscr_unoccupied"] = 4;
    validNodeDefaults["max_lanczos_iterations"] = 20;
    validNodeDefaults["omega_range"].push_back(1.0);
    validNodeDefaults["omega_range"].push_back(2.0);
    validNodeDefaults["n_omega"] = 1000;
    const Config params(validNodeDefaults);
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

TEST_CASE("Test Config construction from an invalid YAML::node that uses a forbidden key.",
          "[invalid-config-missing-param]") {
    YAML::Node invalidNodeInvalidKey;
    invalidNodeInvalidKey["n_k_points"] = 50;
    invalidNodeInvalidKey["n_occupied_total"] = 13;
    invalidNodeInvalidKey["n_unoccupied_total"] = 37;
    invalidNodeInvalidKey["n_isdf_vexc"] = 2;
    invalidNodeInvalidKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeInvalidKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeInvalidKey["max_lanczos_iterations"] = 20;
    invalidNodeInvalidKey["omega_range"].push_back(1.0);
    invalidNodeInvalidKey["omega_range"].push_back(2.0);
    invalidNodeInvalidKey["n_omega"] = 1000;
    invalidNodeInvalidKey["naughty"] = "no";
    REQUIRE_THROWS_AS(Config(invalidNodeInvalidKey), InvalidKeyException);
}

TEST_CASE("Test Config construction from an invalid YAML::node that does not define all mandatory parameters.",
          "[invalid-config-missing-param]") {
    YAML::Node invalidNodeMissingKey;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);

    invalidNodeMissingKey.reset();
    invalidNodeMissingKey["n_k_points"] = 50;
    invalidNodeMissingKey["n_occupied_total"] = 13;
    invalidNodeMissingKey["n_unoccupied_total"] = 37;
    invalidNodeMissingKey["n_isdf_vexc"] = 2;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    REQUIRE_THROWS_AS(Config(invalidNodeMissingKey), MissingKeyException);
}

TEST_CASE("Test Config construction from an invalid YAML::node that defines a parameter wrongly.", "[invalid-config]") {
    YAML::Node invalidNodeInvalidConfiguration;
    invalidNodeInvalidConfiguration["n_isdf_vexc"] = "asf";
    invalidNodeInvalidConfiguration["n_isdf_wscr_occupied"] = 3;
    invalidNodeInvalidConfiguration["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeInvalidConfiguration["max_lanczos_iterations"] = 20;
    invalidNodeInvalidConfiguration["omega_range"].push_back(1.0);
    invalidNodeInvalidConfiguration["omega_range"].push_back(2.0);
    invalidNodeInvalidConfiguration["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeInvalidConfiguration), InvalidConfigurationException);

    invalidNodeInvalidConfiguration.reset();
    invalidNodeInvalidConfiguration["n_isdf_vexc"] = 1;
    invalidNodeInvalidConfiguration["n_isdf_wscr_occupied"] = 3;
    invalidNodeInvalidConfiguration["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeInvalidConfiguration["max_lanczos_iterations"] = "asd";
    invalidNodeInvalidConfiguration["omega_range"].push_back(1.0);
    invalidNodeInvalidConfiguration["omega_range"].push_back("2.0");
    invalidNodeInvalidConfiguration["n_omega"] = 1000;
    REQUIRE_THROWS_AS(Config(invalidNodeInvalidConfiguration), InvalidConfigurationException);
}
