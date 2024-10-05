#include <data_processing/read_hdf5.h>

namespace tp = flash::types;
using namespace flash::data_processing::read_hdf5;

tp::DVector readDoubleVectorHDF5(const std::string &filename, const std::string &datasetName) {
    try {
        // Open the HDF5 file
        H5::H5File file(filename, H5F_ACC_RDONLY);

        // Open the dataset
        H5::DataSet dataset = file.openDataSet(datasetName);

        // Get the dataspace and the size of the dataset
        H5::DataSpace dataspace = dataset.getSpace();
        hsize_t dims[1];
        int ndims = dataspace.getSimpleExtentDims(dims, nullptr);

        // Read data into a std::vector<double>
        std::vector<tp::real_dp> buffer(dims[0]);
        dataset.read(buffer.data(), H5::PredType::NATIVE_DOUBLE);

        // Convert std::vector to tp::DVector
        tp::DVector eigenVector = Eigen::Map<tp::DVector>(buffer.data(), buffer.size());

        return eigenVector;
    } catch (H5::FileIException &error) {
        error.printErrorStack();
    } catch (H5::DataSetIException &error) {
        error.printErrorStack();
    } catch (H5::DataSpaceIException &error) {
        error.printErrorStack();
    }

    // Return an empty vector in case of an error
    return tp::DVector();
}