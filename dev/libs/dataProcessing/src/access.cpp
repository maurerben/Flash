#include <dataProcessing/hdf5/access.h>


using namespace flash::dataProcessing::hdf5;


H5::PredType getPredType(const std::type_info& ti) {
    static const std::unordered_map<std::type_index, H5::PredType> type_map = {
        {typeid(int),    H5::PredType::NATIVE_INT},
        {typeid(Eigen::Index),  H5::PredType::NATIVE_LONG},
        {typeid(std::float_t), H5::PredType::NATIVE_FLOAT},
        {typeid(std::double_t), H5::PredType::NATIVE_DOUBLE}
        // etc.
    };

    auto it = type_map.find(std::type_index(ti));
    if (it != type_map.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Unknown type");
    }

}


H5::Group creatGroup(H5::H5Object& h5obj, const std::string& groupName, bool overwrite) {
    try {
        // Check if the group exists
        if (H5Lexists(h5obj.getId(), groupName.c_str(), H5P_DEFAULT) > 0) {
            if (overwrite) {
                H5Ldelete(h5obj.getId(), groupName.c_str(), H5P_DEFAULT);
            } else {
                throw std::runtime_error("Overwrite is disabled, cannot delete " + groupName + "\n");
            }
            
        }

        // Now create the group
        H5::Group group = h5obj.createGroup(groupName);
        std::cout << "Created group: " << groupName << "\n";

        return group;
    }
    catch (const H5::Exception& e) {
        std::cerr << "HDF5 error: " << e.getDetailMsg() << std::endl;
        throw;
    }

}

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
void electrons::write(
    H5::H5Object& h5obj,
    const index_t& numKpoints,
    const index_t& numBands,
    const vector_t<real_t>& energies,
    const vector_t<real_t>& occupations,
    bool overwrite) {

    using namespace flash::dataProcessing::hdf5::names::electrons;

    try {
        H5::Group group = creatGroup(h5obj, groupName, overwrite);

        // Number of k-points (as an attribute)
        H5::Attribute attrNumKpoints =
            group.createAttribute(
                numKpointsAtrrName, getPredType(typeid(index_t)), H5::DataSpace()
            );
        attrNumKpoints.write(getPredType(typeid(index_t)), &numKpoints);

        // Number of bands (as an attribute)
        H5::Attribute attrNumBands =
            group.createAttribute(
                numBandsAtrrName, getPredType(typeid(index_t)), H5::DataSpace()
            );
        attrNumBands.write(getPredType(typeid(index_t)), &numBands);

        // create eneriges data set
        hsize_t dimsEnergies[1] = {static_cast<hsize_t>(energies.size())};
        H5::DataSpace energiesDataspace(1, dimsEnergies);
        H5::DataSet energiesDataSet
            = group.createDataSet(
                energiesDsetName, getPredType(typeid(real_t)), energiesDataspace);
        energiesDataSet.write(energies.data(), getPredType(typeid(real_t)));

        // create occupations data set
        hsize_t dimsOccupations[1] = {static_cast<hsize_t>(occupations.size())};
        H5::DataSpace occupationsDataspace(1, dimsOccupations);
        H5::DataSet occupationsDataSet
            = group.createDataSet(
                occupationsDsetName, getPredType(typeid(real_t)), occupationsDataspace);
        occupationsDataSet.write(occupations.data(), getPredType(typeid(real_t)));
    } catch (const std::exception &e) {
        std::cerr << "write::electronData: Caught exception: " << e.what() << std::endl;
    }
}

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
void electrons::read(
    const H5::H5Object& h5obj,
    index_t& numKpoints,
    index_t& numBands,
    vector_t<real_t>& energies,
    vector_t<real_t>& occupations) {

    using namespace flash::dataProcessing::hdf5::names::electrons;

    try {
        H5::Group group = h5obj.openGroup(groupName);

        // Read number of numKpoints
        H5::Attribute attrNumKpoints = group.openAttribute(numKpointsAtrrName);
        attrNumKpoints.read(getPredType(typeid(index_t)), &numKpoints);

        // Read number of targets
        H5::Attribute attrBands = group.openAttribute(numBandsAtrrName);
        attrBands.read(getPredType(typeid(index_t)), &numBands);

        // Load energies from the HDF5 group
        H5::DataSet energyDataset = group.openDataSet(energiesDsetName);
        H5::DataSpace energySpace = energyDataset.getSpace();
        hsize_t energyDims[1];
        energySpace.getSimpleExtentDims(energyDims, nullptr);

        energies.resize(energyDims[0]);
        energyDataset.read(energies.data(), getPredType(typeid(real_t)));

        // Load occupations from the HDF5 group
        H5::DataSet occupationDataset = group.openDataSet(occupationsDsetName);
        H5::DataSpace occupationSpace = occupationDataset.getSpace();
        hsize_t occupationDims[1];
        occupationSpace.getSimpleExtentDims(occupationDims, nullptr);

        occupations.resize(occupationDims[0]);
        occupationDataset.read(occupations.data(), getPredType(typeid(real_t)));
          
    } catch (const std::exception &e) {
        std::cerr << "read::electronData: Caught exception: " << e.what() << std::endl;
    }
}



