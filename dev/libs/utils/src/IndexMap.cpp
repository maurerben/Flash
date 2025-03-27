//
// Created by bene on 3/16/25.
//
#include <utils/IndexMap.h>

using namespace flashlight::utils;

/** @brief Validate Members of IndexMap. */
void validateMembers(const vector_t<index_t>& targetIndices, const index_t& numberOfTargets) {
    if (numberOfTargets <= 0) {
        throw std::invalid_argument("Number of targets must be greater than zero");
    }
    if ((targetIndices.array() < 0).any()) {
        throw std::invalid_argument("Elements of targetIndices must be non-negative");
    }
    if ((targetIndices.array() > numberOfTargets).any()) {
        throw std::invalid_argument("Elements of targetIndices must be smaller equal then numberOfTargets");
    }
}

/**
 * @brief Default constructor.
 *
 * Initializes an empty IndexMap with zero targets.
 */
IndexMap::IndexMap() = default;

IndexMap::IndexMap(const vector_t<index_t>& targetIndices, const index_t& numberOfTargets)
    : numberOfTargets(numberOfTargets), targetIndices(targetIndices) {
    validateMembers(targetIndices, numberOfTargets);
}

/**
 * @brief Constructs an IndexMap from an HDF5 group.
 *
 * @param group The HDF5 group containing the index map data.
 */
IndexMap::IndexMap(const H5::Group& group) {
    try {
        // Read number of targets
        H5::Attribute attr = group.openAttribute("number_of_targets");
        attr.read(H5::PredType::NATIVE_LONG, &numberOfTargets);

        // Read target indices dataset
        H5::DataSet dataset = group.openDataSet("target_indices");

        // Get dataset size
        H5::DataSpace dataspace = dataset.getSpace();
        hsize_t dims[1];
        dataspace.getSimpleExtentDims(dims, nullptr);
        index_t dataSize = static_cast<index_t>(dims[0]);

        // Read dataset into Eigen vector
        targetIndices.resize(dataSize);
        dataset.read(targetIndices.data(), H5::PredType::NATIVE_LONG);
    } catch (const H5::Exception& e) {
        throw std::runtime_error("Error loading IndexMap from HDF5: " + std::string(e.getDetailMsg()));
    }
    validateMembers(targetIndices, numberOfTargets);
}

/**
 * @brief Destructor.
 *
 * Cleans up resources by resetting the target indices and number of targets.
 */
IndexMap::~IndexMap() {
    numberOfTargets = 0;
    targetIndices.resize(0);
}

/**
 * @brief Overload the == operator.
 */
bool IndexMap::operator==(const IndexMap& other) const {
    if (this->size() != other.size()) {
        return false;
    } else {
        return numberOfTargets == other.numberOfTargets && targetIndices.isApprox(other.targetIndices);
    }
}

/**
 * @brief Overload the != operator.
 */
bool IndexMap::operator!=(const IndexMap& other) const { return !(*this == other); }

/**
 * @brief Gets the number of mapped elements.
 *
 * @return The size of the target indices vector.
 */
index_t IndexMap::size() const { return targetIndices.size(); }

/*
 * @brief Gets the number of target elements.
 *
 * @return The total number of targets.
 */
const index_t& IndexMap::NumberOfTargets() const { return numberOfTargets; }

/**
 * @brief Retrieves the target index for a given element.
 *
 * @param index The index to look up.
 * @return The corresponding target index.
 */
const index_t IndexMap::TargetIndex(const index_t& index) const { return targetIndices(index); }

/**
 * @brief Gets the vector of target indices.
 *
 * @return A reference to the target indices vector.
 */
const vector_t<index_t>& IndexMap::TargetIndices() const { return targetIndices; }

/**
 * @brief Retrieves a subset of target indices.
 *
 * @param indices A vector containing indices to extract.
 * @return A vector containing the selected target indices.
 */
const vector_t<index_t> IndexMap::TargetIndices(const vector_t<index_t>& indices) const {
    return targetIndices(indices);
}