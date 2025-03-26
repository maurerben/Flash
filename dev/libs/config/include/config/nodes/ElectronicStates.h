#pragma once

#include <config/Keys.h>
#include <config/Node.h>
#include <config/parameters/Scalar.h>
#include <constants/tensor.h>
#include <utils/types.h>

#include <Eigen/Dense>
#include <cmath>
#include <cstdint>
#include <stdexcept>

#include "Grid.h"

namespace flashlight {
namespace config {
namespace nodes {

using utils::index_t;
using parameters::Double;
using parameters::Size;

/// @brief Ground state configuration node
class ElectronicStates : Node {
   public:


    /**
     * @brief See Node.
     */
    ElectronicStates(const std::string& key) : Node(key) {}

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
    Size nBands{keys::NBANDS};


    /**
     * @brief Load ElectronicStates instance from a config node.
     * @param node Node that holds the configuration data.
     * @throws std::runtime_error if something goes wrong.
     */
    void load(const YAML::Node& node) {
        try {
            kGrid.load(node[this->key]);
            nBands.load(node[this->key]);
        } catch (std::runtime_error e) {
            throw std::runtime_error(this->key + "." + e.what());
        } catch (...) {
            std::cout << "Caught an unknown exception!" << std::endl;
            throw std::runtime_error(this->key + " could not be loaded.");
        }
        validateRules();
    };

    index_t size() const {return nBands * kGrid.sampling.prod();}

   private:
    void validateRules() {
        // nBands must be > 1
        if (nBands <= static_cast<index_t>(1)) {
            throw std::runtime_error(key + "." + keys::NBANDS + " <= 1.");
        }

        return;
    }
};

}  // namespace nodes
}  // namespace config
}  // namespace flashlight
