#define CATCH_hdf5_wrappers_MAIN

#include <catch2/catch.hpp>
#include <H5Cpp.h>

#include <types/types.h>
#include <data_processing/hdf5_wrappers.h>

namespace tp = flash::types;
using namespace flash::data_processing::hdf5_wrappers;


TEST_CASE("Test reading a vector from an HDF5 dataset type", "[valid-config]") {
    tp::vector<tp::my_double> testVector{{1.0, 2.0, 3.0, 4.0}};
    
}