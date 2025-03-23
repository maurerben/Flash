//
// Created by bene on 3/23/25.
//

#include <dataProcessing/hdf5/utils.h>

using namespace flash::dataProcessing::hdf5::utils;

H5::PredType getPredType(const std::type_info& ti) {
    static const std::unordered_map<std::type_index, H5::PredType> type_map = {
        {typeid(int),    H5::PredType::NATIVE_INT},
        {typeid(Eigen::Index),  H5::PredType::NATIVE_LONG},
        {typeid(std::float_t), H5::PredType::NATIVE_FLOAT},
        {typeid(std::double_t), H5::PredType::NATIVE_DOUBLE}
        // etc.
    };

    auto it = type_map.find(std::type_index(ti));
    if (it != type_map.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Unknown type");
    }

}