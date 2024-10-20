#pragma once

#include <config/Keys.h>
#include <config/Node.h>
#include <config/parameters/Matrix.h>
#include <config/parameters/Vector.h>
#include <constants/tensor.h>

#include <Eigen/Dense>
#include <cstdint>
#include <stdexcept>

namespace flash {
namespace config {
namespace nodes {

using parameters::Sampling;
using parameters::Coordinate;
using parameters::CoordinateTrafo;

/**
 * @brief Regular grid configuration node.
 * @details Defines how a node is expected that defines a regular grid. A regular grid is used
 *          to sample a parallelepiped uniformally so that the points have the same distance
 *          in each dimension and each point has the same volume element.
 *
 *          It is defined by
 *              - the parallelepiped \f$ \mathbf A \f$, saved in RegularGrid#Parallelepiped,
 *              - the sampling \f$ (N_1, N_2, N_3) \f$, saved in RegularGrid#Sampling,
 *              - the offset  \f$ (o_1, o_2, o_3) \f$, saved in RegularGrid#offset
 *          The cartesian coordinates \f$\mathbf{x}\f$ of the \f$i\f$'th in the first, the
 *          \f$j\f$'th in the second and \f$k\f$'th in the third direction is calculated by
 *          \f[
 *              \begin{pmatrix} x_i \\ y_j \\ z_k \end{pmatrix} = \mathbf A \cdot
 *              \begin{pmatrix} (i + o_1) / N_1 \\ (j + o_2) / N_2 \\ (k + o_3) / N_3 \end{pmatrix}
 *          \f]
 */
class RegularGrid : Node {
   public:
    RegularGrid(const std::string& key) : Node(key) {}

    /**
     * @brief Sampling of the grid.
     * @details Give the number of sampling points per dimension.
     *          ### Rules
     *              - Must be larger then 1 for each dimension.
     *          #### Default
     *              Must be defined
     */
    Sampling sampling{keys::SAMPLING};

    /**
     * @brief Offset of the grid points per dimension in parallelepiped coorinates.
     * @details The grid points will be offset by RegularGrid#offset / RegularGrid#Sampling.
     *          #### Rules
     *              - each element must be `>=0.0`
     *              - each element must be `<1.0`
     *          #### Default
     *              - constants#tensor#origin
     */
    Coordinate offset{keys::OFFSET, constants::tensor::origin};

    /**
     * @brief Column-wise span vectors of the parallelepiped.
     * @details
     *          #### Rules
     *              - Column vectors must be linear independent.
     *          #### Default
     *              - constants#tensor#identity3D
     */
    CoordinateTrafo parallelepiped{keys::PARALLELEPIPED, constants::tensor::identity3D};

    /**
     * @brief See Node#load
     * @param node
     * @throws std::runtime_error
     */
    void load(const YAML::Node& node) {
        if (node[this->key]) {
            try {
                sampling.load(node[this->key]);
                offset.load(node[this->key]);
                parallelepiped.load(node[this->key]);
            } catch (std::runtime_error e) {
                throw std::runtime_error(this->key + "." + e.what());
            } catch (...) {
                std::cout << "Caught an unknown exception!" << std::endl;
                throw std::runtime_error(this->key + " could not be loaded.");
            }
        } else {
            throw std::runtime_error(this->key + " is not defined.");
        }
        validateRules();
        return;
    }

   private:
    void validateRules() {
        // Sampling must be >= 1 in every dimension
        if ((sampling < 1).any()) {
            throw std::runtime_error(key + "." + keys::SAMPLING + " < 1 in one dimension or more.");
        }

        // Offset must be >= 0.0 in every dimension
        if ((offset < 0.0).any()) {
            throw std::runtime_error(key + "." + keys::OFFSET + " < 0.0 in one or more dimensions.");
        }
        // Offset must be < 1.0 in every dimension
        if ((offset >= 1.0).any()) {
            throw std::runtime_error(key + "." + keys::OFFSET + " >= 1.0 in one or more dimensions.");
        }

        // Parallelepiped column vectors are linear independent if rank == 3
        Eigen::FullPivLU<Eigen::Matrix<std::double_t, 3, 3>> lu(parallelepiped.Value());
        if (lu.rank() < 3) {
            throw std::runtime_error(key + "." + keys::PARALLELEPIPED + " is not linear independent.");
        }
    }
};

}  // namespace nodes
}  // namespace config
}  // namespace flash
