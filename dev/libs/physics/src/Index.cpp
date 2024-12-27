#include <physics/electrons/Index.h>
#include <stdexcept>

namespace flash {
namespace physics {
namespace electrons {

// Get index as tuple
std::tuple<Eigen::Index, Eigen::Index, Eigen::Index> Index::asTuple() const {
    return std::make_tuple(electron, kpoint, band);
}

// Equality operator
bool Index::operator==(const Index& other) const {
    return electron == other.electron &&
           kpoint == other.kpoint &&
           band == other.band;
}

// Inequality operator
bool Index::operator!=(const Index& other) const {
    return !(*this == other);
}

// Check if electron is occupied
bool Index::isOccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations, const std::double_t& zeroTolerance) const {
    return occupations[electron] > zeroTolerance;
}

// Check if electron is unoccupied
bool Index::isUnoccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations, const std::double_t& zeroTolerance) const {
    return occupations[electron] <= zeroTolerance;
}

// Default constructor for Indices
Indices::Indices() = default;

// Constructor with parameters
Indices::Indices(const Eigen::Index& NumberOfKpoints, const Eigen::Index& NumberOfBands) {
    for (Eigen::Index kpoint = 0; kpoint < NumberOfKpoints; ++kpoint) {
        for (Eigen::Index band = 0; band < NumberOfBands; ++band) {
            indices.push_back(Index(kpoint * NumberOfBands + band, kpoint, band));
        }
    }
}

// Destructor
Indices::~Indices() {
    indices.clear();
}

// Get number of indices
Eigen::Index Indices::size() const {
    return indices.size();
}

// Get index at specific position
Index Indices::operator[](const Eigen::Index& index) const {
    return indices[index];
}

// Get electron indices as Eigen::Vector
Eigen::Vector<Eigen::Index, Eigen::Dynamic> Indices::getElectrons() const {
    Eigen::Vector<Eigen::Index, Eigen::Dynamic> electrons(size());
    for (Eigen::Index i = 0; i < size(); ++i) {
        electrons[i] = indices[i].getElectron();
    }
    return electrons;
}

// Get k-point indices as Eigen::Vector
Eigen::Vector<Eigen::Index, Eigen::Dynamic> Indices::getKpoints() const {
    Eigen::Vector<Eigen::Index, Eigen::Dynamic> kpoints(size());
    for (Eigen::Index i = 0; i < size(); ++i) {
        kpoints[i] = indices[i].getKpoint();
    }
    return kpoints;
}

// Get band indices as Eigen::Vector
Eigen::Vector<Eigen::Index, Eigen::Dynamic> Indices::getBands() const {
    Eigen::Vector<Eigen::Index, Eigen::Dynamic> bands(size());
    for (Eigen::Index i = 0; i < size(); ++i) {
        bands[i] = indices[i].getBand();
    }
    return bands;
}

// Filter indices at specific k-point
Indices Indices::atKpoint(const Eigen::Index& kPoint) const {
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

// Filter indices at specific band
Indices Indices::atBand(const Eigen::Index& band) const {
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

// Filter index of electron with specified band and k-point
Index Indices::atKpointBand(const Eigen::Index& kpoint, const Eigen::Index& band) const {
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

// Filter indices at specific electron index
Index Indices::atElectron(const Eigen::Index& electron) const {
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

// Separate occupied and unoccupied indices
std::tuple<Indices, Indices> Indices::separateByOccupation(
    const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
    const std::double_t& zeroTolerance) const {
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

// Iterator begin
Indices::Iterator Indices::begin() const {
    return Iterator(*this, 0);
}

// Iterator end
Indices::Iterator Indices::end() const {
    return Iterator(*this, size());
}

// Iterator constructor
Indices::Iterator::Iterator(const Indices& indices, Eigen::Index index) : indices{indices}, index{index} {}

// Iterator inequality operator
bool Indices::Iterator::operator!=(const Iterator& other) const {
    return index != other.index;
}

// Iterator increment operator
void Indices::Iterator::operator++() {
    ++index;
}

// Iterator dereference operator
Index Indices::Iterator::operator*() const {
    return indices[index];
}

} // namespace electrons
} // namespace physics
} // namespace flash