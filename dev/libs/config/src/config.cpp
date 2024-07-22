//
// Created by benedikt on 29.03.24.
//

#include "config/config.h"

using namespace config;

std::map<Key, std::string> keyToName{
    { Key::seed, "seed" },
    { Key::rSampling, "rSampling" },
    { Key::kSampling, "kSampling" }, 
    { Key::nOccupied, "nOccupied" },
    { Key::nUnoccupied, "nUnoccupied" },
    { Key::cISDF, "cISDF" },
    { Key::cvtMaximumSteps, "cvtMaximumSteps" },
    { Key::cvtConvergenceTolerance, "cvtConvergenceTolerance" },
    { Key::nISDFOccupiedUnoccupied, "nISDFOccupiedUnoccupied" },
    { Key::nISDFOccupied, "nISDFOccupied" },
    { Key::nISDFUnoccupied, "nISDFUnoccupied" },
    { Key::nLanczosIterations, "nLanczosIterations" }
};

inline std::map<Seed, std::string> seedToString{
        { Seed::fixed, "fixed" },
        { Seed::time, "time" }
    };

std::map<std::string, Seed> stringSeedMap{
        { "fixed", Seed::fixed },
        { "time", Seed::time }
    };

Seed nodeToSeed(const YAML::Node &node, const std::map<std::string, Seed> stringSeedMap){

    try{
        return stringSeedMap.at(node[keyToName.at(Key::seed)].as<std::string>());
    }
    catch(std::out_of_range){
        //throw InvalidConfigurationException(std::format("{} is not a valid option for seed.", string));
        throw InvalidConfigurationException("Invalid option for seed.");
    }
}

typ::SizeTriple nodeToSampling(const YAML::Node &node, const Key &key, const typ::SizeTriple &defaultValue){

    typ::SizeTriple value;
    YAML::Node thisNode;
    try{
        thisNode = node[keyToName.at(key)];

        try{
            value = typ::nodeToSizeTriple(thisNode);
        }

        catch(...){
            throw InvalidConfigurationException("Sampling is invalid. It must be exactly three integer values larger than zero.");
        }
    }

    catch(std::out_of_range){
        value = defaultValue;
    }

    if(value[0]<1 && value[1]<1 && value[2]<1){
        throw InvalidConfigurationException("Sampling is invalid. All values must be larger than zero.");
    }

    return value;
}

template <typename T> T nodeToValue(const YAML::Node &node, const Key &key, const T &defaultValue, bool (*rule)(T value), std::string msg) {
    
    T value;
    try{
        value = node[keyToName.at(key)].as<T>();
    }

    catch(YAML::InvalidNode){
        value = defaultValue;
    }

    if (!rule(value)){
        throw InvalidConfigurationException(msg);
    }

    return value;

}


void verifyConfigYaml(const YAML::Node configYaml) {

    std::vector<std::string> nameList;

    for (auto [key, name] : keyToName) {
        nameList.push_back(name);
    }

    for (auto kv : configYaml){
        auto key = kv.first.as<std::string>();

        if(!std::binary_search(nameList.begin(), nameList.end(), key, [](std::string string1, std::string string2){return string1 == string2;})) {
            throw InvalidKeyException("Invalid key in config.yaml");
        }
    
    }

}


Config::Config() {}

Config::Config(
    const Seed &seed_in,
    const typ::SizeTriple &rSampling_in,
    const typ::SizeTriple &kSampling_in,
    const std::size_t &nOccupied_in,
    const std::size_t &nUnoccupied_in,
    const std::float_t &cISDF_in,
    const std::size_t &cvtMaximumSteps_in,
    const std::float_t &cvtConvergenceTolerance_in,
    const std::size_t &nISDFOccupiedUnoccupied_in,
    const std::size_t &nISDFOccupied_in,
    const std::size_t &nISDFUnoccupied_in,
    const std::size_t &nLanczosIterations_in
){
    seed = seed_in;
    rSampling = rSampling_in;
    kSampling = kSampling_in;
    nOccupied = nOccupied_in;
    nUnoccupied = nUnoccupied_in;
    cISDF = cISDF_in;
    cvtMaximumSteps = cvtMaximumSteps_in;
    cvtConvergenceTolerance = cvtConvergenceTolerance_in;
    nISDFOccupiedUnoccupied = nISDFOccupiedUnoccupied_in;
    nISDFOccupied = nISDFOccupied_in;
    nISDFUnoccupied = nISDFUnoccupied_in;
    nLanczosIterations = nLanczosIterations_in;
}


Config::Config(const YAML::Node &configYaml, const Config &defaultConfig){

    try {
        verifyConfigYaml(configYaml);
    }
    catch(InvalidKeyException ike) {
        std::cout << "Error in configuration:" << std::endl;
        std::cout << "    " << ike.what() << std::endl; 
        throw std::exception();
    }
    
    try{
        seed = nodeToSeed(configYaml, stringSeedMap);

        rSampling = nodeToSampling(configYaml, Key::rSampling, defaultConfig.rSampling);

        kSampling = nodeToSampling(configYaml, Key::kSampling, defaultConfig.kSampling);

        nOccupied = nodeToValue<std::size_t>(configYaml, Key::nOccupied, defaultConfig.nOccupied, 
            [](std::size_t value){return (value > 0);}, "nOccupied must be larger than zero.");

        nUnoccupied = nodeToValue<std::size_t>(configYaml, Key::nUnoccupied, defaultConfig.nUnoccupied, 
            [](std::size_t value){return (value > 0);}, "nUnoccupied must be larger than zero.");

        cISDF = nodeToValue<std::float_t>(configYaml, Key::cISDF, defaultConfig.cISDF, 
            [](std::float_t value){return (value >= 0.0);}, "cISDF must not be smaller than zero.");

        cvtMaximumSteps = nodeToValue<std::size_t>(configYaml, Key::cvtMaximumSteps, defaultConfig.cvtMaximumSteps, 
            [](std::size_t value){return (value>0);}, "cvtMaximumSteps must be larger than zero.");

        cvtConvergenceTolerance = nodeToValue<std::float_t>(configYaml, Key::cvtConvergenceTolerance, defaultConfig.cvtConvergenceTolerance, 
            [](std::float_t value){return (value > 0.0);}, "cvtConvergenceTolerance must be larger than zero.");

        nISDFOccupiedUnoccupied = nodeToValue<std::size_t>(configYaml, Key::nISDFOccupiedUnoccupied, defaultConfig.nISDFOccupiedUnoccupied, 
            [](std::size_t value){return (value>=0);}, "nISDFOccupiedUnoccupied must not be smaller than zero.");
        
        nISDFOccupied = nodeToValue<std::size_t>(configYaml, Key::nISDFOccupied, defaultConfig.nISDFOccupied, 
            [](std::size_t value){return (value>=0);}, "nISDFOccupied must not be smaller than zero.");

        nISDFUnoccupied = nodeToValue<std::size_t>(configYaml, Key::nISDFUnoccupied, defaultConfig.nISDFUnoccupied, 
            [](std::size_t value){return (value>=0);}, "nISDFUnoccupied must not be smaller than zero.");

        nLanczosIterations = nodeToValue<std::size_t>(configYaml, Key::nLanczosIterations, defaultConfig.nLanczosIterations, 
            [](std::size_t value){return (value>0);}, "nLanczosIterations must be larger than zero.");
    }

    catch(InvalidConfigurationException ice){
        std::cout << "Error in configuration:" << std::endl;
        std::cout << "    " << ice.what() << std::endl;
        throw std::exception();
    }

    
}

bool Config::operator==(const Config &rhs){

    if( 
        seed == rhs.seed
        && rSampling == rhs.rSampling
        && kSampling == rhs.kSampling
        && nOccupied == rhs.nOccupied
        && nUnoccupied == rhs.nUnoccupied
        && cISDF == rhs.cISDF
        && cvtMaximumSteps == cvtMaximumSteps
        && cvtConvergenceTolerance == cvtConvergenceTolerance
        && nISDFOccupiedUnoccupied == nISDFOccupiedUnoccupied
        && nISDFOccupied == nISDFOccupied
        && nISDFUnoccupied == nISDFUnoccupied
        && nLanczosIterations == nLanczosIterations
    ) return true;

    return false;
}


YAML::Node Config::toYamlNode(){
    YAML::Node node;
    // rSampling
    node[keyToName.at(Key::seed)] = seedToString[seed];
    node[keyToName.at(Key::rSampling)] = typ::sizeTripleToNode(rSampling);
    node[keyToName.at(Key::kSampling)] = typ::sizeTripleToNode(kSampling);
    node[keyToName.at(Key::nOccupied)] = nOccupied;
    node[keyToName.at(Key::nUnoccupied)] = nUnoccupied;
    node[keyToName.at(Key::cISDF)] = cISDF;
    node[keyToName.at(Key::cvtMaximumSteps)] = cvtMaximumSteps;
    node[keyToName.at(Key::cvtConvergenceTolerance)] = cvtConvergenceTolerance;
    node[keyToName.at(Key::nISDFOccupiedUnoccupied)] = nISDFOccupiedUnoccupied;
    node[keyToName.at(Key::nISDFOccupied)] = nISDFOccupied;
    node[keyToName.at(Key::nISDFUnoccupied)] = nISDFUnoccupied;
    node[keyToName.at(Key::nLanczosIterations)] = nLanczosIterations;
    return node;
}


void Config::print_to_stdout(){
    std::string seperator = ": ";
    std::cout<<keyToName.at(Key::seed)<< seperator << seedToString.at(seed) <<  std::endl;
    std::cout<<keyToName.at(Key::rSampling)<< seperator << typ::sizeTripleToString(rSampling) << std::endl;
    std::cout<<keyToName.at(Key::kSampling)<< seperator << typ::sizeTripleToString(kSampling) << std::endl;
    std::cout<<keyToName.at(Key::nOccupied)<< seperator << nOccupied << std::endl;
    std::cout<<keyToName.at(Key::nUnoccupied)<< seperator << nUnoccupied << std::endl;
    std::cout<<keyToName.at(Key::cISDF)<< seperator << cISDF << std::endl;
    std::cout<<keyToName.at(Key::cvtMaximumSteps)<< seperator << cvtMaximumSteps << std::endl;
    std::cout<<keyToName.at(Key::cvtConvergenceTolerance)<< seperator << cvtConvergenceTolerance << std::endl;
    std::cout<<keyToName.at(Key::nISDFOccupiedUnoccupied)<< seperator << nISDFOccupiedUnoccupied << std::endl;
    std::cout<<keyToName.at(Key::nISDFOccupied)<< seperator << nISDFOccupied << std::endl;
    std::cout<<keyToName.at(Key::nISDFUnoccupied)<< seperator << nISDFUnoccupied << std::endl;
    std::cout<<keyToName.at(Key::nLanczosIterations)<< seperator << nLanczosIterations << std::endl;
}

std::size_t Config::nRpoints(){
    return rSampling.at(0) * rSampling.at(1) * rSampling.at(2);
}

std::size_t Config::nKpoints(){
    return kSampling.at(0) * kSampling.at(1) * kSampling.at(2);
}