#pragma once

#include <config/Keys.h>
#include <config/Node.h>
#include <config/nodes/ElectronicStates.h>

namespace flashlight {
namespace config {
namespace nodes {

class Input : Node {
   public:
    /**
     * @brief see Node
     */
    Input(const std::string& key) : Node(key) {}

    /**
     * @brief Ground state configuration of the input data. See also ElectronicStates.
     *
     * @details Define the ground state of the input data. The ground state is defined by
     *         - the **k**-grid on which the ground state is prepared, see ElectronicStates#kGrid
     *         - the number of bands, see ElectronicStates#nBands
     *         - the occupation factor, see ElectronicStates#occupiedPart
     */
    ElectronicStates electronicStates{keys::ELECTRONICSTATES};

    void load(const YAML::Node& node) {
        try {
            auto useNode = node[this->key] ? node[this->key] : DEFAULT_NODE;
            electronicStates.load(useNode);
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
}  // namespace flashlight