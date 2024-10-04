//
// Created by benedikt on 29.03.24.
//

#include <data_processing/config.h>

using namespace flash;

inline std::map<Seed, std::string> seedToStringDefault{{Seed::fixed, "fixed"}, {Seed::clock, "clock"}};

inline std::map<std::string, Seed> stringToSeedDefault{{"fixed", Seed::fixed}, {"clock", Seed::clock}};

Seed nodeToSeed(const YAML::Node &node, const std::string &key, const Seed &defaultValue,
                const std::map<std::string, Seed> &stringToSeed = stringToSeedDefault) {
    try {
        return stringToSeed.at(node[key].as<std::string>());
    } catch (YAML::InvalidNode) {
        return defaultValue;
    } catch (std::out_of_range) {
        throw InvalidConfigurationException("Invalid option for seed.");
    }
}

template <typename T>
T nodeToValue(const YAML::Node &node, std::string msg) {
    try {
        return node.as<T>();
    } catch (...) {
        throw InvalidConfigurationException(msg);
    }
}

template <typename T>
T nodeToValueWithDefault(const YAML::Node &node, const std::string &key, const T &defaultValue, std::string msg) {
    try {
        return node[key].as<T>();
        ;
    } catch (YAML::InvalidNode) {
        return defaultValue;
    } catch (...) {
        throw InvalidConfigurationException(msg);
    }
}

template <typename T, size N>
auto nodeToArray(const YAML::Node &node, bool (*rule)(T value), std::string msg) {
    try {
        std::array<T, N> values;
        size idx = 0;
        for (auto element : node) {
            values[idx] = node.as<T>();
        }
        return values;
    } catch (...) {
        throw InvalidConfigurationException(msg);
    }
}

Config::Config(const YAML::Node &configYamlNode, const std::vector<std::string> &allowedKeys,
               const std::vector<std::string> &mandatoryKeys, const DefaultConfig &defaults) {
    // extract keys from yaml node
    std::vector<std::string> keysInNode;
    for (auto kv : configYamlNode) {
        auto key = kv.first.as<std::string>();
        keysInNode.push_back(key);
    }
    // verify that all keys from keysInNode that are in allowedKeys
    for (auto key : keysInNode) {
        if (std::find(allowedKeys.begin(), allowedKeys.end(), key) == allowedKeys.end()) {
            throw InvalidKeyException("Configuration has a forbidden parameter: " + key + " = " +
                                      configYamlNode[key].as<std::string>());
        }
    }

    // verify that keys from mandatoryKeys are in keysInNode
    for (auto key : mandatoryKeys) {
        if (std::find(keysInNode.begin(), keysInNode.end(), key) == keysInNode.end()) {
            throw MissingKeyException("Configuration is missing a mandatory parameter: " + key);
        }
    }

    try {
        n_isdf_vexc = nodeToValue<size>(configYamlNode["n_isdf_vexc"], "n_isdf_vexc is wrongly initialized.");
        n_isdf_wscr_occupied =
            nodeToValue<size>(configYamlNode["n_isdf_wscr_occupied"], "n_isdf_wscr_occupied is wrongly initialized.");
        n_isdf_wscr_unoccupied = nodeToValue<size>(configYamlNode["n_isdf_wscr_unoccupied"],
                                                   "n_isdf_wscr_unoccupied is wrongly initialized.");
        max_lanczos_iterations = nodeToValue<size>(configYamlNode["max_lanczos_iterations"],
                                                   "max_lanczos_iterations is wrongly initialized.");
        omega_range =
            nodeToValue<std::array<real, 2>>(configYamlNode["omega_range"], "omega_range is wrongly initialized.");
        n_omega = nodeToValue<size>(configYamlNode["n_omega"], "n_omega is wrongly initialized.");
        max_cvt_iterations =
            nodeToValueWithDefault<size>(configYamlNode, "max_cvt_iterations", defaults.max_cvt_Itereations,
                                         "max_cvt_iterations is wrongly initialized.");
        cvt_convergence_criterium = nodeToValueWithDefault<real>(configYamlNode, "cvt_convergence_criterium",
                                                                 defaults.cvt_convergence_criterium,
                                                                 "cvt_convergence_criterium is wrongly initialized.");
        seed_source = nodeToSeed(configYamlNode, "seed_source", defaults.seed_source);
    } catch (const InvalidConfigurationException) {
        throw;
    }
}

Config::Config(const std::string &configYamlFile, const std::vector<std::string> &allowedKeys,
               const std::vector<std::string> &mandatoryKeys, const DefaultConfig &defaults) {
    YAML::Node configYamlNode = YAML::LoadFile("config.yaml");
    Config(configYamlNode, allowedKeys, mandatoryKeys, defaults);
};

void Config::sanityChecks() {
    // Rule for n_k_points
    if (!n_k_points > 0) {
        throw InvalidConfigurationException("n_k_points must be > 0.");
    }
    // Rule for n_occupied_total
    if (!n_occpied_total > 0) {
        throw InvalidConfigurationException("n_occpied_total must be > 0.");
    }
    // Rule for n_unoccupied_total
    if (!n_unoccupied_total > 0) {
        throw InvalidConfigurationException("n_unoccupied_total must be > 0.");
    }
    // Rule for n_occupied_total + n_unoccupied_total
    if (!(n_unoccupied_total + n_occpied_total) % n_k_points == 0) {
        throw InvalidConfigurationException("n_occpied_total + n_unoccupied_total must be divisable by n_k_points.");
    }
    // Rules for n_isdf_vexc
    if (!n_isdf_vexc > 0) {
        throw InvalidConfigurationException("n_isdf_vexc must be > 0.");
    }
    if (!n_isdf_vexc <= n_occpied_total * n_unoccupied_total / n_k_points) {
        throw InvalidConfigurationException(
            "n_isdf_vexc must be <= n_occpied_total * n_unoccupied_total / n_k_points (" +
            std::to_string(n_occpied_total * n_unoccupied_total / n_k_points) + ").");
    }
    // Rules for n_isdf_wscr_occupied
    if (!n_isdf_wscr_occupied > 0) {
        throw InvalidConfigurationException("n_isdf_wscr_occupied must be > 0.");
    }
    if (!n_isdf_wscr_occupied <= n_occpied_total * *2) {
        throw InvalidConfigurationException("n_isdf_wscr_occupied must be <= n_occpied_total ** 2 (" +
                                            std::to_string(n_occpied_total * *2) + ").");
    }
    // Rules for n_isdf_wscr_unoccupied
    if (!n_isdf_wscr_unoccupied > 0) {
        throw InvalidConfigurationException("n_isdf_wscr_unoccupied must be > 0.");
    }
    if (!n_isdf_wscr_unoccupied <= n_unoccupied_total * *2) {
        throw InvalidConfigurationException("n_isdf_wscr_unoccupied must be <= n_occpied_total ** 2 (" +
                                            std::to_string(n_unoccupied_total * *2) + ").");
    }
    // Rule for max_lanczos_iterations
    if (!max_lanczos_iterations > 0) {
        throw InvalidConfigurationException("max_lanczos_iterations must be > 0.");
    }
    // Rule for omega_range
    if (!omega_range[0] < omega_range[1]) {
        throw InvalidConfigurationException(
            "Lower limit of omega_range (left value) must be smaller then the upper limit (right value).");
    }
    // Rule for n_omega
    if (!n_omega > 0) {
        throw InvalidConfigurationException("n_omega must be > 0.");
    }
    // Rule for max_cvt_iterations
    if (!cvt_convergence_criterium > 0) {
        throw InvalidConfigurationException("cvt_convergence_criterium must be > 0.");
    }
}
