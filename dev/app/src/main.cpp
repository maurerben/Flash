#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "config/config.h"
#include "constants/constants.h"
#include <yaml-cpp/yaml.h>

int main() {

    // Construct YAML::Node from reference data
    YAML::Node configYaml = YAML::LoadFile("config.yaml");
    
    
    // Parse test configuration from YAML::Node
    config::Config configuration = config::Config(configYaml, config::defaultConfig);


    configuration.print_to_stdout();


}
