#include <data_processing/hdf5_wrappers.h>

namespace tp = flash::types;
using namespace flash::data_processing::hdf5_wrappers;




// Default template definition
template <typename T>
H5::PredType hdf5Type() {
    static_assert(sizeof(T) == 0, "Unsupported type for HDF5 conversion");
    return H5::PredType::NATIVE_INT;  // Return something or remove this line
}

// Full specialization for double
template <>
H5::PredType hdf5Type<tp::my_double>() {
    return H5::PredType::NATIVE_DOUBLE;
}

// Full specialization for std::complex<double>
template <>
H5::PredType hdf5Type<std::complex<double>>() {
    return H5::PredType::NATIVE_DOUBLE;  // You may need to handle complex types differently.
}

std::vector<hsize_t> dataspaceDims(const H5::DataSpace& dataspace) {
    auto rank = dataspace.getSimpleExtentNdims();
    std::vector<hsize_t> dims(rank);

    rank = dataspace.getSimpleExtentDims(dims.data(), nullptr);

    return dims;
}

template<typename T>
flash::types::vector<T> readVector(const H5::DataSet &dataset) {
    try {
        // Get the dataspace and the size of the dataset
        H5::DataSpace dataspace = dataset.getSpace();
        
        auto dims_dataspace = dataspaceDims(dataspace);



        // if (sizeof(T) == std::complex(std::double_t)){
        //     if ( rank != 1 ) {
        //         throw std::
        //     }
        // }
        // elif (sizeof(T) == std::complex(std::double_t) {

        // }
        // }

        // Read data into a std::vector<double>
        std::vector<tp::my_double> buffer(dims_dataspace[0]);
        dataset.read(buffer.data(), hdf5Type<T>);

        // Convert std::vector to tp::vector<T>
        tp::vector<T> eigenVector = Eigen::Map<tp::vector<T>>(buffer.data(), buffer.size());

        return eigenVector;

    } catch (H5::DataSetIException &error) {
        error.printErrorStack();
    } catch (H5::DataSpaceIException &error) {
        error.printErrorStack();
    }

    // Return an empty vector in case of an error
    return tp::vector<T>();
}

