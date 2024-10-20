
#pragma once

#include <config/parameters/Vector.h>
#include <config/parameters/Scalar.h>
#include <config/parameters/Option.h>
#include <config/Node.h>
#include <config/Keys.h>
#include "Peak.h"
#include <Eigen/Dense>
#include <cstdint>


namespace flash {
namespace config {
namespace nodes {

using parameters::Interval;
using parameters::Size;

/**
 * @brief Spectrum configuration node
 * @details Define how a spectrum should be created. In general a spectrum is a function  
 *          \f$ S(\omega) \f$ defined on an energy interval  \f$ \omega \in \[left E_{min}, E_{max} \right] \f$ 
 *          that is defined by the sum of peak functions  \f$ f(\omega; \mu, \Delta) \f$ at the positions \f$ [E_1, E_2, \hdots, E_n] \f$,
 *          weighted by the weights \f$ [w_1, w_2, \hdots, w_n] \f$ such that
 *          \f[
 *              S(\omega) = \sum_{i=1}^n w_i f(\omega; E_i, \Delta),
 *          \f]
 *          where \f$ \mu \f$ is the postion of the peak and \f$ \Delta \f$ its width. 
 *          
 *          A spectrum config node is defined by 
 *              - the energy interval \f$ \[left E_{min}, E_{max} \right] \f$, 
 *                  saved in Spectrum#energyInterval with unit Hartree
 *              - the number of sampling points of \f$ \omega \in \[left E_{min}, E_{max} \right] \f$ , 
 *                  saved in Spectrum#nSampling
 *              - the kind of peaks, saved in Spectrum#peak, see also Peak.
 *          
 * 
 *          The peak positions and weights are calculated by the solver, depending on the context.
 */
class Spectrum : Node {
    public:
    /**
     * @brief See Node.
     */
    Spectrum(const std::string& key) : Node(key) { }
    
    /**
     * @brief Energy interval \f$ \[left E_{min}, E_{max} \right] \f$ in Hartree
     * @details 
     *          #### Rules
     *              - \f$ E_{min} < E_{max} \f$
     *          #### Default
     *              - Must be defined
     */
    Interval energyInterval{keys::ENERGYINTERVAL};

    /**
     * @brief Number of sampling points of \f$ \omega \in \[left E_{min}, E_{max} \right] \f$
     * @details 
     *          #### Rules
     *              - must be > 0
     *          #### Default
     *              - Must be defined
     */
    Size nSampling{keys::NSAMPLING};

    /**
     * @brief Define the peaks to build the spectrum from
     * @details 
     *          #### Rules
     *              - See Peak
     *          #### Default
     *              - Lorentz peak with a width of 0.001 Hartree
     */
    Peak peak{keys::PEAK};

    /**
     * @brief Load Peak instance from a config node.
     * @param node Node that holds the configuration data.
     * @throws std::runtime_error if something goes wrong.
     */
    void load(const YAML::Node& node) {
        auto useNode = node;
        // if (node[this->key]) {
        // auto useNode = 
        try {
            energyInterval.load(node[this->key]);
            nSampling.load(node[this->key]);
            peak.load(node[this->key]);
        } catch (std::runtime_error e) {
            throw std::runtime_error(this->key + "." + e.what());
        } catch (...) {
            std::cout << "Caught an unknown exception!" << std::endl;
            throw std::runtime_error(this->key + " could not be loaded.");
        }
        validateRules();
    }

    private:
    void validateRules() {
        // first element of energy interval must be smaller than second element
        if (energyInterval.Value()[0] >= energyInterval.Value()[1]) {
            throw std::runtime_error(key + "." + keys::ENERGYINTERVAL + " first value is larger equal the second"); 
        }

        // nSampling must be larger zero
        if (nSampling <= static_cast<std::size_t>(0)) {
            throw std::runtime_error(key + "." + keys::NSAMPLING + " <= 0");
        }
    }

};

}  // namespace nodes
}  // namespace config
}  // namespace flash