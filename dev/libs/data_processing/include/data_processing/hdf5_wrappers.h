
#ifndef FLASH_DATA_PROCESSING_HDF5_WRAPPERS_H
#define FLASH_DATA_PROCESSING_HDF5_WRAPPERS_H

#include <string>
#include <cstdint>
#include <complex>
#include <type_traits>
#include <exception>
#include <vector>

#include <H5Cpp.h>

#include <types/types.h>

namespace flash {
namespace data_processing {
namespace hdf5_wrappers {


/**
 * @brief Read a \p flash::types::my_double vector from an HDF5 file
 *
 * @param[in] filename Name of the HDF5 file
 * @param[in] datasetName Absolute name of the dataset
 * @return A \p flash::types::DVector initialized with the data from the file
 */
template<typename T>
flash::types::vector<T> readVector(const H5::DataSet &dataset);

}  // namespace hdf5_wrappers
}  // namespace data_processing
}  // namespace flash


#endif  // FLASH_DATA_PROCESSING_HDF5_WRAPPERS_H