//
// Created by bene on 3/23/25.
//
#include <dataProcessing/access.h>
#include <utils/types.h>

#include <Eigen/Dense>
#include <cstddef>

using namespace flashlight::dataProcessing;
using namespace flashlight::utils;

extern "C" {

void electrons_write_double(const char* filename, const std::size_t num_kpoints, const std::size_t num_bands,
                            const double* energies, const double* occupations, bool overwrite) {
    //
    const std::size_t lengths = num_kpoints * num_bands;
    Eigen::Map<const vector_t<double>> energiesVecor(energies, lengths);
    Eigen::Map<const vector_t<double>> occupationsVecor(occupations, lengths);

    // Construct HDF5 file and call C++ write
    H5::H5File file(filename, overwrite ? H5F_ACC_TRUNC : H5F_ACC_RDWR);
    writeElectrons(file, num_kpoints, num_bands, energiesVecor, occupationsVecor, overwrite);
}

//    void electrons_read(
//        const char* filename,
//        int* num_kpoints,
//        int* num_bands,
//        double* energies,
//        double* occupations,
//        int length
//    ) {
//        H5::H5File file(filename, H5F_ACC_RDONLY);
//        std::vector<double> energyVec, occVec;
//        readElectrons(file, *num_kpoints, *num_bands, energyVec, occVec);
//
//        // Assume caller allocated enough space
//        std::copy(energyVec.begin(), energyVec.end(), energies);
//        std::copy(occVec.begin(), occVec.end(), occupations);
//    }
}