//
// Created by benedikt on 15.07.24.
//

#include "types/types.h"

using namespace typ;

YAML::Node typ::sizeTripleToNode(const SizeTriple &sizeTriple) {
    YAML::Node node;
    node.push_back(sizeTriple[0]);
    node.push_back(sizeTriple[1]);
    node.push_back(sizeTriple[2]);
    return node;
}

std::string typ::sizeTripleToString(const SizeTriple &sizeTriple) {
    std::string seperator = " x ";
    return std::to_string(sizeTriple[0]) + seperator
         + std::to_string(sizeTriple[1]) + seperator
         + std::to_string(sizeTriple[2]);
}

SizeTriple typ::nodeToSizeTriple(const YAML::Node &node){

    return SizeTriple{
        node[0].as<std::size_t>(), 
        node[1].as<std::size_t>(), 
        node[2].as<std::size_t>()
    };
}