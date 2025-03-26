#ifndef FLASH_dataProcessing_HDF5_H
#define FLASH_dataProcessing_HDF5_H

#include <H5Cpp.h>
#include <Eigen/Dense>
#include <complex>
#include <cstdint>
#include <exception>
#include <string>
#include <type_traits>
#include <vector>
#include <array>

namespace flashlight {
namespace dataProcessing {
namespace hdf5 {

template <typename T>
concept ArithmeticType = std::is_arithmetic<T>::value;

/**
 * @brief Read a vector from a dataset.
 * 
 * @tparam T The type of the elements of the vector.
 * @param dataset The dataset to read the vector from.
 * @return The vector read from the dataset.
// */
//    template<typename T, std::size_t N> void
//    read_vector( H5::DataSet& dataset, T* data );

template<std::size_t N>
void read_data(H5::DataSet& dataset, std::double_t data[N]);


}  // namespace hdf5
}  // namespace dataProcessing
}  // namespace flashlight

#endif  // FLASH_dataProcessing_HDF5_H