#pragma once
#include <string>

namespace flash {
namespace config {
namespace keys {

/// @brief Indicate a sampling parameter
const inline std::string SAMPLING{"sampling"};
/// @brief Indicate an offset parameter
const inline std::string OFFSET{"offset"};
/// @brief Indicate the span vectors of an parallelepiped
const inline std::string PARALLELEPIPED{"parallelepiped"};
/// @brief Indicate the number of states
const inline std::string NSTATES{"n_states"};
/// @brief Indicate the sampling of the **k**-grid
const inline std::string KGRID{"k_grid"};
/// @brief Indicate the occupation factor
const inline std::string OCCUPIED_PART{"occupied_part"};
/// @brief Indicate a width
const inline std::string WIDTH{"width"};
/// @brief Indicate a form of a distribution
const inline std::string FORM{"form"};
/// @brief Indicate an energy interval
const inline std::string ENERGYINTERVAL{"energy_interval"};
/// @brief Indicate a number of sampling points
const inline std::string NSAMPLING{"n_sampling"};
/// @brief Indicate a peal
const inline std::string PEAK{"peak"};

}  // namespace keys
}  // namespace config
}  // namespace flash
