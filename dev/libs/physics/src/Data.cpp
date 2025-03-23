#include <physics/electrons/Data.h>

using namespace flash::physics::electrons;
using namespace flash::utils;
using namespace flash::dataProcessing;



// Function to convert a boolean mask to a vector of indices
//std::vector<index_t> maskToIndices(const Eigen::Array<bool, Eigen::Dynamic, 1>& mask) {
//    std::vector<index_t> indices;
//    for (index_t i = 0; i < mask.size(); ++i) {
//        if (mask[i]) {
//            indices.push_back(i);
//        }
//    }
//    return indices;
//}

/** @brief Validate Members of Data. */
void validateMembers(
    const index_t numKpoints,
    const index_t numBands,
    const vector_t<real_t>& energies,
    const vector_t<real_t>& occupations) {
  	auto expectedSize = numKpoints * numBands;
    if (
    	expectedSize != energies.size() ||
        expectedSize != occupations.size()
        ) {
        throw std::invalid_argument(
          	"The sizes of energies and occupations must be the \
			same as `numKpoints * numBands.`");
    } else if (
        (occupations.array() < 0.0).any() ||
        (occupations.array() > 2.0).any()
      	) {
      throw std::invalid_argument(
          	"Occupations must be in the interval [0., 2.]."
      );
    }
}


/**
 * @brief Default constructor for Data.
 *
 * Initializes empty vectors and maps.
 */
Data::Data() : energies(), occupations() {}


/**
 * @brief Constructor to initialize Data with provided vectors and maps.
 *
 * @param numKpoints Number of __k__-points.
 * @param numBands Number of bands.
 * @param energies A vector containing the energies of the electronic states.
 * @param occupations A vector containing the occupations of the electronic states.
 */
Data::Data(
    const index_t numKpoints,
    const index_t numBands,
    const vector_t<real_t>& energies,
    const vector_t<real_t>& occupations)
    : numKpoints(numKpoints),
      numBands(numBands),
      energies{energies},
      occupations{occupations} {
  	validateMembers(numKpoints, numBands, energies, occupations);
}



/**
 * @brief Constructor to initialize Data from configuration and an HDF5 group.
 *
 * This constructor loads data from an HDF5 file group and an input configuration.
 *
 * @param h5obj The HDF5 group or file from which data will be loaded.
 */
Data::Data(const H5::H5Object& h5obj)
{
    hdf5::electrons::read(h5obj, numKpoints, numBands, energies, occupations);
  	validateMembers(numKpoints, numBands, energies, occupations);
}


/**
  * @brief Destructor for Data.
  */
Data::~Data() {
    numKpoints = 0;
    numBands = 0;
    energies.resize(0);
    occupations.resize(0);
}


/**
* @brief Retrieve number of __k__-points.
*/
const index_t Data::NumKpoints() const {
    return numKpoints;
}


/**
 * @brief Retrieve number of bands.
 */
const index_t Data::NumBands() const {
    return numBands;
}


/**
 * @brief Retrieves the energy for a given index.
 *
 * @param index The index for which the energy is to be retrieved.
 * @return The energy at the specified index.
 */
const real_t Data::Energy(const Index& index) const {
    return energies(index.getElectron());
}


/**
 * @brief Retrieves the energies for a set of indices.
 *
 * @param indices The indices for which the energies are to be retrieved.
 * @return A vector of energies corresponding to the provided indices.
 */
const vector_t<real_t> Data::Energies(Indices indices) const {
    return energies(indices.getElectrons());
}


/**
 * @brief Returns a constant reference to the energies.
 *
 * @return A reference to the vector containing the energies of the electronic states.
 */
const vector_t<real_t>& Data::Energies() const {return energies;}


/**
 * @brief Retrieves the occupation for a given index.
 *
 * @param index The index for which the energy is to be retrieved.
 * @return The occupation at the specified index.
 */
const real_t Data::Occupation(const Index& index) const {
    return occupations(index.getElectron());
}


/**
 * @brief Retrieves the occupations for a set of indices.
 *
 * @param indices The indices for which the occupations are to be retrieved.
 * @return A vector of occupations corresponding to the provided indices.
 */
const vector_t<real_t> Data::Occupations(Indices indices) const {
    return occupations(indices.getElectrons());
}


/**
 * @brief Returns a constant reference to the occupations.
 *
 * @return A reference to the vector containing the occupations of the electronic states.
 */
const vector_t<real_t>& Data::Occupations() const {
    return occupations;
}
