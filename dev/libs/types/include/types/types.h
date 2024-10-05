#ifndef FLASH_TYPES_H
#define FLASH_TYPES_H

#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <array>
#include <complex>
#include <cstdint>
#include <string>

namespace flash {
namespace types {

typedef std::size_t size;
typedef std::double_t real_dp;
typedef std::complex<real_dp> complex_dp;

typedef Eigen::Vector<real_dp, Eigen::Dynamic> DVector;
typedef Eigen::Vector<complex_dp, Eigen::Dynamic> CVector;
typedef Eigen::Matrix<real_dp, Eigen::Dynamic, Eigen::Dynamic> DMatrix;
typedef Eigen::Matrix<complex_dp, Eigen::Dynamic, Eigen::Dynamic> CMatrix;

}  // namespace types
}  // namespace flash

#endif  // FLASH_TYPES_H