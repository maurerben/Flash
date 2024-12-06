#pragma once

#include <config/Keys.h>
#include <config/Node.h>
#include <config/nodes/Spectrum.h>

#include <cstdint>
#include <stdexcept>

namespace flash {
namespace config {
namespace nodes {

class Output : Node {
   public:
    /**
     * @brief see Node
     */
    Output(const std::string& key) : Node(key) {}

    /**
     * @brief Spectrum configuration of the input data. See also Spectrum.
     * @details Define the spectrum of the input data. The spectrum is defined by
     *        - the energy interval, see Spectrum#energyInterval
     *        - the number of sampling points, see Spectrum#nSampling
     *        - the kind of peaks, see Spectrum#peak
     */
    Spectrum spectrum{keys::SPECTRUM};

    void load(const YAML::Node& node) {
        try {
            auto useNode = node[this->key] ? node[this->key] : DEFAULT_NODE;
            spectrum.load(useNode);
        } catch (std::runtime_error e) {
            throw std::runtime_error(this->key + "." + e.what());
        } catch (...) {
            std::cout << "Caught an unknown exception!" << std::endl;
            throw std::runtime_error(this->key + " could not be loaded.");
        }
        validateRules();
    }

   private:
    void validateRules() {}
};

}  // namespace nodes
}  // namespace config
}  // namespace flash