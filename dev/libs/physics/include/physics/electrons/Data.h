#pragma once

#include <utils/types.h>
#include <utils/IndexMap.h>
#include <dataProcessing/hdf5/access.h>
#include <Eigen/Dense>
#include <cstdint>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <complex>
#include <H5Cpp.h>
#include "Index.h"

namespace flash {
namespace physics {
namespace electrons {

using namespace utils;


/**
 * @class Data
 * @brief Holds data for electronic states.
 *
 * This class encapsulates the energies, occupations, k-point indizes, and band indices of electronic states.
 * It provides methods to retrieve the data associated with these properties for further calculations.
 */
class Data {
   private:
    index_t numKpoints = 0;	/**< Number of __k__-points. */
    index_t numBands = 0; /**< Number of bands. */
    vector_t<real_t> energies; /**< Stores the energies of the electronic states */
    vector_t<real_t> occupations; /**< Stores the occupations of the electronic states */
	// TODO matrix_t<cplx_t> waveFunctions;

   public:

    /**
     * @brief Default constructor for Data.
     *
     * Initializes empty data containers for energies, occupations, k-point map, and band map.
     */
    Data();


    /**
     * @brief Constructor to initialize Data with provided vectors and maps.
     *
	 * @param numKpoints Number of __k__-points.
	 * @param numBands Number of bands.
     * @param energies A vector containing the energies of the electronic states.
     * @param occupations A vector containing the occupations of the electronic states.
     */
    Data(const index_t numKpoints,
         const index_t numBands,
         const vector_t<real_t>& energies,
         const vector_t<real_t>& occupations);


    /**
     * @brief Constructor to initialize Data from configuration and an HDF5 group.
     *
     * This constructor loads data from an HDF5 file group and an input configuration.
     *
     * @param group The HDF5 group from which data will be loaded.
     */
    explicit Data(const H5::H5Object& h5obj);


    /**
     * @brief Destructor for Data.
     */
    ~Data();


    /**
	 * @brief Retrieve number of __k__-points.
	 */
     const index_t NumKpoints() const;


     /**
	  * @brief Retrieve number of bands.
	  */
     const index_t NumBands() const;

    /**
     * @brief Retrieves the energy for a given index.
     *
     * @param index The index for which the energy is to be retrieved.
     * @return The energy at the specified index.
     */
    const real_t Energy(const Index& index) const;


    /**
     * @brief Retrieves the energies for a set of indices.
     *
     * @param indices The indices for which the energies are to be retrieved.
     * @return A vector of energies corresponding to the provided indices.
     */
    const vector_t<real_t> Energies(Indices indices) const;


    /**
     * @brief Returns a constant reference to the energies.
     *
     * @return A reference to the vector containing the energies of the electronic states.
     */
    const vector_t<real_t>& Energies() const;


    /**
     * @brief Retrieves the occupation for a given index.
     *
     * @param index The index for which the occupation is to be retrieved.
     * @return The occupation at the specified index.
     */
    const real_t Occupation(const Index& index) const;


    /**
     * @brief Retrieves the occupations for a set of indices.
     *
     * @param indices The indices for which the occupations are to be retrieved.
     * @return A vector of occupations corresponding to the provided indices.
     */
    const vector_t<real_t> Occupations(Indices indices) const;


    /**
     * @brief Returns a constant reference to the occupations.
     *
     * @return A reference to the vector containing the occupations of the electronic states.
     */
    const vector_t<real_t>& Occupations() const;

};

}  // namespace electrons
}  // namespace physics
}  // namespace flash