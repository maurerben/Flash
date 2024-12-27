#pragma once

#include <string>
#include <Eigen/Dense>
#include <tuple>

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
        Eigen::Index electron; // electron index
        Eigen::Index kpoint; // kpoint index
        Eigen::Index band; // band index

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
        Index(const Eigen::Index& electron, const Eigen::Index& kpoint, const Eigen::Index& band)
            : electron{electron}, kpoint{kpoint}, band{band} {}


        ~Index() = default; // Destructor

        // @brief Gets the electron index.
        Eigen::Index getElectron() const {return electron;}

        // @brief Gets the k-point index.
        Eigen::Index getKpoint() const {return kpoint;}

        // @brief Gets the band index.
        Eigen::Index getBand() const {return band;}

        // @brief Gets the index as a tuple.
        std::tuple<Eigen::Index, Eigen::Index, Eigen::Index> asTuple() const {
            return std::make_tuple(electron, kpoint, band);
        }

        /**
         * @brief Compares two indices.
         * @details The operator compares two indices and returns true if they are equal, false otherwise.
         * @param other The other index to compare with.
         * @return True if the indices are equal, false otherwise.
         */
        bool operator == (const Index& other) const {
            return electron == other.electron &&
                   kpoint == other.kpoint &&
                   band == other.band;
        }

        /**
         * @brief Compares two indices.
         * @details The operator compares two indices and returns true if they are not equal, false otherwise.
         * @param other The other index to compare with.
         * @return True if the indices are not equal, false otherwise.
         */
        bool operator != (const Index& other) const {
            return !(*this == other);
        }

        /**
         * @brief Checks if the electron is occupied.
         * @param occupations The vector of occupations.
         * @param zeroTolerance The zero tolerance for the occupation.
         * @return True if the electron is occupied, false otherwise.
         */
        bool isOccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations, const std::double_t& zeroTolerance) const {
            return occupations[electron] > zeroTolerance;
        }

        /**
         * @brief Checks if the electron is unoccupied.
         * @param occupations The vector of occupations.
         * @param zeroTolerance The zero tolerance for the occupation.
         * @return True if the electron is unoccupied, false otherwise.
         */
        bool isUnoccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations, const std::double_t& zeroTolerance) const {
            return occupations[electron] <= zeroTolerance;
        }

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
        Indices(const Eigen::Index& NumberOfKpoints, const Eigen::Index& NumberOfBands) {
            for (Eigen::Index kpoint = 0; kpoint < NumberOfKpoints; ++kpoint) {
                for (Eigen::Index band = 0; band < NumberOfBands; ++band) {
                    indices.push_back(Index(kpoint * NumberOfBands + band, kpoint, band));
                }
            }
        }

        // @brief Destructor
        ~Indices() {
            indices.clear();
        };

        // @brief Gets the number of indices.
        Eigen::Index size() const {return indices.size();}

        // @brief Gets the index at a specific position.
        Index operator[](const Eigen::Index& index) const {return indices[index];}

        // @brief Gets the electron indices as a Eigen::Vector.
        Eigen::Vector<Eigen::Index, Eigen::Dynamic> getElectrons() const {
            Eigen::Vector<Eigen::Index, Eigen::Dynamic> electrons(size());
            for (Eigen::Index i = 0; i < size(); ++i) {
                electrons[i] = indices[i].getElectron();
            }
            return electrons;
        }

        // @brief Gets the k-point indices as a Eigen::Vector.
        Eigen::Vector<Eigen::Index, Eigen::Dynamic> getKpoints() const {
            Eigen::Vector<Eigen::Index, Eigen::Dynamic> kpoints(size());
            for (Eigen::Index i = 0; i < size(); ++i) {
                kpoints[i] = indices[i].getKpoint();
            }
            return kpoints;
        }

        // @brief Gets the band indices as a Eigen::Vector.
        Eigen::Vector<Eigen::Index, Eigen::Dynamic> getBands() const {
            Eigen::Vector<Eigen::Index, Eigen::Dynamic> bands(size());
            for (Eigen::Index i = 0; i < size(); ++i) {
                bands[i] = indices[i].getBand();
            }
            return bands;
        }

        /**
         * @brief Filters the indices at a specific k-point.
         * @param kPoint The k-point index to filter by.
         * @return A new Indices object containing only the indices at the specified k-point.
         */
        Indices atKpoint(const Eigen::Index& kPoint) const {
            Indices IndicesAtKpoint;
            for (auto index : indices) {
                if (index.getKpoint() == kPoint) {
                    IndicesAtKpoint.indices.push_back(index);
                }
            }
            if (IndicesAtKpoint.size() == 0) {
                throw std::runtime_error("No indices found for k-point.");
            }
            return IndicesAtKpoint;
        }

        /**
         * @brief Filters the indices at a specific band.
         * @param band The band index to filter by.
         * @return A new Indices object containing only the indices at the specified band.
         */
        Indices atBand(const Eigen::Index& band) const {
            Indices IndicesAtBand;
            for (auto index : indices) {
                if (index.getBand() == band) {
                    IndicesAtBand.indices.push_back(index);
                }
            }
            if (IndicesAtBand.size() == 0) {
                throw std::runtime_error("No indices found for band.");
            }
            return IndicesAtBand;
        }

        /**
         * @brief Filters the index of the electron with specified band and k-point.
         * @param kpoint The k-point index to filter by.
         * @param band The band index to filter by.
         * @return The Index object containing of the specified electron.
         */
        Index atKpointBand(const Eigen::Index& kpoint, const Eigen::Index& band) const {
            Index index;
            bool found = false;
            for (auto jndex : indices) {
                if (jndex.getKpoint() == kpoint && jndex.getBand() == band) {
                    index = jndex;
                    found = true;
                    break;
                }
            }
            if (!found) {
                throw std::runtime_error("Index not found at kpoint and band.");
            }
            return index;
        }

        /**
         * @brief Filters the indices at a specific electron index.
         * @param electron The electron index to filter by.
         * @return The Index object containing of the specified electron.
         */
        Index atElectron(const Eigen::Index& electron) const {
            Index index;
            bool found = false;
            for (auto jndex : indices) {
                if (jndex.getElectron() == electron) {
                    index = jndex;
                    found = true;
                    break;
                }
            }
            if (!found) {
                throw std::runtime_error("Index not found at electron.");
            }
            return index;
        }

        /**
         * @brief Filters the occupied and unoccupied indices.
         * @param occupations The vector of occupation numbers.
         * @param zeroTolerance The tolerance value to consider a state as occupied.
         * @return A tuple containing the occupied and unoccupied indices.
         */
        std::tuple<Indices, Indices> separateByOccupation(
            const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
            const std::double_t& zeroTolerance = 1e-6) const {
            Indices occupiedIndices;
            Indices unoccupiedIndices;
            for (Eigen::Index i = 0; i < size(); ++i) {
                if (indices[i].isOccupied(occupations, zeroTolerance)) {
                    occupiedIndices.indices.push_back(indices[i]);
                } else {
                    unoccupiedIndices.indices.push_back(indices[i]);
                }
            }
            if (occupiedIndices.size() == 0) {
                throw std::runtime_error("No occupied indices found.");
            }
            if (unoccupiedIndices.size() == 0) {
                throw std::runtime_error("No unoccupied indices found.");
            }
            return std::make_tuple(occupiedIndices, unoccupiedIndices);
        }


    class Iterator {
        private:
            const Indices& indices;
            Eigen::Index index;

        public:
            Iterator(const Indices& indices, Eigen::Index index) : indices{indices}, index{index} {}
            bool operator!=(const Iterator& other) const {return index != other.index;}
            void operator++() {++index;}
            Index operator*() const {return indices[index];}
    };

    Iterator begin() const {return Iterator(*this, 0);}
    Iterator end() const {return Iterator(*this, size());}



};

} // electrons
} // physics
} // flash