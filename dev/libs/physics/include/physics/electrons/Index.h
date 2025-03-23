#pragma once

#include <utils/types.h>
#include <string>
#include <Eigen/Dense>
#include <tuple>

using namespace flash::utils;

namespace flash{
namespace physics{
namespace electrons{

/**
 * @brief A class to store an electron index.
 * @details The Index class stores an index for a given k-point and band. The index is stored as a composite index
 *          in the complete list of electrons, as well as the k-point and band indices.
 */
class Index {
    private:
        index_t electron; // electron index
        index_t kpoint; // kpoint index
        index_t band; // band index

    public:
        // @brief Default constructor
        Index() = default; // Default constructor


        // @brief Copy constructor
        Index(const Index& other) = default; // Copy constructor


        // @brief Copy assignment operator
        constexpr Index& operator=(const Index& other) = default; // Copy assignment operator


        /**
         * @brief Constructs an Index object.
         * @details The constructor creates an index for a given electron, k-point, and band.
         * @param electron The electron index.
         * @param kpoint The k-point index.
         * @param band The band index.
         */
        Index(const index_t& electron, const index_t& kpoint, const index_t& band)
            : electron{electron}, kpoint{kpoint}, band{band} {}


        ~Index() = default; // Destructor

        // @brief Gets the electron index.
        index_t getElectron() const {return electron;}

        // @brief Gets the k-point index.
        index_t getKpoint() const {return kpoint;}

        // @brief Gets the band index.
        index_t getBand() const {return band;}

        // @brief Gets the index as a tuple.
        std::tuple<index_t, index_t, index_t> asTuple() const;

        /**
         * @brief Compares two indices.
         * @details The operator compares two indices and returns true if they are equal, false otherwise.
         * @param other The other index to compare with.
         * @return True if the indices are equal, false otherwise.
         */
        bool operator == (const Index& other) const;

        /**
         * @brief Compares two indices.
         * @details The operator compares two indices and returns true if they are not equal, false otherwise.
         * @param other The other index to compare with.
         * @return True if the indices are not equal, false otherwise.
         */
        bool operator != (const Index& other) const;

        /**
         * @brief Checks if the electron is occupied.
         * @param occupations The vector of occupations.
         * @param zeroTolerance The zero tolerance for the occupation.
         * @return True if the electron is occupied, false otherwise.
         */
        bool isOccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations, const std::double_t& zeroTolerance) const;


        /**
         * @brief Checks if the electron is unoccupied.
         * @param occupations The vector of occupations.
         * @param zeroTolerance The zero tolerance for the occupation.
         * @return True if the electron is unoccupied, false otherwise.
         */
        bool isUnoccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations, const std::double_t& zeroTolerance) const;

};

/**
 * @brief A class to store a list of electron indices.
 * @details The Indices class stores a list of indices for a given number of k-points and bands. The indices are
 *          stored in the order of k-points and bands, so that the first band of the first k-point is first, the
 *          second band of the first k-point is second, and so on.
 */
class Indices {
    private:
         // @brief A vector of indices.
         std::vector<Index> indices;

    public:
         // @brief Default constructor.
        Indices(){}; // Default constructor

        /**
         * @brief Constructs an Indices object.
         * @details The constructor creates a list of indices for a given number of k-points and bands. The indices are
         *          stored in the order of k-points and bands, so that the first band of the first k-point is first, the
         *          second band of the first k-point is second, and so on.
         * @param NumberOfKpoints The number of k-points.
         * @param NumberOfBands The number of bands.
         */
        Indices(const index_t& NumberOfKpoints, const index_t& NumberOfBands);

        // @brief Destructor
        ~Indices();

        // @brief Gets the number of indices.
        index_t size() const;

        // @brief Gets the index at a specific position.
        Index operator[](const index_t& index) const;

        // @brief Gets the electron indices as a Eigen::Vector.
        vector_t<index_t> getElectrons() const;

        // @brief Gets the k-point indices as a Eigen::Vector.
        vector_t<index_t> getKpoints() const;

        // @brief Gets the band indices as a Eigen::Vector.
        vector_t<index_t> getBands() const;

        /**
         * @brief Filters the indices at a specific k-point.
         * @param kPoint The k-point index to filter by.
         * @return A new Indices object containing only the indices at the specified k-point.
         */
        Indices atKpoint(const index_t& kPoint) const;

        /**
         * @brief Filters the indices at a specific band.
         * @param band The band index to filter by.
         * @return A new Indices object containing only the indices at the specified band.
         */
        Indices atBand(const index_t& band) const;

        /**
         * @brief Filters the index of the electron with specified band and k-point.
         * @param kpoint The k-point index to filter by.
         * @param band The band index to filter by.
         * @return The Index object containing of the specified electron.
         */
        Index atKpointBand(const index_t& kpoint, const index_t& band) const;

        /**
         * @brief Filters the indices at a specific electron index.
         * @param electron The electron index to filter by.
         * @return The Index object containing of the specified electron.
         */
        Index atElectron(const index_t& electron) const;

        /**
         * @brief Filters the occupied and unoccupied indices.
         * @param occupations The vector of occupation numbers.
         * @param zeroTolerance The tolerance value to consider a state as occupied.
         * @return A tuple containing the occupied and unoccupied indices.
         */
        std::tuple<Indices, Indices> separateByOccupation(
            const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
            const std::double_t& zeroTolerance = 1e-6) const;

    /**
     * @brief An iterator for traversing the indices.
     */
    class Iterator {
        private:
            const Indices& indices;
            index_t index;

        public:
            // @brief Constructs an iterator for the Indices object.
            Iterator(const Indices& indices, index_t index) : indices{indices}, index{index} {}

            // @brief Checks if two iterators are not equal.
            bool operator!=(const Iterator& other) const;

            // @brief Advances the iterator to the next element.
            void operator++();

            // @brief Dereferences the iterator to access the current element.
            Index operator*() const;
    };

    // @brief Gets the begin iterator.
    Iterator begin() const;

    // @brief Gets the end iterator.
    Iterator end() const;

};

} // electrons
} // physics
} // flash