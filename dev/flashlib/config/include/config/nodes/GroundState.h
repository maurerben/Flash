#pragma once

#include <config/Keys.h>
#include <config/Node.h>
#include <config/parameters/Scalar.h>
#include <constants/Tensor.h>

#include <Eigen/Dense>
#include <cstdint>
#include <stdexcept>

#include "Grid.h"

namespace flash {
namespace config {
namespace nodes {

/// @brief Parameter for the count of something
using CountParameter = config::parameters::Scalar<std::size_t>;
/// @brief Parameter for the factor of something
using FactorParameter = config::parameters::Scalar<std::double_t>;

/// @brief Ground state configuration node
class GroundStateConfig : Node {
   public:
    GroundStateConfig(const std::string &name) : Node(name) {}

    /**
     * @brief Number of **k**-points per dimesion.
     * @details Must be larger equal one in all dimensions.
     */
    SamplingParameter kSampling{keys::KSAMPLING};

    /**
     * @brief Number of states
     * @details Must be `> 1`.
     */
    CountParameter nStates{keys::NSTATES, 1};

    /**
     * @brief Occupation facter
     * @details The occupation factor is defined as the number of occupied
     *          states devided by GroundState#nStates. If the occupation is
     *          constant on the **k**points, the number of occupied bands can
     *          obtained by `GroundState#nStates * GroundState#occupationFactor`.
     *          If this is not true, then the number of occupied Electrons, given by
     *          `GroundState#nStates * GroundState#occupationFactor * product(Groundstate#kSampling)`
     *          must be an integral. Must be `> 0` and `< 1`.
     */
    FactorParameter occupationFactor { keys::OCCUPATION_FACTOR }

    /**
     * @brief Load GroundState instance from a config node.
     * @param node Node that holds the configuration data.
     * @throws std::runtime_error if something goes wrong.
     */
    void load(const YAML::Node &node);

   private:
    void validate();
};

void GroundStateConfig::validate() { return; }

void GroundStateConfig::load(const YAML::Node &node) {
    try {
        kSampling.load(node[this->name]);
        nStates.load(node[this->name]);
        occupationFactor.load(node[this->name]);
    } catch (std::runtime_error e) {
        throw std::runtime_error(this->name + "." + e.what());
    }
}

}  // namespace nodes
}  // namespace config
}  // namespace flash
