#include <physics/electrons/ElectronicStates.h>



// Function to convert a boolean mask to a vector of indices
std::vector<Eigen::Index> maskToIndices(const Eigen::Array<bool, Eigen::Dynamic, 1>& mask) {
    std::vector<Eigen::Index> indices;
    for (Eigen::Index i = 0; i < mask.size(); ++i) {
        if (mask[i]) {
            indices.push_back(i);
        }
    }
    return indices;
}

flash::physics::electrons::ElectronicStates::ElectronicStates(
    const Eigen::Vector<std::double_t, Eigen::Dynamic>& energies,
    const Eigen::Vector<std::double_t, Eigen::Dynamic>& occupations,
    const Eigen::Vector<std::size_t, Eigen::Dynamic>& kPointMap,
    const Eigen::Vector<std::size_t, Eigen::Dynamic>& bandMap)
    : energies{energies}, occupations{occupations}, kPointMap{kPointMap}, bandMap{bandMap} {
    if (energies.size() != occupations.size() || energies.size() != kPointMap.size() ||
        energies.size() != bandMap.size()) {
        throw std::invalid_argument("All input vectors must have the same size.");
    }
}

flash::physics::electrons::ElectronicStates::ElectronicStates(const std::string& file) {
    
    H5::H5File fileHandle(file, H5F_ACC_RDONLY);

    // H5::DataSet dataset = fileHandle.openDataSet("energies");
    // energies = data_processing::hdf5::read_vector<std::double_t>(dataset);
    // dataset.close();
    
    // dataset = fileHandle.openDataSet("occupations");
    // occupations = data_processing::hdf5::read_vector<std::double_t>(dataset);
    // dataset.close();

    // dataset = fileHandle.openDataSet("kPointMap");
    // kPointMap = data_processing::hdf5::read_vector<std::size_t>(dataset);
    // dataset.close();

    // dataset = fileHandle.openDataSet("bandMap");
    // bandMap = data_processing::hdf5::read_vector<std::size_t>(dataset);
    // dataset.close();

    fileHandle.close();

    if (energies.size() != occupations.size() || energies.size() != kPointMap.size() ||
        energies.size() != bandMap.size()) {
        throw std::invalid_argument("All input vectors must have the same size.");
    }
}

flash::physics::electrons::ElectronicStates flash::physics::electrons::ElectronicStates::atKPoint(
    const std::size_t& kPoint) {
    auto kPointIndices = maskToIndices(kPointMap.array() == kPoint);
    return ElectronicStates(energies(kPointIndices), occupations(kPointIndices), kPointMap(kPointIndices),
                            bandMap(kPointIndices));
}

flash::physics::electrons::ElectronicStates flash::physics::electrons::ElectronicStates::atBand(
    const std::size_t& band) {
    auto bandIndices = maskToIndices(bandMap.array() == band);
    return ElectronicStates(energies(bandIndices), occupations(bandIndices), kPointMap(bandIndices),
                            bandMap(bandIndices));
}

flash::physics::electrons::ElectronicStates flash::physics::electrons::ElectronicStates::occupied(
    const std::double_t& zeroTolerance) {
    auto occupiedIndices = maskToIndices(occupations.array() > zeroTolerance);
    return ElectronicStates(energies(occupiedIndices), occupations(occupiedIndices), kPointMap(occupiedIndices),
                            bandMap(occupiedIndices));
}

flash::physics::electrons::ElectronicStates flash::physics::electrons::ElectronicStates::unoccupied(
    const std::double_t& zeroTolerance) {
    auto unoccupiedIndices = maskToIndices(occupations.array() <= zeroTolerance);
    return ElectronicStates(energies(unoccupiedIndices), occupations(unoccupiedIndices), kPointMap(unoccupiedIndices),
                            bandMap(unoccupiedIndices));
}

const Eigen::Vector<std::double_t, Eigen::Dynamic>& flash::physics::electrons::ElectronicStates::getEnergies() const {
    return energies;
}

const Eigen::Vector<std::double_t, Eigen::Dynamic>& flash::physics::electrons::ElectronicStates::getOccupations()
    const {
    return occupations;
}

const Eigen::Vector<std::size_t, Eigen::Dynamic>& flash::physics::electrons::ElectronicStates::getKPointMap() const {
    return kPointMap;
}

const Eigen::Vector<std::size_t, Eigen::Dynamic>& flash::physics::electrons::ElectronicStates::getBandMap() const {
    return bandMap;
}

std::size_t flash::physics::electrons::ElectronicStates::size() const { return energies.size(); }

flash::physics::electrons::ElectronicStates::Iterator::Iterator(const ElectronicStates& electronicStates,
                                                                 std::size_t index)
    : electronicStates{electronicStates}, index{index} {}

bool flash::physics::electrons::ElectronicStates::Iterator::operator!=(const Iterator& other) const { return index != other.index; }

void flash::physics::electrons::ElectronicStates::Iterator::operator++() { ++index; }

std::tuple<double, double, std::size_t, std::size_t> flash::physics::electrons::ElectronicStates::Iterator::operator*() const {
    return {electronicStates.energies[index], electronicStates.occupations[index], electronicStates.kPointMap[index],
            electronicStates.bandMap[index]};
}

flash::physics::electrons::ElectronicStates::Iterator flash::physics::electrons::ElectronicStates::begin() const {
    return Iterator(*this, 0);
}

flash::physics::electrons::ElectronicStates::Iterator flash::physics::electrons::ElectronicStates::end() const {
    return Iterator(*this, size());
}