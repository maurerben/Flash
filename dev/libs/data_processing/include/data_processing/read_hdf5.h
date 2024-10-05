
#ifndef FLASH_READ_H
#define FLASH_READ_H

#include <string>
#include <vector>

#include <H5Cpp.h>

#include <types/types.h>

namespace flash {
namespace data_processing {
namespace read_hdf5 {

/**
 * @brief Read a \p flash::types::real_dp vector from an HDF5 file
 *
 * @param[in] filename Name of the HDF5 file
 * @param[in] datasetName Absolute name of the dataset
 * @return A \p flash::types::DVector initialized with the data from the file
 */
flash::types::DVector readDoubleVectorHDF5(const std::string &filename, const std::string &datasetName);

}  // namespace read_hdf5
}  // namespace data_processing
}  // namespace flash

#endif  // FLASH_READ_H