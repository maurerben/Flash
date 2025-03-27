#pragma once

#include <utils/assert.h>
#include <utils/types.h>

#include <Eigen/Dense>
#include <stdexcept>
#include <string>
#include <tuple>

namespace flashlight {
namespace physics {

using namespace utils;

/**
 * @brief Represents a unique electron index within a k-point and band structure.
 *
 * The ElectronIndex class encapsulates an electron's composite index, including its
 * global index, k-point index, and band index. This is useful for systems where
 * electrons are organized by discrete k-points and bands.
 */
class ElectronIndex {
   private:
    index_t electron;  ///< Global electron index.
    index_t kpoint;    ///< k-point index.
    index_t band;      ///< Band index.

   public:
    /// @brief Default constructor.
    ElectronIndex() = default;

    /// @brief Copy constructor.
    ElectronIndex(const ElectronIndex& other) = default;

    /// @brief Copy assignment operator.
    constexpr ElectronIndex& operator=(const ElectronIndex& other) = default;

    /**
     * @brief Constructs an ElectronIndex from electron, k-point, and band indices.
     * @param electron Global electron index.
     * @param kpoint k-point index.
     * @param band Band index.
     */
    ElectronIndex(const index_t& electron, const index_t& kpoint, const index_t& band)
        : electron{electron}, kpoint{kpoint}, band{band} {
        assert(electron >= 0);
        assert(kpoint >= 0);
        assert(band >= 0);
    }

    /// @brief Default destructor.
    ~ElectronIndex() = default;

    /// @brief Returns the global electron index.
    index_t getElectron() const { return electron; }

    /// @brief Returns the k-point index.
    index_t getKpoint() const { return kpoint; }

    /// @brief Returns the band index.
    index_t getBand() const { return band; }

    /// @brief Returns a tuple representation (electron, k-point, band).
    std::tuple<index_t, index_t, index_t> asTuple() const;

    /// @brief Equality comparison operator.
    bool operator==(const ElectronIndex& other) const;

    /// @brief Inequality comparison operator.
    bool operator!=(const ElectronIndex& other) const;

    /**
     * @brief Checks if the electron is occupied based on a given occupation vector.
     * @param occupations A vector of occupation values.
     * @param zeroTolerance A threshold below which occupation is considered zero.
     * @return True if the electron is occupied.
     */
    bool isOccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
                    const std::double_t& zeroTolerance) const;

    /**
     * @brief Checks if the electron is unoccupied based on a given occupation vector.
     * @param occupations A vector of occupation values.
     * @param zeroTolerance A threshold below which occupation is considered zero.
     * @return True if the electron is unoccupied.
     */
    bool isUnoccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
                      const std::double_t& zeroTolerance) const;
};

/**
 * @brief Container for managing a list of electron indices across k-points and bands.
 *
 * Provides utilities for access, filtering, and occupation-based separation of electrons
 * in a system defined by discrete k-points and bands.
 */
class ElectronIndices {
   private:
    std::vector<ElectronIndex> indices;  ///< Collection of all electron indices.
    index_t numKpoints = 0;              ///< Total number of k-points.
    index_t numBands = 0;                ///< Total number of bands.
    index_t offset = 0;                  ///< Offset

   public:
    /// @brief Default constructor.
    ElectronIndices() = default;

    /// @brief Copy constructor.
    ElectronIndices(const ElectronIndices& other) = default;

    /// @brief Copy assignment operator.
    ElectronIndices& operator=(const ElectronIndices& other) = default;

    /**
     * @brief Constructs ElectronIndices for a grid of k-points and bands.
     *
     * The indices are ordered such that all bands of a k-point are grouped consecutively.
     * @param numKpoints Number of k-points.
     * @param numBands Number of bands.
     * @param offset Offset of the band indices
     */
    ElectronIndices(const index_t& numKpoints, const index_t& numBands, const index_t& offset = 0);

    /// @brief Destructor.
    ~ElectronIndices();

    /// @brief Returns the total number of electron indices.
    index_t size() const;

    /// @brief Returns the number of k-points.
    index_t NumKpoints() const { return numKpoints; }

    /// @brief Returns the number of bands.
    index_t NumBands() const { return numBands; }

    /// @brief Returns offset of electron indices.
    index_t Offset() const { return offset; }

    /// @brief Returns the ElectronIndex at a given linear index.
    ElectronIndex operator[](const index_t& index) const;

    /// @brief Extracts all global electron indices into a vector.
    vector_t<index_t> getElectrons() const;

    /// @brief Extracts all k-point indices into a vector.
    vector_t<index_t> getKpoints() const;

    /// @brief Extracts all band indices into a vector.
    vector_t<index_t> getBands() const;

    /**
     * @brief Filters and returns indices at a specific k-point.
     * @param kPoint Index of the k-point to filter.
     * @return A new ElectronIndices object containing matching entries.
     */
    ElectronIndices atKpoint(const index_t& kPoint) const;

    /**
     * @brief Filters and returns indices at a specific band.
     * @param band Index of the band to filter.
     * @return A new ElectronIndices object containing matching entries.
     */
    ElectronIndices atBand(const index_t& band) const;

    /**
     * @brief Retrieves the ElectronIndex for a specific (k-point, band) combination.
     * @param kpoint k-point index.
     * @param band Band index.
     * @return ElectronIndex corresponding to the specified coordinates.
     */
    ElectronIndex atKpointBand(const index_t& kpoint, const index_t& band) const;

    /**
     * @brief Retrieves the ElectronIndex by its global electron index.
     * @param electron Global electron index.
     * @return ElectronIndex object at the specified position.
     */
    ElectronIndex atElectron(const index_t& electron) const;

    /**
     * @brief Splits the collection into occupied and unoccupied indices.
     * @param occupations Vector of occupation values.
     * @param zeroTolerance Threshold for determining occupancy (default = 1e-6).
     * @return Tuple of two ElectronIndices: (occupied, unoccupied).
     */
    std::pair<ElectronIndices, ElectronIndices> separateByOccupation(
        const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
        const std::double_t& zeroTolerance = 1e-6) const;

    /**
     * @brief Iterator class for traversing ElectronIndices.
     */
    class Iterator {
       private:
        const ElectronIndices& indices;  ///< Reference to the ElectronIndices container.
        index_t index;                   ///< Current position.

       public:
        /// @brief Constructs an iterator at a specific position.
        Iterator(const ElectronIndices& indices, index_t index) : indices{indices}, index{index} {}

        /// @brief Inequality comparison.
        bool operator!=(const Iterator& other) const;

        /// @brief Advances to the next index.
        void operator++();

        /// @brief Dereferences the iterator.
        ElectronIndex operator*() const;

        /// @brief Returns the current index position of the iterator.
        index_t position() const { return index; }
    };

    /// @brief Returns an iterator to the beginning.
    Iterator begin() const;qwqwe

    /// @brief Returns an iterator to the end (one past the last element).
    Iterator end() const;
};

}  // namespace physics
}  // namespace flashlight
