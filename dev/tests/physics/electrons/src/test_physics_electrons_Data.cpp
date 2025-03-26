#define CATCH_PHYSICS_MAIN
#include <utils/types.h>
#include <physics/electrons/Index.h>
#include <physics/electrons/Data.h>
#include <catch2/catch_all.hpp>
#include <Eigen/Dense>
#include <cstdint>
#include <dataProcessing/hdf5/access.h>

using namespace flashlight::utils;
using namespace flashlight::dataProcessing;
using namespace flashlight::physics::electrons;


TEST_CASE("Data Default Constructor", "[Data]") {
    Data data;
    REQUIRE(data.NumKpoints() == 0);
    REQUIRE(data.NumBands() == 0);
    REQUIRE(data.Energies().size() == 0);
    REQUIRE(data.Occupations().size() == 0);
}


TEST_CASE("Data Parameterized Constructor", "[Data]") {
  	index_t numKpoints = 3;
    index_t numBands = 2;
    vector_t<real_t> energies(6);
	energies << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
    vector_t<real_t> occupations(6);
	occupations << 0.5, 0.6, 0.7, 0.8, 1.0, 2.0;

    Data data(numKpoints, numBands, energies, occupations);

    REQUIRE(data.NumKpoints() == numKpoints);
    REQUIRE(data.NumBands() == numBands);
    REQUIRE(data.Energies().isApprox(energies));
    REQUIRE(data.Occupations().isApprox(occupations));
}


TEST_CASE("Invalid Data Parameterized Constructor", "[Data]") {
  	index_t numKpoints = 3;
    index_t numBands = 2;
    vector_t<real_t> energies(6);
	energies << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
    vector_t<real_t> occupations(6);
	occupations << 0.5, 0.6, 0.7, 0.8, 1.0, 2.0;

    // Test invalid constructions
    auto initializeData = [](size_t numKpoints, size_t numBands, vector_t<real_t>& energies, vector_t<real_t>& occupations) {
          Data obj(numKpoints, numBands, energies, occupations);
        };

    // numKpoints * numBands is not equal to the size of energies and occupations
    numKpoints = 2;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );

    // size of energies is wrong
    numKpoints = 3;
    energies.resize(5);
    energies << 1.0, 2.0, 3.0, 4.0, 5.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );
    energies.resize(6);
    energies << 0.5, 0.6, 0.7, 0.8, 1.0, 2.0;

    // size of occupations is wrong
    occupations.resize(5);
    occupations << 0.5, 0.0, 0.7, 0.8, 1.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );

    // some elements of occupations are smaller 0.
    occupations.resize(6);
    occupations << 0.5, 0.6, -0.7, 0.8, -1.0, 2.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );

    // some elements of occupations are larger 2.
    occupations << 0.5, 2.6, 0.7, 2.8, 1.0, 2.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );
}


H5::Group createTestH5GroupForData(
    const index_t& numKpoints,
    const index_t& numBands,
    const vector_t<real_t>& energies,
    const vector_t<real_t>& occupations) {

    // Set file access property list to use in-data storage
    H5::FileAccPropList fapl;
    fapl.setCore(1024 * 1024, false);  // 1 MB data allocation, no backing store

    // Create an in-data HDF5 file
    H5::H5File file("data.h5", H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, fapl);
    H5::Group group = file.createGroup("/group");

      hdf5::electrons::write(group, numKpoints, numBands, energies, occupations);

    return group;
};


TEST_CASE("Data HDF5 Constructor", "[Data]") {
  	index_t numKpoints = 3;
    index_t numBands = 2;
    vector_t<real_t> energies(6);
	energies << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
    vector_t<real_t> occupations(6);
	occupations << 0.5, 0.6, 0.7, 0.8, 1.0, 2.0;

    // Create an file
    H5::H5File file("data.h5", H5F_ACC_TRUNC);
    hdf5::electrons::write(file, numKpoints, numBands, energies, occupations);
   	Data data(file);

    REQUIRE(data.NumKpoints() == numKpoints);
    REQUIRE(data.NumBands() == numBands);
    REQUIRE(data.Energies().isApprox(energies));
    REQUIRE(data.Occupations().isApprox(occupations));
}


TEST_CASE("Invalid Data HDF5 Constructor", "[Data]") {
  	index_t numKpoints = 3;
    index_t numBands = 2;
    vector_t<real_t> energies(6);
	energies << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
    vector_t<real_t> occupations(6);
	occupations << 0.5, 0.6, 0.7, 0.8, 1.0, 2.0;

    // Test invalid constructions
    auto initializeData = [](
          size_t numKpoints,
          size_t numBands,
          vector_t<real_t>& energies,
          vector_t<real_t>& occupations) {
        H5::H5File file("data.h5", H5F_ACC_TRUNC);
        hdf5::electrons::write(file, numKpoints, numBands, energies, occupations);
   	    Data data(file);
    };

    // numKpoints * numBands is not equal to the size of energies and occupations
    numKpoints = 2;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );

    // size of energies is wrong
    numKpoints = 3;
    energies.resize(5);
    energies << 1.0, 2.0, 3.0, 4.0, 5.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );
    energies.resize(6);
    energies << 0.5, 0.6, 0.7, 0.8, 1.0, 2.0;

    // size of occupations is wrong
    occupations.resize(5);
    occupations << 0.5, 0.0, 0.7, 0.8, 1.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );

    // some elements of occupations are smaller 0.
    occupations.resize(6);
    occupations << 0.5, 0.6, -0.7, 0.8, -1.0, 2.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );

    // some elements of occupations are larger 2.
    occupations << 0.5, 2.6, 0.7, 2.8, 1.0, 2.0;
    REQUIRE_THROWS_AS(
        initializeData(numKpoints, numBands, energies, occupations),
        std::invalid_argument
    );
}


TEST_CASE("Data Energy Retrieval", "[Data]") {
	index_t numKpoints = 3;
    index_t numBands = 2;
    vector_t<real_t> energies(6);
	energies << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
    vector_t<real_t> occupations(6);
	occupations << 0.5, 0.6, 0.7, 0.8, 1.0, 2.0;

    Data data(numKpoints, numBands, energies, occupations);
    Indices indices(data.NumKpoints(),data.NumBands());

    REQUIRE(data.Energy(indices[0]) == 1.0);
    REQUIRE(data.Energy(indices[1]) == 2.0);
    REQUIRE(data.Energy(indices[2]) == 3.0);
    REQUIRE(data.Energy(indices[3]) == 4.0);
    REQUIRE(data.Energy(indices[4]) == 5.0);
    REQUIRE(data.Energy(indices[5]) == 6.0);

    REQUIRE(data.Occupation(indices[0]) == 0.5);
    REQUIRE(data.Occupation(indices[1]) == 0.6);
    REQUIRE(data.Occupation(indices[2]) == 0.7);
    REQUIRE(data.Occupation(indices[3]) == 0.8);
    REQUIRE(data.Occupation(indices[4]) == 1.0);
    REQUIRE(data.Occupation(indices[5]) == 2.0);

    REQUIRE(data.Energies(indices).isApprox(energies));
    REQUIRE(data.Occupations(indices).isApprox(occupations));
}