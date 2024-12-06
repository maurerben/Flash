#include <data_processing/hdf5.h>


template<flash::data_processing::hdf5::ArithmeticType T> Eigen::Vector<T, Eigen::Dynamic> 
flash::data_processing::hdf5::read_vector(const H5::DataSet& dataset) {

    // Get the dataspace of the dataset
    auto dataspace = dataset.getSpace();

    // Get the number of dimensions of the dataspace
    auto rank = dataspace.getSimpleExtentNdims();

    // Get the dimensions of the dataspace
    std::vector<hsize_t> dims(rank);
    dataspace.getSimpleExtentDims(dims.data());

    // Check if the number of dimensions of the dataspace is equal to 1
    if (rank != 1) {
        throw std::runtime_error("The dataspace of the dataset must have rank 1.");
    }
    // Create a vector to store the data
    Eigen::Vector<T, Eigen::Dynamic> data(dims[0]);

    // Read the data from the dataset
    dataset.read(data.data(), H5::PredType::NATIVE_DOUBLE);
    return data;
}
