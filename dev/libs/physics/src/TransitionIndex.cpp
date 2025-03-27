//
// Created by bene on 3/26/25.
//

#include <physics/TransitionIndex.h>

namespace flashlight {
namespace physics {

// @brief Gets the index as a tuple.
std::tuple<index_t, index_t, index_t, index_t> TransitionIndex::asTuple() const {
    return std::make_tuple(transition, kpoint, occupied, unoccupied);
}

/**
 * @brief Compares two indices.
 * @details The operator compares two indices and returns true if they are equal, false otherwise.
 * @param other The other index to compare with.
 * @return True if the indices are equal, false otherwise.
 */
bool TransitionIndex::operator==(const TransitionIndex& other) const {
    return transition == other.transition && kpoint == other.kpoint && occupied == other.occupied &&
           unoccupied == other.unoccupied;
}

/**
 * @brief Compares two indices.
 * @details The operator compares two indices and returns true if they are not equal, false otherwise.
 * @param other The other index to compare with.
 * @return True if the indices are not equal, false otherwise.
 */
bool TransitionIndex::operator!=(const TransitionIndex& other) const { return !(*this == other); }

/**
 * @brief Constructs the transition index list from occupied and unoccupied band indices.
 * @param occupiedBands Set of occupied band indices per k-point.
 * @param unoccupiedBands Set of unoccupied band indices per k-point.
 */
TransitionIndices::TransitionIndices(const ElectronIndices& occupiedBands, const ElectronIndices& unoccupiedBands) {
    ASSERT_TRUE(occupiedBands.NumKpoints() == unoccupiedBands.NumKpoints());
    ASSERT_TRUE(occupiedBands.size() >= 1);
    ASSERT_TRUE(unoccupiedBands.size() >= 1);

    indices.clear();
    numKpoints = occupiedBands.NumKpoints();
    numOccupied = occupiedBands.size();
    numUnoccupied = unoccupiedBands.size();
    offsetOccupied = occupiedBands.Offset();
    offsetUnoccupied = unoccupiedBands.Offset();

    for (index_t kpoint = 0; kpoint < numKpoints; kpoint++) {
        auto occupiedBandsAtKpoint = occupiedBands.atKpoint(kpoint);
        auto unoccupiedBandsAtKpoint = unoccupiedBands.atKpoint(kpoint);
        for (auto itOcc = occupiedBandsAtKpoint.begin(); itOcc != occupiedBandsAtKpoint.end(); ++itOcc) {
            for (auto itUno = unoccupiedBandsAtKpoint.begin(); itUno != unoccupiedBandsAtKpoint.end(); ++itUno) {
                indices.push_back(TransitionIndex(
                    kpoint * numOccupied * numUnoccupied + itOcc.position() * numUnoccupied + itUno.position(), kpoint,
                    (*itOcc).getElectron(), (*itUno).getElectron()));
            }
        }
    }
}

/**
 * @brief Constructs the transition index list from a tuple of occupied and unoccupied band indices.
 * @param electronIndices Tuple containing {occupiedBands, unoccupiedBands}.
 */
TransitionIndices::TransitionIndices(std::tuple<ElectronIndices, ElectronIndices>& occupiedAndUnoccupied) {
    const auto& [occupiedBands, unoccupiedBands] = occupiedAndUnoccupied;
    TransitionIndices(occupiedBands, unoccupiedBands);
}

/** @brief Destructor. */
TransitionIndices::~TransitionIndices() {
    indices.clear();
    numKpoints = 0;
    numOccupied = 0;
    numUnoccupied = 0;
    offsetOccupied = 0;
    offsetUnoccupied = 0;
}

/**
 * @brief Returns the total number of transitions stored.
 * @return Number of transition indices.
 */
index_t TransitionIndices::size() const { return indices.size(); }

/**
 * @brief Access a transition by its index.
 * @param index Global index into the transition list.
 * @return Corresponding TransitionIndex.
 */
TransitionIndex TransitionIndices::operator[](const index_t& index) const {
    ASSERT_TRUE(index >= 0 && index < indices.size());
    return indices.at(index);
}

/**
 * @brief Returns a vector of all transition indices (global).
 * @return Vector of transition indices.
 */
vector_t<index_t> TransitionIndices::getTransitions() const {
    auto transitions = vector_t<index_t>(indices.size());
    for (index_t idx = 0; idx < size(); ++idx) {
        transitions(idx) = indices.at(idx).getTransition();
    }
    return transitions;
}

/**
 * @brief Returns the list of k-point indices involved in the transitions.
 * @return Vector of k-point indices.
 */
vector_t<index_t> TransitionIndices::getKpoints() const {
    auto kpoints = vector_t<index_t>(indices.size());
    for (index_t idx = 0; idx < size(); ++idx) {
        kpoints(idx) = indices.at(idx).getKpoint();
    }
    return kpoints;
}

/**
 * @brief Returns the list of occupied band indices.
 * @return Vector of occupied band indices.
 */
vector_t<index_t> TransitionIndices::getOccBands() const {
    auto bands = vector_t<index_t>(indices.size());
    for (index_t idx = 0; idx < size(); ++idx) {
        bands(idx) = indices.at(idx).getOccupied();
    }
    return bands;
}

/**
 * @brief Returns the list of unoccupied band indices.
 * @return Vector of unoccupied band indices.
 */
vector_t<index_t> TransitionIndices::getUnoBands() const {
    auto bands = vector_t<index_t>(indices.size());
    for (index_t idx = 0; idx < size(); ++idx) {
        bands(idx) = indices.at(idx).getUnoccupied();
    }
    return bands;
}

/**
 * @brief Filters transitions by a specific k-point.
 * @param kpoint The k-point index to filter by.
 * @return A new TransitionIndices object containing only transitions at the given k-point.
 */
TransitionIndices TransitionIndices::atKpoint(const index_t& kpoint) const {
    ASSERT_TRUE(kpoint >= 0);
    ASSERT_TRUE(kpoint < numKpoints);
    TransitionIndices indicesAtKpoint;
    for (auto index : indices) {
        if (index.getKpoint() == kpoint) {
            indicesAtKpoint.indices.push_back(index);
        }
    }
    if (indicesAtKpoint.size() == 0) {
        throw std::runtime_error("No transition indices found for k-point " + std::to_string(kpoint) + ".");
    }
    return indicesAtKpoint;
}

/**
 * @brief Filters transitions by a specific occupied band.
 * @param occupied The occupied band index to filter by.
 * @return A new TransitionIndices object containing only transitions from the given occupied band.
 */
TransitionIndices TransitionIndices::atOccupied(const index_t& occupied) const {
    ASSERT_TRUE(occupied >= 0);
    ASSERT_TRUE(occupied < numOccupied + offsetOccupied);
    TransitionIndices indicesAtOccupied;
    for (auto index : indices) {
        if (index.getOccupied() == occupied) {
            indicesAtOccupied.indices.push_back(index);
        }
    }
    if (indicesAtOccupied.size() == 0) {
        throw std::runtime_error("No transition indices found for occupied band " + std::to_string(occupied) + ".");
    }
    return indicesAtOccupied;
}

/**
 * @brief Filters transitions by a specific unoccupied band.
 * @param unoccupied The unoccupied band index to filter by.
 * @return A new TransitionIndices object containing only transitions to the given unoccupied band.
 */
TransitionIndices TransitionIndices::atUnoccupied(const index_t& unoccupied) const {
    ASSERT_TRUE(unoccupied >= 0);
    ASSERT_TRUE(unoccupied < numUnoccupied + offsetUnoccupied);
    TransitionIndices indicesAtUnoccupied;
    for (auto index : indices) {
        if (index.getUnoccupied() == unoccupied) {
            indicesAtUnoccupied.indices.push_back(index);
        }
    }
    if (indicesAtUnoccupied.size() == 0) {
        throw std::runtime_error("No transition indices found for unoccupied band " + std::to_string(unoccupied) + ".");
    }
    return indicesAtUnoccupied;
}

/**
 * @brief Filters transitions by occupied and unoccupied index.
 * @param transition Index of the transition to isolate.
 * @return A new TransitionIndices object containing only the specified transition.
 */
TransitionIndex TransitionIndices::atOccupiedUnoccupied(const index_t& occupied, const index_t& unoccupied) const {
    ASSERT_TRUE(occupied >= 0);
    ASSERT_TRUE(occupied < numOccupied + offsetOccupied);
    ASSERT_TRUE(unoccupied >= 0);
    ASSERT_TRUE(unoccupied < numUnoccupied + offsetUnoccupied);
    TransitionIndex index;
    bool found = false;
    for (auto jndex : indices) {
        if (jndex.getOccupied() == occupied && jndex.getUnoccupied() == unoccupied) {
            index = jndex;
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("TransitionIndex not found for occupied band" + std::to_string(occupied) +
                                 " and unoccupied band " + std::to_string(unoccupied) + ".");
    }
    return index;
}

/**
 * @brief Filters transitions by transition index (e.g. transition ID).
 * @param transition Index of the transition to isolate.
 * @return A new TransitionIndices object containing only the specified transition.
 */
TransitionIndex TransitionIndices::atTransition(const index_t& transition) const {
    ASSERT_TRUE(transition >= 0);
    ASSERT_TRUE(transition < size());
    TransitionIndex index;
    bool found = false;
    for (auto jndex : indices) {
        if (jndex.getTransition() == transition) {
            index = jndex;
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("ElectronIndex not found for transition " + std::to_string(transition) + ".");
    }
    return index;
}

/**
 * @brief Returns an iterator pointing to the first TransitionIndex.
 *
 * @return Iterator at the beginning of the TransitionIndices container.
 */
TransitionIndices::Iterator TransitionIndices::begin() const { return Iterator(*this, 0); }

/**
 * @brief Returns an iterator pointing past the last TransitionIndex.
 *
 * @return Iterator representing the end of the TransitionIndices container.
 */
TransitionIndices::Iterator TransitionIndices::end() const { return Iterator(*this, size()); }

/**
 * @brief Compares two iterators for inequality.
 *
 * @param other Iterator to compare with.
 * @return True if the iterators point to differe
 */
bool TransitionIndices::Iterator::operator!=(const Iterator& other) const { return index != other.index; }

/**
 * @brief Advances the iterator to the next eleme
 */
void TransitionIndices::Iterator::operator++() { ++index; }

/**
 * @brief Dereferences the iterator to access the
 *
 * @return The current TransitionIndex at the ite
 */
TransitionIndex TransitionIndices::Iterator::operator*() const { return indices[index]; }

}  // namespace physics
}  // namespace flashlight