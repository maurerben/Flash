//
// Created by benedikt on 15.07.24.
//

#ifndef LIGHTNING_TYPES_H
#define LIGHTNING_TYPES_H

#include <array>
#include <string>
#include <cstdint>
#include <complex>
#include <Eigen/Dense>

#include <yaml-cpp/yaml.h>

namespace flash{

    typedef std::size_t size;
    typedef std::double_t real;
    typedef std::complex<real> complex;

    typedef Eigen::Matrix<complex, Eigen::Dynamic, Eigen::Dynamic> DMatrix;
    typedef Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic> CMatrix;



    typedef std::complex<std::double_t> Complex;


}




#endif //LIGHTNING_TYPES_H