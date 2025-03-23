#define CATCH_UTILS_MAIN

#include <utils/IndexMap.h>
#include <utils/types.h>
#include <catch2/catch_all.hpp>


using namespace flash::utils;

TEST_CASE("IndexMap: Default Constructor", "[IndexMap]") {

    auto indexMap = IndexMap();
    REQUIRE(indexMap.size() == 0);
    REQUIRE(indexMap.TargetIndices().size() == 0);
    REQUIRE(indexMap.NumberOfTargets() == 0);
}


TEST_CASE("IndexMap: Parameterized Constructor", "[IndexMap]") {
        vector_t<index_t> targetIndices(3);
        targetIndices << 2, 4, 6;

        index_t numTargets = 6;
        IndexMap map(targetIndices, numTargets);

        REQUIRE(map.size() == 3);
        REQUIRE(map.NumberOfTargets() == numTargets);
        REQUIRE(map.TargetIndices().isApprox(targetIndices));
}


TEST_CASE("IndexMap: Destructor", "[IndexMap]") {
    vector_t<index_t> targetIndices(3);
    targetIndices << 2, 4, 6;
    index_t numTargets = 6;

    IndexMap map(targetIndices, numTargets);
    REQUIRE(map.size() == 3);
    REQUIRE(map.NumberOfTargets() == numTargets);
    REQUIRE(map.TargetIndices().isApprox(targetIndices));

    map.~IndexMap();
    REQUIRE(map.size() == 0);
    REQUIRE(map.TargetIndices().size() == 0);
    REQUIRE(map.NumberOfTargets() == 0);
}

TEST_CASE("IndexMap: == and != Operators", "[IndexMap]") {
    // An instance compared with it self
    vector_t<index_t> targetIndices(3);
    targetIndices << 2, 4, 6;
    index_t numTargets = 6;
    IndexMap map1(targetIndices, numTargets);
    REQUIRE(map1 == map1);
    REQUIRE_FALSE(map1 != map1);

    // Change only numberOfTargets
    numTargets = 7;
    IndexMap map2(targetIndices, numTargets);
    REQUIRE_FALSE(map1 == map2);
    REQUIRE(map1 != map2);

    // Change elements targetIndices
    targetIndices << 1, 4, 6;
    numTargets = 6;
    IndexMap map3(targetIndices, numTargets);
    REQUIRE_FALSE(map1 == map3);
    REQUIRE(map1 != map3);

    // Resize targetIndice
    targetIndices.resize(4);
    targetIndices << 2, 4, 6, 5;
    IndexMap map4(targetIndices, numTargets);
    REQUIRE_FALSE(map1 == map4);
    REQUIRE(map1 != map4);
}




TEST_CASE("IndexMap: TargetIndex() Function", "[IndexMap]") {
    vector_t<index_t> targetIndices(4);
    targetIndices << 1, 3, 5, 7;

    index_t numTargets = 7;
    IndexMap map(targetIndices, numTargets);

    REQUIRE(map.TargetIndex(0) == 1);
    REQUIRE(map.TargetIndex(1) == 3);
    REQUIRE(map.TargetIndex(2) == 5);
    REQUIRE(map.TargetIndex(3) == 7);
}

TEST_CASE("IndexMap: TargetIndices() with Subset", "[IndexMap]") {
    vector_t<index_t> targetIndices(5);
    targetIndices << 10, 20, 30, 40, 50;

    IndexMap map(targetIndices, 50);

    vector_t<index_t> indices(3);
    indices << 0, 2, 4;

    vector_t<index_t> subTargets = map.TargetIndices(indices);

    REQUIRE(subTargets.size() == 3);
    REQUIRE(subTargets(0) == 10);
    REQUIRE(subTargets(1) == 30);
    REQUIRE(subTargets(2) == 50);
}


H5::Group mockTestH5GroupForIndexMap(const vector_t<index_t>& targetIndices, const index_t& numberOfTargets) {
    // Set file access property list to use in-memory storage
    H5::FileAccPropList fapl;
    fapl.setCore(1024 * 1024, false);  // 1 MB memory allocation, no backing store

    // Create an in-memory HDF5 file
    H5::H5File file("memory.h5", H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, fapl);//,
    H5::Group group = file.createGroup("/group");

    // Number of targets (as an attribute)
    H5::Attribute attr = group.createAttribute("number_of_targets", H5::PredType::NATIVE_LONG, H5::DataSpace());
    attr.write(H5::PredType::NATIVE_LONG, &numberOfTargets);

    // Target indices dataset
    hsize_t dims[1] = {static_cast<hsize_t>(targetIndices.size())};
    H5::DataSpace dataspace(1, dims);
    H5::DataSet dataset = group.createDataSet("target_indices", H5::PredType::NATIVE_LONG, dataspace);
    dataset.write(targetIndices.data(), H5::PredType::NATIVE_LONG);

    return group;
};


TEST_CASE("IndexMap: HDF5 Constructor", "[IndexMap]") {
    vector_t<index_t> targetIndices(3);
    targetIndices << 2, 4, 6;

    index_t numTargets = 6;
    auto testGroup = mockTestH5GroupForIndexMap(targetIndices, numTargets);
    IndexMap map(testGroup);

    REQUIRE(map.size() == 3);
    REQUIRE(map.NumberOfTargets() == numTargets);
    REQUIRE(map.TargetIndices().isApprox(targetIndices));
}