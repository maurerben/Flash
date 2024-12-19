#include <data_processing/hdf5.h>


//template<typename T, std::size_t N> void
//flash::data_processing::hdf5::read_vector( H5::DataSet& dataset, T data[N]) {
//
//    // Get the dataspace of the dataset
//    auto dataspace = dataset.getSpace();
//
//    // Get the number of dimensions of the dataspace
//    auto rank = dataspace.getSimpleExtentNdims();
//
//    // Get the dimensions of the dataspace
//    std::vector<hsize_t> dims(rank);
//    dataspace.getSimpleExtentDims(dims.data());
//
//    // Check if the number of dimensions of the dataspace is equal to 1
//    if (rank != 1) {
//        throw std::runtime_error("The dataspace of the dataset must have rank 1.");
//    }
//
//    // Check if dataset has size N
//    if (dims[0] != N) {
//      throw std::runtime_error("The dataspace of the dataset must have size N.");
//    }
//
//    // Read the data from the dataset
////    std::array<T, N> result;
//    dataset.read(data, H5::PredType::NATIVE_DOUBLE);
//
////    return result;
//}

template<std::size_t N>
void flash::data_processing::hdf5::read_data(H5::DataSet& dataset, std::double_t data[N]) {
    // Get the dataspace of the dataset
    auto dataspace = dataset.getSpace();

    // Get the number of dimensions of the dataspace
    auto rank = dataspace.getSimpleExtentNdims();

    // Get the dimensions of the dataspace
    std::vector<hsize_t> dims(rank);
    dataspace.getSimpleExtentDims(dims.data());

    // Check if the number of dimensions of the dataspace is equal to 1
//    if (rank != 1) {
//        throw std::runtime_error("The dataspace of the dataset must have rank 1.");
//    }

    // Check if dataset has size N
//    if (dims[0] != N) {
//        throw std::runtime_error("The dataspace of the dataset must have size N.");
//    }

    // Read the data from the dataset
    //    std::array<T, N> result;
    dataset.read(data, H5::PredType::NATIVE_DOUBLE);
}
