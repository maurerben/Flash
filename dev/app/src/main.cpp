#include <iostream>

#include "config/config.h"
#include "constants/constants.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    // Define reference data
    const config::grid RGridRef{1, 2, 3};

    // Define yaml node 
    YAML::Node ConfigNode = YAML::Load(
        "{RGrid: [1, 2, 3]}"
    );

    config::AppConfig TestConfig(ConfigNode);

    std::cout << std::get<0>(TestConfig.RGrid) << std::get<1>(TestConfig.RGrid) <<std::get<2>(TestConfig.RGrid) <<std::endl;
}
