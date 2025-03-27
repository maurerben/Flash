//
// Created by bene on 3/16/25.
//

#pragma once

#include <Eigen/Dense>
#include <complex>
#include <cstdint>

namespace flashlight {
namespace utils {

typedef int int_t;
typedef Eigen::Index index_t;
typedef std::double_t real_t;
typedef std::complex<real_t> cplx_t;

template <typename T>
using vector_t = Eigen::Vector<T, Eigen::Dynamic>;

template <typename T>
using matrix_t = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

}  // namespace utils
}  // namespace flashlight
