//
// Created by bene on 3/23/25.
//

#pragma once

#include <string>


// Electron data names as strings
namespace flashlight{
namespace dataProcessing{
namespace hdf5{
namespace names{

namespace electrons{
    constexpr std::string groupName("electrons");
    constexpr std::string numKpointsAtrrName("num_kpoints");
    constexpr std::string numBandsAtrrName("num_bands");
    constexpr std::string energiesDsetName("energies");
    constexpr std::string occupationsDsetName("occupations");
}

}
}
}
}