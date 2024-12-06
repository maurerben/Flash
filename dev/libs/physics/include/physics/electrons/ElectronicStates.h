#pragma once

#include <physics/electrons/ElectronicStates.h>

#include <data_processing/hdf5.h>
#include <Eigen/Dense>
#include <cstdint>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>

namespace flash {
namespace physics {
namespace electrons {

/**
 * @class ElectronicStates
 * @brief Represents the electronic states in a physical system.
 *
 * This class encapsulates the energies, occupations, k-point map, and band map of electronic states.
 */
class ElectronicStates {
   private:
    Eigen::Vector<std::double_t, Eigen::Dynamic> energies;
    Eigen::Vector<std::double_t, Eigen::Dynamic> occupations;
    Eigen::Vector<std::size_t, Eigen::Dynamic> kPointMap;
    Eigen::Vector<std::size_t, Eigen::Dynamic> bandMap;

   public:
    // ElectronicStates(const config::nodes::ElectronicStates& config) : config_{config} {}

    /**
     * @brief Constructs an ElectronicStates object.
     * 
     * @param energies A vector of energy values.
     * @param occupations A vector of occupation numbers.
     * @param kPointMap A vector mapping k-points.
     * @param bandMap A vector mapping bands.
     */
    ElectronicStates(const Eigen::Vector<std::double_t, Eigen::Dynamic>& energies,
                     const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
                     const Eigen::Vector<std::size_t, Eigen::Dynamic>& kPointMap,
                     const Eigen::Vector<std::size_t, Eigen::Dynamic>& bandMap);

    /**
     * @brief Constructs an ElectronicStates object from an HDF5 file.
     * 
     * @param file The HDF5 file to read the electronic states from.
     */
    ElectronicStates(const std::string& file);
    

    /**
     * @brief Filters the electronic states at a specific k-point.
     * 
     * @param kPoint The k-point index to filter by.
     * @return A new ElectronicStates object containing only the states at the specified k-point.
     */
    ElectronicStates atKPoint(const std::size_t& kPoint);

    /**
     * @brief Filters the electronic states at a specific band.
     * 
     * @param band The band index to filter by.
     * @return A new ElectronicStates object containing only the states at the specified band.
     */
    ElectronicStates atBand(const std::size_t& band);

    /**
     * @brief Filters the occupied electronic states.
     * 
     * @param zeroTolerance The tolerance value to consider a state as occupied.
     * @return A new ElectronicStates object containing only the occupied states.
     */
    ElectronicStates occupied(const std::double_t& zeroTolerance = 1e-6);

    /**
     * @brief Filters the unoccupied electronic states.
     * 
     * @param zeroTolerance The tolerance value to consider a state as unoccupied.
     * @return A new ElectronicStates object containing only the unoccupied states.
     */
    ElectronicStates unoccupied(const std::double_t& zeroTolerance = 1e-6);

    /**
     * @brief Gets the vector of energy values.
     * 
     * @return A constant reference to the vector of energy values.
     */
    const Eigen::Vector<std::double_t, Eigen::Dynamic>& getEnergies() const;

    /**
     * @brief Gets the vector of occupation numbers.
     * 
     * @return A constant reference to the vector of occupation numbers.
     */
    const Eigen::Vector<std::double_t, Eigen::Dynamic>& getOccupations() const;

    /**
     * @brief Gets the vector mapping k-points.
     * 
     * @return A constant reference to the vector mapping k-points.
     */
    const Eigen::Vector<std::size_t, Eigen::Dynamic>& getKPointMap() const;

    /**
     * @brief Gets the vector mapping bands.
     * 
     * @return A constant reference to the vector mapping bands.
     */
    const Eigen::Vector<std::size_t, Eigen::Dynamic>& getBandMap() const;

    /**
     * @brief Gets the number of electronic states.
     * 
     * @return The number of electronic states.
     */
    std::size_t size() const;

    /**
     * @class Iterator
     * @brief An iterator for traversing the electronic states.
     */
    class Iterator {
       private:
        const ElectronicStates& electronicStates;
        std::size_t index;

       public:
        /**
         * @brief Constructs an iterator for the ElectronicStates object.
         * 
         * @param electronicStates The ElectronicStates object to iterate over.
         * @param index The starting index for the iterator.
         */
        Iterator(const ElectronicStates& electronicStates, std::size_t index);

        /**
         * @brief Checks if two iterators are not equal.
         * 
         * @param other The other iterator to compare with.
         * @return True if the iterators are not equal, false otherwise.
         */
        bool operator!=(const Iterator& other) const;

        /**
         * @brief Advances the iterator to the next element.
         */
        void operator++();

        /**
         * @brief Dereferences the iterator to access the current element.
         * 
         * @return A tuple containing the energy, occupation, k-point index, and band index of the current element.
         */
        std::tuple<double, double, std::size_t, std::size_t> operator*() const;
    };

    /**
     * @brief Gets an iterator to the beginning of the electronic states.
     * 
     * @return An iterator to the beginning of the electronic states.
     */
    Iterator begin() const;

    /**
     * @brief Gets an iterator to the end of the electronic states.
     * 
     * @return An iterator to the end of the electronic states.
     */
    Iterator end() const;
};
}  // namespace electrons
}  // namespace physics
}  // namespace flash