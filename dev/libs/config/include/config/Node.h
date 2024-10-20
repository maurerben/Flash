#pragma once

#include <yaml-cpp/yaml.h>

#include <string>

namespace flash {
namespace config {

const auto DEFAULT_NODE = YAML::Load("default: 0");

/// @class Node
/// @brief Base class to define elements in the configuration file
class Node {
   public:
    /// @brief Initilize a node and set Node#key
    /// @param key See Node#key
    /// @param is_madatory See Node#is_madatory
    Node(const std::string& key) : key(key){}

    /// @brief Load configuration from \a node
    /// @param node contains cofiguration file
    /// @throws std::runtime_error if something goes wrong during loading
    virtual void load(const YAML::Node& node) {};

    /// @brief Validate if the node is defined correctly.
    /// @throws std::runtime_error if the validation fails.
    virtual void validateRules() {};

   protected:
    /// @brief Name of the key indcating the node in the configuration file.
    std::string key;
};

}  // namespace config
}  // namespace flash