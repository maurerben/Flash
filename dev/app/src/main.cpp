#include <constants/constants.h>
#include <yaml-cpp/yaml.h>

#include <data_processing/config.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace flash;

int main() {
    // hi
    //  Construct YAML::Node from reference data
    YAML::Node configYaml = YAML::LoadFile("config.yaml");

    // Parse test configuration from YAML::Node
    Config config(configYaml);

    YAML::Node invalidNodeMissingKey;
    invalidNodeMissingKey["n_isdf_wscr_occupied"] = 3;
    invalidNodeMissingKey["n_isdf_wscr_unoccupied"] = 4;
    invalidNodeMissingKey["max_lanczos_iterations"] = 20;
    invalidNodeMissingKey["omega_range"].push_back(1.0);
    invalidNodeMissingKey["omega_range"].push_back(2.0);
    invalidNodeMissingKey["n_omega"] = 1000;
    // Verify that the correct exception is thrown
    auto bla = Config(invalidNodeMissingKey);
}
