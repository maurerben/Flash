//
// Created by benedikt on 29.03.24.
//

#include "config/config.h"



using namespace config;

grid readGridFromNode(YAML::Node node){
    return grid{node[0].as<std::uint32_t>(), node[1].as<std::uint32_t>(), node[2].as<std::uint32_t>()};
}

AppConfig::AppConfig(){
  RGrid = std::make_tuple(0, 0, 0);
}

AppConfig::AppConfig(grid RGrid){
  RGrid = RGrid;
}

AppConfig::AppConfig(YAML::Node config_yaml){

    AppConfig::RGrid = readGridFromNode(config_yaml[RGrid_yamlKey]);

}

