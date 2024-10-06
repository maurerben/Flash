#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>

#include <data_processing/config.h>
#include <types/types.h>

namespace tp = flash::types;
using namespace flash::data_processing::config;

/**
 * @brief string of a valid config.yaml that defines all parameters.
 */
std::string config_yaml = R"(
    n_k_points: 50
    n_occupied_total: 13
    n_unoccupied_total: 37
    n_isdf_vexc: 3
    n_isdf_wscr_occupied: 4
    n_isdf_wscr_unoccupied: 5
    max_lanczos_iterations: 200
    omega_range: [1.0, 2.0]
    n_omega: 1500
    max_cvt_iterations: 333
    cvt_convergence_criterium: 1.e-6
    seed_source: fixed
)";

/**
 * @brief string of a valid config.yaml that defines only mandatory parameters.
 */
std::string default_config_yaml = R"(
    n_k_points: 50
    n_occupied_total: 13
    n_unoccupied_total: 37
    n_isdf_vexc: 3
    n_isdf_wscr_occupied: 4
    n_isdf_wscr_unoccupied: 5
    max_lanczos_iterations: 200
    omega_range: [1.0, 2.0]
    n_omega: 1500
)";

TEST_CASE("Test Config construction from a valid YAML::node that defines all parameters.", "[valid-config]") {
    Config params(YAML::Load(config_yaml));
    REQUIRE(params.n_k_points == 50);
    REQUIRE(params.n_occupied_total == 13);
    REQUIRE(params.n_unoccupied_total == 37);
    REQUIRE(params.n_isdf_vexc == 3);
    REQUIRE(params.n_isdf_wscr_occupied == 4);
    REQUIRE(params.n_isdf_wscr_unoccupied == 5);
    REQUIRE(params.max_lanczos_iterations == 200);
    REQUIRE(params.omega_range == std::array<tp::my_double, 2>{1.0, 2.0});
    REQUIRE(params.n_omega == 1500);
    REQUIRE(params.max_cvt_iterations == 333);
    REQUIRE(params.cvt_convergence_criterium == 1e-6);
    REQUIRE(params.seed_source == Seed::fixed);
}

TEST_CASE("Test Config construction from a valid YAML::node that defines not all parameters.",
          "[valid-config-default]") {
    Config params(YAML::Load(default_config_yaml));
    REQUIRE(params.n_k_points == 50);
    REQUIRE(params.n_occupied_total == 13);
    REQUIRE(params.n_unoccupied_total == 37);
    REQUIRE(params.n_isdf_vexc == 3);
    REQUIRE(params.n_isdf_wscr_occupied == 4);
    REQUIRE(params.n_isdf_wscr_unoccupied == 5);
    REQUIRE(params.max_lanczos_iterations == 200);
    REQUIRE(params.omega_range == std::array<tp::my_double, 2>{1.0, 2.0});
    REQUIRE(params.n_omega == 1500);
    REQUIRE(params.max_cvt_iterations == 1000);
    REQUIRE(params.cvt_convergence_criterium == 1e-5);
    REQUIRE(params.seed_source == Seed::clock);
}

TEST_CASE("Test Config construction from an invalid YAML::node that uses a forbidden key.",
          "[invalid-config-missing-param]") {
    auto configNode = YAML::Load(config_yaml);
    configNode["naughty"] = "no";  // assign value to forbidden key "naugty"
    REQUIRE_THROWS_AS(Config(configNode), InvalidKeyException);
}

TEST_CASE("Test Config construction from an invalid YAML::node that does not define all mandatory parameters.",
          "[invalid-config-missing-param]") {
    auto configNode = YAML::Load(config_yaml);
    configNode.remove("n_k_points");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_occupied_total");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_unoccupied_total");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_isdf_vexc");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_unoccupied_total");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_isdf_vexc");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_isdf_wscr_occupied");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_unoccupied_total");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("max_lanczos_iterations");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("omega_range");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);

    configNode = YAML::Load(config_yaml);
    configNode.remove("n_omega");
    REQUIRE_THROWS_AS(Config(configNode), MissingKeyException);
}

TEST_CASE("Test Config construction from an invalid YAML::node that defines a parameter illegally.",
          "[invalid-config]") {
    auto configNode = YAML::Load(config_yaml);
    configNode["n_isdf_vexc"] = "nope";
    REQUIRE_THROWS_AS(Config(configNode), InvalidConfigurationException);

    configNode = YAML::Load(config_yaml);
    configNode["cvt_convergence_criterium"] = "nope";
    REQUIRE_THROWS_AS(Config(configNode), InvalidConfigurationException);

    configNode = YAML::Load(config_yaml);
    configNode["seed_source"] = "nope";
    REQUIRE_THROWS_AS(Config(configNode), InvalidConfigurationException);

    configNode = YAML::Load(config_yaml);
    configNode["omega_range"] = 1.0;
    REQUIRE_THROWS_AS(Config(configNode), InvalidConfigurationException);
}

TEST_CASE("Test Config construction from an invalid YAML::node that defines a parameter wrongly.", "[invalid-config]") {
    // Check n_k_points rule
    auto configNode = YAML::Load(config_yaml);
    configNode["n_k_points"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // Check n_occupied_total rule
    configNode = YAML::Load(config_yaml);
    configNode["n_occupied_total"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // Check n_unoccupied_total rule
    configNode = YAML::Load(config_yaml);
    configNode["n_unoccupied_total"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // Check n_occupied_total + n_unoccupied_total rule
    configNode = YAML::Load(config_yaml);
    configNode["n_occupied_total"] = 13;
    configNode["n_unoccupied_total"] = 38;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // Checkn_isdf_vexc rules
    configNode = YAML::Load(config_yaml);
    configNode["n_isdf_vexc"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);
    configNode["n_isdf_vexc"] = 10;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // n_isdf_wscr_occupied rules
    configNode = YAML::Load(config_yaml);
    configNode["n_isdf_wscr_occupied"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);
    configNode["n_isdf_wscr_occupied"] = 170;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // n_isdf_wscr_unoccupied rules
    configNode = YAML::Load(config_yaml);
    configNode["n_isdf_wscr_unoccupied"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);
    configNode["n_isdf_wscr_unoccupied"] = 1407;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // max_lanczos_iterations rule
    configNode = YAML::Load(config_yaml);
    configNode["max_lanczos_iterations"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // omega_range rules
    configNode = YAML::Load(config_yaml);
    configNode.remove("omega_range");
    configNode["omega_range"].push_back(2.0);
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);
    configNode.remove("omega_range");
    configNode["omega_range"].push_back(2.0);
    configNode["omega_range"].push_back(1.0);
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);
    configNode.remove("omega_range");
    configNode["omega_range"].push_back(1.0);
    configNode["omega_range"].push_back(2.0);
    configNode["omega_range"].push_back(3.0);
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // n_omega rule
    configNode = YAML::Load(config_yaml);
    configNode["n_omega"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // max_cvt_iterations rule
    configNode = YAML::Load(config_yaml);
    configNode["max_cvt_iterations"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);

    // cvt_convergence_criterium rule
    configNode = YAML::Load(config_yaml);
    configNode["cvt_convergence_criterium"] = 0;
    REQUIRE_THROWS_AS(Config(configNode).validate(), InvalidConfigurationException);
}
