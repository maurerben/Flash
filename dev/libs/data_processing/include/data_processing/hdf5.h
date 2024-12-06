#ifndef FLASH_DATA_PROCESSING_HDF5_H
#define FLASH_DATA_PROCESSING_HDF5_H

#include <H5Cpp.h>
#include <Eigen/Dense>
#include <complex>
#include <cstdint>
#include <exception>
#include <string>
#include <type_traits>
#include <vector>

namespace flash {
namespace data_processing {
namespace hdf5 {

template <typename T>
concept ArithmeticType = std::is_arithmetic<T>::value;

/**
 * @brief Read a vector from a dataset.
 * 
 * @tparam T The type of the elements of the vector.
 * @param dataset The dataset to read the vector from.
 * @return The vector read from the dataset.
 */
template<ArithmeticType T> Eigen::Vector<T, Eigen::Dynamic> 
read_vector(const H5::DataSet& dataset);


}  // namespace hdf5
}  // namespace data_processing
}  // namespace flash

#endif  // FLASH_DATA_PROCESSING_HDF5_H