//
// Created by bene on 3/16/25.
//

#pragma once

#include "utils/types.h"
#include <Eigen/Dense>
#include <H5Cpp.h>
#include <iostream>


namespace flash {
namespace utils {

/**
 * @brief A class to manage an index map.
 *
 * The IndexMap class maps elements of one array to elements of a target array.
 */
class IndexMap {

  private:

    /** @brief Array of the target indices. */
    vector_t<index_t> targetIndices;


    /** @brief Number of targets that are mapped to. */
    index_t numberOfTargets = 0;


  public:

    /**
     * @brief Default constructor.
     *
     * Initializes an empty IndexMap with zero targets.
     */
    IndexMap();


    /**
     * @brief Constructs an IndexMap with given target indices and number of targets.
     *
     * @param targetIndices A vector containing target indices.
     * @param numberOfTargets The number of targets in the mapping.
     */
    IndexMap(const vector_t<index_t>& targetIndices, const index_t& numberOfTargets);


    /**
     * @brief Constructs an IndexMap from an HDF5 group.
     *
     * @param group The HDF5 group containing the index map data.
     */
    IndexMap(const H5::Group& group);


    /**
     * @brief Destructor.
     *
     * Cleans up resources by resetting the target indices and number of targets.
     */
    ~IndexMap();


    /**
     * @brief Overload the == operator.
     */
    bool operator==(const IndexMap& other) const;


    /**
     * @brief Overload the != operator.
     */
    bool operator!=(const IndexMap& other) const;


    /**
     * @brief Gets the number of mapped elements.
     *
     * @return The size of the target indices vector.
     */
    index_t size() const;


    /**
     * @brief Gets the number of target elements.
     *
     * @return The total number of targets.
     */
    const index_t& NumberOfTargets() const;


    /**
     * @brief Retrieves the target index for a given element.
     *
     * @param index The index to look up.
     * @return The corresponding target index.
     */
    const index_t TargetIndex(const index_t& index) const;


    /**
     * @brief Gets the vector of target indices.
     *
     * @return A reference to the target indices vector.
     */
    const vector_t<index_t>& TargetIndices() const;


    /**
     * @brief Retrieves a subset of target indices.
     *
     * @param indices A vector containing indices to extract.
     * @return A vector containing the selected target indices.
     */
    const vector_t<index_t> TargetIndices(const vector_t<index_t>& indices) const;

};

}
}
