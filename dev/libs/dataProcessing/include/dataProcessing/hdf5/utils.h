//
// Created by bene on 3/23/25.
//

#pragma once

#include <H5Cpp.h>
#include <Eigen/Core>
#include <utils/types.h>
#include <unordered_map>
#include <typeindex>
#include <cstdint>
#include <vector>

namespace flashlight{
namespace dataProcessing{
namespace hdf5{
namespace utils{

    H5::PredType getPredType(const std::type_info& ti);


}
}
}
}
