//
// Created by bene on 3/26/25.
//

#pragma once
#include <utils/types.h>

#include <Eigen/Dense>
#include <cassert>
#include <stdexcept>
#include <string>
#include <tuple>

#include "ElectronIndex.h"

using namespace flashlight::utils;

namespace flashlight {
namespace physics {

class TransitionIndex {
   private:
    index_t transition = 0; /**< Transition index. */
    index_t kpoint = 0;     /**< __k__-point index. */
    index_t occupied = 0;   /**< Occupied band index. */
    index_t unoccupied = 0; /**< Unoccupied band index. */

   public:
    // @brief Default constructor
    TransitionIndex() = default;

    // @brief Copy constructor
    TransitionIndex(const TransitionIndex& other) = default;

    // @brief Copy assignment constructor
    constexpr TransitionIndex& operator=(const TransitionIndex& other) = default;

    /**
     * @brief Constructs an TransitionIndex object.
     * @details The constructor creates an index for a given transition, __k__-point, occupied and unoccupied band
     * index.
     * @param transition The transition index.
     * @param kpoint The __k__-point index.
     * @param occupied The occupied band index.
     * @param unoccupied The unoccupied band index.
     */
    TransitionIndex(const index_t& transition, const index_t& kpoint, const index_t& occupied,
                    const index_t& unoccupied)
        : transition{transition}, kpoint{kpoint}, occupied{occupied}, unoccupied{unoccupied} {
        assert(transition >= 0);
        assert(kpoint >= 0);
        assert(occupied >= 0);
        assert(unoccupied >= 0);
    }

    ~TransitionIndex() = default;  // Destructor

    // @brief Gets the electron index.
    index_t getTransition() const { return transition; }

    // @brief Gets the __k__-point index.
    index_t getKpoint() const { return kpoint; }

    // @brief Gets the occupied band index.
    index_t getOccupied() const { return occupied; }

    // @brief Gets the unoccupied band index.
    index_t getUnoccupied() const { return unoccupied; }

    // @brief Gets the index as a tuple.
    std::tuple<index_t, index_t, index_t, index_t> asTuple() const;

    /**
     * @brief Compares two indices.
     * @details The operator compares two indices and returns true if they are equal, false otherwise.
     * @param other The other index to compare with.
     * @return True if the indices are equal, false otherwise.
     */
    bool operator==(const TransitionIndex& other) const;

    /**
     * @brief Compares two indices.
     * @details The operator compares two indices and returns true if they are not equal, false otherwise.
     * @param other The other index to compare with.
     * @return True if the indices are not equal, false otherwise.
     */
    bool operator!=(const TransitionIndex& other) const;
};

/**
 * @class TransitionIndices
 * @brief Represents and manages a list of transition indices from occupied to unoccupied bands
 *        across __k__-points.
 *
 * This class stores transitions between occupied and unoccupied electronic bands
 * across multiple __k__-points. It provides utilities to construct the set of transitions,
 * access them individually, or filter them by __k__-point, band index, or transition index.
 *
 * The transitions are internally represented as a std::vector of `TransitionIndex` objects.
 * The class also maintains metadata about the number of k-points and bands involved.
 */
class TransitionIndices {
   private:
    std::vector<TransitionIndex> indices; /**< Vector that stores the indices of all transitions. */
    index_t numKpoints = 0;               /**< Number of k-points. */
    index_t numOccupied = 0;              /**< Number of occupied bands. */
    index_t numUnoccupied = 0;            /**< Number of unoccupied bands. */
    index_t offsetOccupied = 0;           /**< Offset of unoccupied bands. */
    index_t offsetUnoccupied = 0;         /**< Offset of unoccupied bands. */

   public:
    /** @brief Default constructor. */
    TransitionIndices() = default;

    /** @brief Copy constructor. */
    TransitionIndices(const TransitionIndices& other) = default;

    /** @brief Copy assignment operator. */
    TransitionIndices& operator=(const TransitionIndices& other) = default;

    /**
     * @brief Constructs the transition index list from occupied and unoccupied band indices.
     * @param occupiedBands Set of occupied band indices per k-point.
     * @param unoccupiedBands Set of unoccupied band indices per k-point.
     */
    TransitionIndices(const ElectronIndices& occupiedBands, const ElectronIndices& unoccupiedBands);

    /**
     * @brief Constructs the transition index list from a tuple of occupied and unoccupied band indices.
     * @param electronIndices Tuple containing {occupiedBands, unoccupiedBands}.
     */
    TransitionIndices(std::tuple<ElectronIndices, ElectronIndices>& electronIndices);

    /** @brief Destructor. */
    ~TransitionIndices();

    /**
     * @brief Returns the total number of transitions stored.
     * @return Number of transition indices.
     */
    index_t size() const;

    /**
     * @brief Returns the number of k-points.
     * @return Number of k-points.
     */
    index_t NumKpoints() const { return numKpoints; }

    /**
     * @brief Returns the number of occupied bands.
     * @return Number of occupied bands.
     */
    index_t NumOccupied() const { return numOccupied; }

    /**
     * @brief Returns the number of unoccupied bands.
     * @return Number of unoccupied bands.
     */
    index_t NumUnoccupied() const { return numUnoccupied; }

    /**
     * @brief Access a transition by its index.
     * @param index Global index into the transition list.
     * @return Corresponding TransitionIndex.
     */
    TransitionIndex operator[](const index_t& index) const;

    /**
     * @brief Returns a vector of all transition indices (global).
     * @return Vector of transition indices.
     */
    vector_t<index_t> getTransitions() const;

    /**
     * @brief Returns the list of k-point indices involved in the transitions.
     * @return Vector of k-point indices.
     */
    vector_t<index_t> getKpoints() const;

    /**
     * @brief Returns the list of occupied band indices.
     * @return Vector of occupied band indices.
     */
    vector_t<index_t> getOccBands() const;

    /**
     * @brief Returns the list of unoccupied band indices.
     * @return Vector of unoccupied band indices.
     */
    vector_t<index_t> getUnoBands() const;

    /**
     * @brief Filters transitions by a specific k-point.
     * @param kpoint The k-point index to filter by.
     * @return A new TransitionIndices object containing only transitions at the given k-point.
     */
    TransitionIndices atKpoint(const index_t& kpoint) const;

    /**
     * @brief Filters transitions by a specific occupied band.
     * @param occupied The occupied band index to filter by.
     * @return A new TransitionIndices object containing only transitions from the given occupied band.
     */
    TransitionIndices atOccupied(const index_t& occupied) const;

    /**
     * @brief Filters transitions by a specific unoccupied band.
     * @param unoccupied The unoccupied band index to filter by.
     * @return A new TransitionIndices object containing only transitions to the given unoccupied band.
     */
    TransitionIndices atUnoccupied(const index_t& unoccupied) const;

    /**
     * @brief Filters transitions by occupied and unoccupied index.
     * @param transition Index of the transition to isolate.
     * @return A new TransitionIndices object containing only the specified transition.
     */
    TransitionIndex atOccupiedUnoccupied(const index_t& occupied, const index_t& unoccupied) const;

    /**
     * @brief Filters transitions by transition index (e.g. transition ID).
     * @param transition Index of the transition to isolate.
     * @return A new TransitionIndices object containing only the specified transition.
     */
    TransitionIndex atTransition(const index_t& transition) const;

    /**
     * @brief Iterator class for traversing TransitionIndices.
     *
     * This iterator allows read-only access to each TransitionIndex
     * stored in the parent TransitionIndices container.
     */
    class Iterator {
       private:
        const TransitionIndices& indices;  ///< Reference to the TransitionIndices container.
        index_t index;                     ///< Current position in the container.

       public:
        /**
         * @brief Constructs an iterator.
         *
         * @param indices Reference to the TransitionIndices container to iterate over.
         * @param index Initial position of the iterator.
         */
        Iterator(const TransitionIndices& indices, index_t index) : indices{indices}, index{index} {}

        /**
         * @brief Compares two iterators for inequality.
         *
         * @param other Iterator to compare with.
         * @return True if the iterators point to different positions.
         */
        bool operator!=(const Iterator& other) const;

        /**
         * @brief Advances the iterator to the next element.
         */
        void operator++();

        /**
         * @brief Dereferences the iterator to access the current TransitionIndex.
         *
         * @return The current TransitionIndex at the iterator position.
         */
        TransitionIndex operator*() const;
    };

    /**
     * @brief Returns an iterator pointing to the first TransitionIndex.
     *
     * @return Iterator at the beginning of the TransitionIndices container.
     */
    Iterator begin() const;

    /**
     * @brief Returns an iterator pointing past the last TransitionIndex.
     *
     * @return Iterator representing the end of the TransitionIndices container.
     */
    Iterator end() const;
};

}  // namespace physics
}  // namespace flashlight
