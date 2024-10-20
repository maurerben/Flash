
#pragma once

#include <config/parameters/Scalar.h>
#include <config/parameters/Option.h>
#include <config/Node.h>
#include <config/Keys.h>

#include <Eigen/Dense>
#include <cstdint>


namespace flash {
namespace config {
namespace nodes {


using parameters::Double;
using parameters::Option;

/**     
 * @brief Options for the peak form.
 * @details Valid options are
 * 
 *          ####`"lorentz"` 
 *          The peak will follow the Lorentz distribution. The amplitude at
 *          \f$ x \f$ for a peak with width \f$ \Delta \f$ centered at \f$ \mu \f$
 *          is given by
 *          \f[
 *              f(x; \mu, \Delta) = \frac{1}{\pi \Delta \left[ 1 + 
 *              \left( \frac{x - \mu}{\Delta} \right)^2 \right]}
 *          \f]
 *          
 *          ####`"gauss"` 
 *          The peak will follow the Gauss distribution. The amplitude at
 *          \f$ x \f$ for a peak with width \f$ \Delta \f$ centered at \f$ \mu \f$
 *          is given by
 *          \f[
 *              f(x; \mu, \Delta) = \frac{1}{\Delta \sqrt{2\pi}} 
 *              \exp\left( -\frac{(x - \mu)^2}{2\Delta^2} \right)

 *          \f]
 */
const inline parameters::options_t formOptions{"lorentz", "gauss"};
/// @brief Default width, unit is Hartree
const inline std::double_t defaultWidth{0.001}; // Hartree
/// @brief Default form
const inline std::string defaultForm{"lorentz"};

/**
 * @brief Peak configuration node
 * @details Define a peak, _e.g._, for creating a spectrum. 
 *          
 *          A peak is defined by
 *              - the width \f$ \Delta \f$, saved in Peak#width
 *              - the form of the peak is saved in Peak#form. For valid options
 *                  see #formOptions
 */
class Peak : Node {
    public:
    /**
     * @brief See Node.
     */
    Peak(const std::string& key) : Node(key) {}

    /**
     * @brief Width of the peak
     * @details 
     *          #### Rules
     *              - must be > 0.0
     *          #### Default
     *              - 0.001 Hartree
     */
    Double width{keys::WIDTH, defaultWidth};
    
    /**
     * @brief Options for the peak form.
     * 
     * @details Valid options include:
     * 
     * #### "lorentz"
     * The peak will follow the Lorentz distribution. The amplitude at
     * \f$ x \f$ for a peak with width \f$ \Delta \f$ centered at \f$ \mu \f$
     * is given by
     * \f[
     *     f(x; \mu, \Delta) = \frac{1}{\pi \Delta \left[ 1 + 
     *     \left( \frac{x - \mu}{\Delta} \right)^2 \right]}
     * \f]
     * 
     * #### "gauss"
     * The peak will follow the Gaussian distribution. The amplitude at
     * \f$ x \f$ for a peak with width \f$ \Delta \f$ centered at \f$ \mu \f$
     * is given by
     * \f[
     *     f(x; \mu, \Delta) = \frac{1}{\Delta \sqrt{2\pi}} 
     *     \exp\left( -\frac{(x - \mu)^2}{2\Delta^2} \right)
     * \f]
     * 
     * #### Rules
     * - Must be a valid option.
     * 
     * #### Default
     * - "lorentz"
     */
    Option form{keys::FORM, formOptions, defaultForm};

    /**
     * @brief Load Peak instance from a config node.
     * @param node Node that holds the configuration data.
     * @throws std::runtime_error if something goes wrong.
     */
    void load(const YAML::Node& node) {
        
        // width.load(node[this->key]);
        // form.load(node[this->key]);
        try {
            auto useNode = node[this->key] ? node[this->key] : DEFAULT_NODE;
            width.load(useNode);
            form.load(useNode);
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
        // Width must be larger zero
        if(width <= 0.0) {
            throw std::runtime_error(key + "." + keys::WIDTH + " <= 0.0");
        }
    }
};


}  // namespace nodes
}  // namespace config
}  // namespace flash