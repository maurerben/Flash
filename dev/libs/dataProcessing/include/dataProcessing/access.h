//
// Created by bene on 3/23/25.
//
// Functions for writing and reading blocks from HDF5 files.
//
// # Specification:
// 1.  Write and read functions must be of type void
// 2.  The first argument that is passed must be an `H5::H5Object`
// 3.  ElectronData must be passed as `const` reference to write functions
// 4.  ElectronData must be passed as reference to read functions
// 5.  Every read function requires a write function and vice verca
// 6.  The whole read or write action must be performed in an try/catch environment
// 7.  In case of an exception the program must be terminated and the error messege
//     passed to std::cerr
// 8.  Each write/read pair has their own namespace (e.g. `electronData`)
// 9.  The read and write function must be named `read` and `write`, respectively
// 10. The data blocks must be saved in their own group.
// 11. Array objects are saved to datasets
// 12. Scalar objects are saved to attributes
// 13. The hdf5 data types must be determined with `getPredType`
// 14. Names for groups, datasets and attributes must be saved as `constexpr std::string`
//     in `names.h`.

#pragma once

#include <H5Cpp.h>
#include <utils/types.h>

#include <Eigen/Core>
#include <cstdint>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "names.h"

namespace flashlight {
namespace dataProcessing {

using namespace utils;

/**
 * @brief Writes electron-related data to an HDF5 group.
 *
 * This function creates (or overwrites) an HDF5 group inside the provided HDF5 object
 * and writes the following electron-related data:
 *
 * - Number of k-points (as an attribute)
 * - Number of bands (as an attribute)
 * - Energy values (as a 1D dataset)
 * - Occupation numbers (as a 1D dataset)
 *
 * If a group with the target name already exists, it will be deleted and recreated
 * if `overwrite` is set to true. Otherwise, the function assumes the group does not exist.
 *
 * @param h5obj        The HDF5 object (file or group) under which the electron group is created.
 * @param numKpoints   Number of k-points (written as an attribute).
 * @param numBands     Number of bands (written as an attribute).
 * @param energies     Vector of energy values (written as a dataset).
 * @param occupations  Vector of occupation numbers (written as a dataset).
 * @param overwrite    If true, overwrite the group if it already exists.
 */
void writeElectrons(H5::H5Object& h5object, const index_t& numKpoints, const index_t& numElectrons,
                    const vector_t<real_t>& electrons, const vector_t<real_t>& occupations, bool overwrite = true);

/**
 * @brief Reads electron-related data from an HDF5 group.
 *
 * This function reads the electron group stored within the given HDF5 object and
 * extracts the following data:
 *
 * - Number of k-points (from an attribute)
 * - Number of bands (from an attribute)
 * - Energy values (from a 1D dataset)
 * - Occupation numbers (from a 1D dataset)
 *
 * The input vectors `energies` and `occupations` will be resized to match the size of
 * the corresponding datasets.
 *
 * @param h5obj         The HDF5 object (file or group) containing the electron group.
 * @param numKpoints    Output: number of k-points (read from an attribute).
 * @param numBands      Output: number of bands (read from an attribute).
 * @param energies      Output: energy values (read from a 1D dataset).
 * @param occupations   Output: occupation numbers (read from a 1D dataset).
 */
void readElectrons(const H5::H5Object& h5object, index_t& numKpoints, index_t& numElectrons,
                   vector_t<real_t>& electrons, vector_t<real_t>& occupations);

}  // namespace dataProcessing
}  // namespace flashlight
