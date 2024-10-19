#pragma once

#include <config/Keys.h>
#include <config/Node.h>
#include <config/parameters/Matrix.h>
#include <config/parameters/Vector.h>
#include <constants/Tensor.h>

#include <Eigen/Dense>
#include <cstdint>
#include <stdexcept>

namespace flash {
namespace config {
namespace nodes {

/// @brief Parameter type for sampling
using SamplingParameter = config::parameters::Vector<std::size_t, 3>;
/// @brief Parameter type for offset
using OffsetParameter = config::parameters::Vector<std::double_t, 3>;
/// @brief Parameter type for lattice vectors
using LatticeParameter = config::parameters::Matrix<std::double_t, 3, 3>;

/// @brief Regular grid configuration node
class RegularGrid : Node {
   public:
    RegularGrid(const std::string& name) : Node(name) {}

    /**
     * @brief Sampling per dimension.
     * @details Each element must be at least 1. The default is
     *          given by #singularSampling
     */
    SamplingParameter Sampling{keys::SAMPLING, constants::Tensor::singularSampling};

    /**
     * @brief Offset of the grid points per dimension in lattice coorinates.
     * @details The grid points will be offset by 1 / RegularGrid#Sampling.
     *          Each element must be `>=0.0` and `<1`. The default is
     *          given by #zeroOffset
     */
    OffsetParameter Offset{keys::OFFSET, constants::Tensor::zeroOffset};

    /**
     * @brief Lattice vectors.
     * @details The vectors need to be linear independent. The default is
     *          given by #zeroOffset
     */
    LatticeParameter Lattice{keys::LATTICE, constants::Tensor::identity3D};

    /**
     * @brief Load RegularGrid instance from a config node.
     * @param node Node that holds the configuration data.
     * @throws std::runtime_error if something goes wrong.
     */
    void load(const YAML::Node& node);

   private:
    /**
     * @brief Validate instance.
     * @details To be valid the members must fulfill the described conditions.
     * @throws std::runtime_error If the conditions are not met.
     */
    void validate();
};

void RegularGrid::load(const YAML::Node& node) {
    if (node[this->name]) {
        try {
            Sampling.load(node[this->name]);
            Offset.load(node[this->name]);
            Lattice.load(node[this->name]);
        } catch (std::runtime_error e) {
            throw std::runtime_error(this->name + "." + e.what());
        }
    } else {
        throw std::runtime_error(this->name + " is not defined.");
    }
    validate();
    return;
}

void RegularGrid::validate() {
    // Sampling must not be 0 for any dimension
    Eigen::Vector<std::size_t, 3> sampling = Sampling;
    bool samplingIsSmallerZero = (sampling.array() > 1).all();
    if (samplingIsSmallerZero) {
        throw std::runtime_error(name + ".Sampling > 1 in one or more dimensions.");
    }

    // Offset must be >= 0 and < 1 for every dimension
    Eigen::Vector<std::double_t, 3> offset = Offset;
    bool offsetIsSmallerZero = (offset.array() < 0.0).all();
    if (offsetIsSmallerZero) {
        throw std::runtime_error(name + ".Offset < 0.0 in one or more dimensions.");
    }
    bool offsetIsLargerEqualOne = (offset.array() >= 1.0).all();
    if (offsetIsLargerEqualOne) {
        throw std::runtime_error(name + ".Offset >= 1.0 in one or more dimensions.");
    }

    // Lattice columns must be linear independent
    // this is the case if rank==3
    using trafo3D = Eigen::Matrix<std::double_t, 3, 3>;
    trafo3D lattice = Lattice;
    Eigen::FullPivLU<trafo3D> lu(lattice);
    bool latticeIsNotLinearIndependent = (lu.rank() < 3);
    if (latticeIsNotLinearIndependent) {
        throw std::runtime_error(name + ".Lattice is not linear independent.");
    }
}

}  // namespace nodes
}  // namespace config
}  // namespace flash
