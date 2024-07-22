//
// Created by benedikt on 15.07.24.
//

#ifndef LIGHTNING_TYPES_H
#define LIGHTNING_TYPES_H

#include <array>
#include <string>
#include <cstdint>
#include <complex>

#include <yaml-cpp/yaml.h>

namespace typ{

    /// 3D sampling as number of points per lattice direction.
    typedef std::array<std::size_t, 3> SizeTriple;

    /**
     * @brief Transform SizeTriple to YAML::Node.
     */
    YAML::Node sizeTripleToNode(const SizeTriple &sizeTriple);

    /**
     * @brief Transform YAML::Node to SizeTriple.
     */
    SizeTriple nodeToSizeTriple(const YAML::Node &node);

    /**
     * @brief Transform SizeTriple to string.
     */
    std::string sizeTripleToString(const SizeTriple &sizeTriple);



    typedef std::complex<std::double_t> Complex;


}




#endif //LIGHTNING_TYPES_H