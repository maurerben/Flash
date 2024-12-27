#pragma once

#include "electrons/ElectronicStates.h"
#include <Eigen/Dense>
#include <cstdint>

namespace flash {
namespace physics {
namespace excitons {

class Transitions {
  private:
    Eigen::Vector<std::double_t, Eigen::Dynamic> energies;
    Eigen::Vector<std::size_t, Eigen::Dynamic> kPointMap;
    Eigen::Vector<std::size_t, Eigen::Dynamic> occupiedBandMap;
    Eigen::Vector<std::size_t, Eigen::Dynamic> unoccupiedBandMap;

  public:
    Transitions(const electrons::ElectronicStates& occupied,
                const electrons::ElectronicStates& unoccupied);
    ~Transitions();
};


}  // namespace electrons
}  // namespace physics
}  // namespace excitons