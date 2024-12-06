#define CATCH_PHYSICS_MAIN
#include <physics/electrons/ElectronicStates.h>
#include <catch2/catch_all.hpp>
#include <Eigen/Dense>
#include <cstdint>

namespace fpe = flash::physics::electrons;

TEST_CASE("Test flash::physics::electrons::ElectronicStates class") {

    Eigen::Vector<std::double_t, 6> energies;
    energies << 1.0, 1.1, 2.0, 2.1, 3.0, 3.1;

    Eigen::Vector<std::double_t, 6> occupations;
    occupations << 1.0, 1.0, 1.0, 0.0, 0.0, 0.0;
    
    Eigen::Vector<std::size_t, 6> kPointMap;
    kPointMap << 1, 2, 1, 2, 1, 2;
    
    Eigen::Vector<std::size_t, 6> bandMap;
    bandMap << 1, 1, 2, 2, 3, 3;
    
    fpe::ElectronicStates electronicStates(energies, occupations, kPointMap, bandMap);

    SECTION("Test constructor") {
        REQUIRE(electronicStates.getEnergies() == energies);
        REQUIRE(electronicStates.getOccupations() == occupations);
        REQUIRE(electronicStates.getKPointMap() == kPointMap);  
        REQUIRE(electronicStates.getBandMap() == bandMap);
    }
    

    SECTION("Test atKPoint") {
        std::vector<Eigen::Index> indices = {0, 2, 4};
        auto electronicStatesAtKPoint1 = electronicStates.atKPoint(1);
        REQUIRE(electronicStatesAtKPoint1.getEnergies() == energies(indices));
        REQUIRE(electronicStatesAtKPoint1.getOccupations() == occupations(indices));
        REQUIRE(electronicStatesAtKPoint1.getKPointMap() == kPointMap(indices));
        REQUIRE(electronicStatesAtKPoint1.getBandMap() == bandMap(indices));
    }

    SECTION("Test atBand") {
        std::vector<Eigen::Index> indices = {2, 3};
        auto electronicStatesAtBand1 = electronicStates.atBand(2);
        REQUIRE(electronicStatesAtBand1.getEnergies() == energies(indices));
        REQUIRE(electronicStatesAtBand1.getOccupations() == occupations(indices));
        REQUIRE(electronicStatesAtBand1.getKPointMap() == kPointMap(indices));
        REQUIRE(electronicStatesAtBand1.getBandMap() == bandMap(indices));
    }

    SECTION("Test occupied") {
        std::vector<Eigen::Index> indices = {0, 1, 2};
        auto electronicStatesOccupied = electronicStates.occupied();
        REQUIRE(electronicStatesOccupied.getEnergies() == energies(indices));
        REQUIRE(electronicStatesOccupied.getOccupations() == occupations(indices));
        REQUIRE(electronicStatesOccupied.getKPointMap() == kPointMap(indices));
        REQUIRE(electronicStatesOccupied.getBandMap() == bandMap(indices));
    }
    SECTION("Test unoccupied") {
        std::vector<Eigen::Index> indices = {3, 4, 5};
        auto electronicStatesUnoccupied = electronicStates.unoccupied();
        REQUIRE(electronicStatesUnoccupied.getEnergies() == energies(indices));
        REQUIRE(electronicStatesUnoccupied.getOccupations() == occupations(indices));
        REQUIRE(electronicStatesUnoccupied.getKPointMap() == kPointMap(indices));
        REQUIRE(electronicStatesUnoccupied.getBandMap() == bandMap(indices));
    }
}