//
// Created by benedikt on 29.03.24.
//

#ifndef LIGHTNING_CONFIG_H
#define LIGHTNING_CONFIG_H

#include <iostream>

#include <tuple>
#include <cstdint>
#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace config{


  typedef std::tuple<std::uint32_t, 
                     std::uint32_t, 
                     std::uint32_t> grid;


  inline const std::string RGrid_yamlKey = "RGrid";

  class AppConfig {

    public:
      /** 
       * Number of real space points per dimension for the 
       * wave function sampling.
       */
      grid RGrid;

      /**
       * Standard constructor.
       */
      AppConfig();

      /**
       * Standard constructor.
       */
      AppConfig(grid RGrid);

      /**
       * Load the configuration from YAML node
       */
      AppConfig(YAML::Node yamlConfigFile);
     

  };

  
}





#endif //LIGHTNING_CONFIG_H
