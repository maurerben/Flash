#pragma once

#include <config/Keys.h>
#include <config/Node.h>
#include <config/parameters/Scalar.h>
#include <constants/tensor.h>

#include <Eigen/Dense>
#include <cmath>
#include <cstdint>
#include <stdexcept>

#include "Grid.h"

namespace flash {
namespace config {
namespace nodes {

using parameters::Size;
using parameters::Double;

/// @brief Ground state configuration node
class GroundState : Node {
   public:
    /**
     * @brief See Node.
     */
    GroundState(const std::string& key) : Node(key) {}

    /**
     * @brief **k**-grid on which the groun state is prepared. See RegularGrid.
     * @details RegularGrid#Offset and RegularGrid#Parallelepiped will be ignored for now.
     *          #### Rules
     *              See RegularGrid
     *          #### Default
     *              Must be defined
     */
    RegularGrid kGrid{keys::KGRID};

    /**
     * @brief Number of bands.
     * @details Give the total number of bands that are prepared in the input ground state data.
     *          #### Rules
     *              - Must be larger then 1
     *          #### Default
     *              Must be defined
     */
    Size nStates{keys::NSTATES};

    /**
     * @brief Occupied part of the bands
     * @details The occupation factor is defined as the number of occupied
     *          states devided by GroundState#nStates. If the occupation is
     *          constant on the **k**-points, the number of occupied bands can
     *          obtained by `GroundState#nStates * GroundState#occupiedPart`.
     *          If this is not true, then the number of occupied Orbitals, given by
     *          `GroundState#nStates * GroundState#occupiedPart * Groundstate#kGrid#sampling.prod()`
     *          must be an integral.
     *          #### Rules
     *              - Must be `> 0.0` and `< 1.0`
     *              - The number of occupied orbitals must be an integral number
     *          #### Default
     *              Must be defined
     */
    Double occupiedPart {keys::OCCUPIED_PART};

    /**
     * @brief Load GroundState instance from a config node.
     * @param node Node that holds the configuration data.
     * @throws std::runtime_error if something goes wrong.
     */
    void load(const YAML::Node& node) {
        try {
            kGrid.load(node[this->key]);
            nStates.load(node[this->key]);
            occupiedPart.load(node[this->key]);
        } catch (std::runtime_error e) {
            throw std::runtime_error(this->key + "." + e.what());
        } catch (...) {
            std::cout << "Caught an unknown exception!" << std::endl;
            throw std::runtime_error(this->key + " could not be loaded.");
        }
        validateRules();
    };

   private:
    void validateRules() {
        // nStates must be > 1
        if (nStates <= static_cast<std::size_t>(1)) {
            throw std::runtime_error(key + "." + keys::NSTATES + " <= 1.");
        }

        // occupiedPart must be > 0.0
        if (occupiedPart <= 0.0) {
            throw std::runtime_error(key + "." + keys::OCCUPIED_PART + " <= 0.0.");
        }
        // occupiedPart must be < 0.0
        if (occupiedPart >= 1.0) {
            throw std::runtime_error(key + "." + keys::OCCUPIED_PART + " >= 1.0.");
        }

        // The number of occpied orbitals as indicated by the members must be an integral number
        auto nOccupiedOrbitals = kGrid.sampling.Value().prod() * nStates * occupiedPart;
        if (std::trunc(nOccupiedOrbitals) != nOccupiedOrbitals) {
            throw std::runtime_error(key + " the number of orbitals as indicated by " + keys::KGRID + ", " +
                                     keys::NSTATES + " and " + keys::OCCUPIED_PART + " is not an integral number.");
        }

        return;
    }
};

}  // namespace nodes
}  // namespace config
}  // namespace flash
