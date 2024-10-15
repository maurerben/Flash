#pragma once

#include <yaml-cpp/yaml.h>

#include <string>

namespace flash {
namespace configParameters {

/// @class Node
/// @brief Base class to define elements in the configuration file
class Node {
   public:
    /// @brief Initilize a node and set Node#name
    /// @param name
    Node(const std::string& name) : name(name) {}

    /// @brief Load configuration from \a configNode
    /// @param configNode contains cofiguration file
    /// @throws std::runtime_error if something goes wrong during loading
    virtual void load(const YAML::Node& configNode) {};

    /// @brief Validate if the node is defined correctly
    /// @throws runtime_error if the validation fails
    virtual void validate() {};

   protected:
    /// @brief Name of the key indcating the node in the configuration file.
    std::string name;
};

}  // namespace configParameters
}  // namespace flash