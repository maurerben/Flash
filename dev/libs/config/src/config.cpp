//
// Created by benedikt on 29.03.24.
//

#include "config/config.h"

using namespace flash;

inline std::map<Seed, std::string> seedToString{
        { Seed::fixed, "fixed" },
        { Seed::clock, "clock" }
    };

std::map<std::string, Seed> stringToSeed{
        { "fixed", Seed::fixed },
        { "clock", Seed::clock }
    };

Seed nodeToSeed(const YAML::Node &node, const std::string &key, const Seed &defaultValue, const std::map<std::string, Seed> &stringToSeed = stringToSeed){
    try{
        return stringToSeed.at(node[key].as<std::string>());
    }
    catch(YAML::InvalidNode){
        return defaultValue;
    }
    catch(std::out_of_range){
        //throw InvalidConfigurationException(std::format("{} is not a valid option for seed.", string));
        throw InvalidConfigurationException("Invalid option for seed.");
    }
}

template <typename T> T nodeToValue(const YAML::Node &node, bool (*rule)(T value), std::string msg) {
    auto value = node.as<T>();
    if (!rule(value)){
        throw InvalidConfigurationException(msg);
    }
    return value;
}

template <typename T> T nodeToValueWithDefault(const YAML::Node &node, const std::string &key, const T &defaultValue, bool (*rule)(T value), std::string msg) {
    T value;
    try{
        value = node[key].as<T>();
    }
    catch(YAML::InvalidNode){
        return defaultValue;
    }
    if (!rule(value)){
        throw InvalidConfigurationException(msg);
    }
    return value;
}

template <typename T, size N> auto nodeToArray(const YAML::Node &node, bool (*rule)(T value), std::string msg) {
    std::array<T, N> values;
    try{
        size idx = 0;
        for (auto element : node){
            values[idx] = node.as<T>();
        }
    }
    catch(YAML::InvalidNode){
        throw InvalidConfigurationException(msg);
    }
    if (!rule(values)){
        throw InvalidConfigurationException(msg);
    }
    return values;
}



Config::Config(const YAML::Node &configYaml, const std::vector<std::string> &allowedKeys, const std::vector<std::string> &mandatoryKeys, const DefaultConfig &defaults) {
    
    // extract keys from yaml node
    std::vector<std::string> keysInNode;
    for (auto kv : configYaml){
        auto key = kv.first.as<std::string>();
        std::cout << key << std::endl;
        keysInNode.push_back(key);
    }
    // verify that all keys from keysInNode that are in allowedKeys
    for (auto key : keysInNode){
        if(!std::binary_search(allowedKeys.begin(), allowedKeys.end(), key, [](std::string string1, std::string string2){return string1 == string2;})) {
            throw InvalidKeyException("Invalid key in config.yaml.");
        }
    }
    // verify that keys from mandatoryKeys are in keysInNode
    for (auto key : mandatoryKeys){
        std::cout << key << " " << std::binary_search(keysInNode.begin(), keysInNode.end(), key, [](std::string string1, std::string string2){return string1 == string2;}) << std::endl;
        if(!std::binary_search(keysInNode.begin(), keysInNode.end(), key, [](std::string string1, std::string string2){return string1 == string2;})) {
            throw MissingKeyException("Mandatory key are missing.");
        }
    }
    
    try{
        n_isdf_vexc = nodeToValue<size>(
            configYaml["n_isdf_vexc"], [](size value){return (value > 0);},
            "n_isdf_vexc in config.yml is errornous."
        );
        n_isdf_wscr_occupied = nodeToValue<size>(
            configYaml["n_isdf_wscr_occupied"], [](size value){return (value > 0);},
            "n_isdf_wscr_occupied in config.yml is errornous."
        );
        n_isdf_wscr_unoccupied = nodeToValue<size>(
            configYaml["n_isdf_wscr_unoccupied"], [](size value){return (value > 0);},
            "n_isdf_wscr_unoccupied in config.yml is errornous."
        );
        max_lanczos_iterations = nodeToValue<size>(
            configYaml["max_lanczos_iterations"], [](size value){return (value > 0);},
            "max_lanczos_iterations in config.yml is errornous."
        );
        omega_range = nodeToValue<std::array<real, 2>>(
            configYaml["omega_range"], [](std::array<real, 2> value){return (value[0] < value[1]);},
            "omega_range in config.yml is errornous."
        );
        n_omega = nodeToValue<size>(
            configYaml["n_omega"], [](size value){return (value > 0);},
            "n_omega in config.yml is errornous."
        );
        max_cvt_iterations = nodeToValueWithDefault<size>(
            configYaml, "max_cvt_iterations", defaults.maxCVTItereations, 
            [](size value){return (value > 0);},
            "max_cvt_iterations in config.yml is errornous."
        );
        cvt_convergence_criterium = nodeToValueWithDefault<real>(
            configYaml, "cvt_convergence_criterium", defaults.CVTConvergenceCriterium, 
            [](real value){return (value > 0);},
            "cvt_convergence_criterium in config.yml is errornous."
        );
        seed_source = nodeToSeed(configYaml, "seed_source", defaults.seedSource);
    }
    catch(InvalidConfigurationException ice){
        throw ice;
    }

    
}



// YAML::Node Config::toYamlNode(){
//     YAML::Node node;
//     // rSampling
//     node[keyToName.at(Key::seed)] = seedToString[seed];
//     node[keyToName.at(Key::rSampling)] = typ::sizeTripleToNode(rSampling);
//     node[keyToName.at(Key::kSampling)] = typ::sizeTripleToNode(kSampling);
//     node[keyToName.at(Key::nOccupied)] = nOccupied;
//     node[keyToName.at(Key::nUnoccupied)] = nUnoccupied;
//     node[keyToName.at(Key::cISDF)] = cISDF;
//     node[keyToName.at(Key::cvtMaximumSteps)] = cvtMaximumSteps;
//     node[keyToName.at(Key::cvtConvergenceTolerance)] = cvtConvergenceTolerance;
//     node[keyToName.at(Key::nISDFOccupiedUnoccupied)] = nISDFOccupiedUnoccupied;
//     node[keyToName.at(Key::nISDFOccupied)] = nISDFOccupied;
//     node[keyToName.at(Key::nISDFUnoccupied)] = nISDFUnoccupied;
//     node[keyToName.at(Key::nLanczosIterations)] = nLanczosIterations;
//     return node;
// }


// void Config::print_to_stdout(){
//     std::string seperator = ": ";
//     std::cout<<keyToName.at(Key::seed)<< seperator << seedToString.at(seed) <<  std::endl;
//     std::cout<<keyToName.at(Key::rSampling)<< seperator << typ::sizeTripleToString(rSampling) << std::endl;
//     std::cout<<keyToName.at(Key::kSampling)<< seperator << typ::sizeTripleToString(kSampling) << std::endl;
//     std::cout<<keyToName.at(Key::nOccupied)<< seperator << nOccupied << std::endl;
//     std::cout<<keyToName.at(Key::nUnoccupied)<< seperator << nUnoccupied << std::endl;
//     std::cout<<keyToName.at(Key::cISDF)<< seperator << cISDF << std::endl;
//     std::cout<<keyToName.at(Key::cvtMaximumSteps)<< seperator << cvtMaximumSteps << std::endl;
//     std::cout<<keyToName.at(Key::cvtConvergenceTolerance)<< seperator << cvtConvergenceTolerance << std::endl;
//     std::cout<<keyToName.at(Key::nISDFOccupiedUnoccupied)<< seperator << nISDFOccupiedUnoccupied << std::endl;
//     std::cout<<keyToName.at(Key::nISDFOccupied)<< seperator << nISDFOccupied << std::endl;
//     std::cout<<keyToName.at(Key::nISDFUnoccupied)<< seperator << nISDFUnoccupied << std::endl;
//     std::cout<<keyToName.at(Key::nLanczosIterations)<< seperator << nLanczosIterations << std::endl;
// }

