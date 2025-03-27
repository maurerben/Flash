#include <physics/ElectronIndex.h>

using namespace flashlight::utils;

namespace flashlight {
namespace physics {

// Get index as tuple
std::tuple<index_t, index_t, index_t> ElectronIndex::asTuple() const { return std::make_tuple(electron, kpoint, band); }

// Equality operator
bool ElectronIndex::operator==(const ElectronIndex& other) const {
    return electron == other.electron && kpoint == other.kpoint && band == other.band;
}

// Inequality operator
bool ElectronIndex::operator!=(const ElectronIndex& other) const { return !(*this == other); }

// Check if electron is occupied
bool ElectronIndex::isOccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
                               const std::double_t& zeroTolerance) const {
    return occupations[electron] > zeroTolerance;
}

// Check if electron is unoccupied
bool ElectronIndex::isUnoccupied(const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
                                 const std::double_t& zeroTolerance) const {
    return occupations[electron] <= zeroTolerance;
}

// Constructor with parameters
ElectronIndices::ElectronIndices(const index_t& numKpoints, const index_t& numBands, const index_t& offset) {
    ASSERT_TRUE(numKpoints > index_t(0));
    ASSERT_TRUE(numBands > index_t(0));

    this->numKpoints = numKpoints;
    this->numBands = numBands;
    this->offset = offset;

    for (index_t kpoint = 0; kpoint < numKpoints; ++kpoint) {
        for (index_t band = 0; band < numBands; ++band) {
            indices.push_back(ElectronIndex(kpoint * numBands + band + offset, kpoint, band));
        }
    }
}

// Destructor
ElectronIndices::~ElectronIndices() {
    indices.clear();
    numKpoints = 0;
    numBands = 0;
}

// Get number of indices
index_t ElectronIndices::size() const { return indices.size(); }

// Get index at specific position
ElectronIndex ElectronIndices::operator[](const index_t& index) const {
    ASSERT_TRUE(index >= 0);
    ASSERT_TRUE(index < indices.size());
    return indices[index];
}

// Get electron indices as Eigen::Vector
vector_t<index_t> ElectronIndices::getElectrons() const {
    vector_t<index_t> electrons(size());
    for (index_t i = 0; i < size(); ++i) {
        electrons[i] = indices[i].getElectron();
    }
    return electrons;
}

// Get __k__-point indices as Eigen::Vector
vector_t<index_t> ElectronIndices::getKpoints() const {
    vector_t<index_t> kpoints(size());
    for (index_t i = 0; i < size(); ++i) {
        kpoints[i] = indices[i].getKpoint();
    }
    return kpoints;
}

// Get band indices as Eigen::Vector
vector_t<index_t> ElectronIndices::getBands() const {
    vector_t<index_t> bands(size());
    for (index_t i = 0; i < size(); ++i) {
        bands[i] = indices[i].getBand();
    }
    return bands;
}

// Filter indices at specific __k__-point
ElectronIndices ElectronIndices::atKpoint(const index_t& kpoint) const {
    ASSERT_TRUE(kpoint >= 0);
    ASSERT_TRUE(kpoint < numKpoints);
    ElectronIndices indicesAtKpoint;
    for (auto index : indices) {
        if (index.getKpoint() == kpoint) {
            indicesAtKpoint.indices.push_back(index);
        }
    }
    if (indicesAtKpoint.size() == 0) {
        throw std::runtime_error("No electron indices found for k-point " + std::to_string(kpoint) + ".");
    }
    return indicesAtKpoint;
}

// Filter indices at specific band
ElectronIndices ElectronIndices::atBand(const index_t& band) const {
    ASSERT_TRUE(band >= 0);
    ASSERT_TRUE(band < numBands);
    ElectronIndices indicesAtBand;
    for (auto index : indices) {
        if (index.getBand() == band) {
            indicesAtBand.indices.push_back(index);
        }
    }
    if (indicesAtBand.size() == 0) {
        throw std::runtime_error("No indices found for band " + std::to_string(band) + ".");
    }
    return indicesAtBand;
}

// Filter index of electron with specified band and __k__-point
ElectronIndex ElectronIndices::atKpointBand(const index_t& kpoint, const index_t& band) const {
    ASSERT_TRUE(kpoint >= 0);
    ASSERT_TRUE(kpoint < numKpoints);
    ASSERT_TRUE(band >= 0);
    ASSERT_TRUE(kpoint < numBands);
    ElectronIndex index;
    bool found = false;
    for (auto jndex : indices) {
        if (jndex.getKpoint() == kpoint && jndex.getBand() == band) {
            index = jndex;
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("ElectronIndex not found for k-point " + std::to_string(kpoint) + " and band " +
                                 std::to_string(band) + ".");
    }
    return index;
}

// Filter indices at specific electron index
ElectronIndex ElectronIndices::atElectron(const index_t& electron) const {
    ASSERT_TRUE(electron >= 0);
    ASSERT_TRUE(electron < size() + offset);
    ElectronIndex index;
    bool found = false;
    for (auto jndex : indices) {
        if (jndex.getElectron() == electron) {
            index = jndex;
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("ElectronIndex not found at electron " + std::to_string(electron) + ".");
    }
    return index;
}

// Separate occupied and unoccupied indices
std::pair<ElectronIndices, ElectronIndices> ElectronIndices::separateByOccupation(
    const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations, const std::double_t& zeroTolerance) const {
    ElectronIndices occupiedIndices;
    ElectronIndices unoccupiedIndices;
    for (index_t i = 0; i < size(); ++i) {
        if (indices[i].isOccupied(occupations, zeroTolerance)) {
            occupiedIndices.indices.push_back(indices[i]);
        } else {
            unoccupiedIndices.indices.push_back(indices[i]);
        }
    }
    assertTrue(occupiedIndices.size() > 0, "ElectronIndices::separateByOccupation: No occupied states found.");
    assertTrue(unoccupiedIndices.size() > 0, "ElectronIndices::separateByOccupation: No unoccupied states found.");

    return std::pair(occupiedIndices, unoccupiedIndices);
}

// Iterator begin
ElectronIndices::Iterator ElectronIndices::begin() const { return Iterator(*this, 0); }

// Iterator end
ElectronIndices::Iterator ElectronIndices::end() const { return Iterator(*this, size()); }

// Iterator inequality operator
bool ElectronIndices::Iterator::operator!=(const Iterator& other) const { return index != other.index; }

// Iterator increment operator
void ElectronIndices::Iterator::operator++() { ++index; }

// Iterator dereference operator
ElectronIndex ElectronIndices::Iterator::operator*() const { return indices[index]; }

}  // namespace physics
}  // namespace flashlight