#ifndef LIGHTNING_CONFIG_H
#define LIGHTNING_CONFIG_H

#include <types/types.h>
#include <yaml-cpp/yaml.h>

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace flash {

/**
 * @brief Throw when an invalid key is detected.
 */
class InvalidKeyException : public std::exception {
   private:
    std::string message;

   public:
    InvalidKeyException(std::string msg) : message(msg) {}
    std::string what() { return message; }
};

/**
 * @brief Throw when a missing key is detected.
 */
class MissingKeyException : public std::exception {
   private:
    std::string message;

   public:
    MissingKeyException(std::string msg) : message(msg) {}
    std::string what() { return message; }
};

/**
 * @brief Throw when an invalid configuration is detected.
 */
class InvalidConfigurationException : public std::exception {
   private:
    std::string message;

   public:
    InvalidConfigurationException(std::string msg) : message(msg) {}
    std::string what() { return message; }
};

/// @brief Options for random seed.
enum class Seed { fixed, clock };

/// @brief Allowed Keys in config.yaml
static const std::vector<std::string> getAllowedKeys() {
    static const std::vector<std::string> allowedKeys = {"n_isdf_vexc",
                                                         "n_isdf_wscr_occupied",
                                                         "n_isdf_wscr_unoccupied",
                                                         "max_lanczos_iterations",
                                                         "omega_range",
                                                         "n_omega",
                                                         "max_cvt_iterations",
                                                         "cvt_convergence_criterium",
                                                         "seed_source"};
    return allowedKeys;
}

/// @brief Mandatory keys in config.yaml. These do not have a default value.
static const std::vector<std::string> getMandatoryKeys() {
    static const std::vector<std::string> allowedKeys = {
        "n_isdf_vexc", "n_isdf_wscr_occupied", "n_isdf_wscr_unoccupied", "max_lanczos_iterations", "omega_range",
        "n_omega"};
    return allowedKeys;
}

struct DefaultConfig {
    size maxCVTItereations = 1000;
    real CVTConvergenceCriterium = 1.e-5;
    Seed seedSource = Seed::clock;

    DefaultConfig() = default;
};

/// @brief Class for handling the configuration of the program
class Config {
   public:
    /// Number of interpolation points for occupied unoccupied pairing.
    size n_isdf_vexc;
    /// Number of interpolation points for occupied occupied pairing.
    size n_isdf_wscr_occupied;
    /// Number of interpolation points for unoccupied unoccupied pairing.
    size n_isdf_wscr_unoccupied;
    /// Number of Lanczos iterations.
    size max_lanczos_iterations;
    /// @brief Energy range for calculating the spectrum
    std::array<real, 2> omega_range;
    /// @brief Number of energy sampling points.
    size n_omega;
    /// @brief Max. iterations for CVT.
    size max_cvt_iterations;
    /// @brief CVT convergence criterium
    real cvt_convergence_criterium;
    /// @brief Seed source for random number generation.
    Seed seed_source;

    /**
     * @brief Initialize the configuration from a yaml node \p configYaml.
     *
     * @details Initialize the configuration from a yaml node \p configYaml and
     * check that the node only contains allowed keys \p allowedKeys and defines all
     * mandatory keys \p mandatoryKeys .
     * value is initialized from \p DefaultConfig.
     *
     * @param[in] configYaml Yaml node that contains the configuration.
     * @param[in] allowedKeys List of allowed keys.
     * @return instance initialized with values from config.yaml
     * @throws InvalidConfigurationException
     */
    Config(const YAML::Node &configYamlFile, const std::vector<std::string> &allowedKeys = getAllowedKeys(),
           const std::vector<std::string> &mandatoryKeys = getMandatoryKeys(),
           const DefaultConfig &defaults = DefaultConfig());

    /**
     * @brief Initialize instance from a YAML node.
     *
     * @details Open a yaml file and load the configuration from it. If an attribute is not defined in \p configYaml
     * node, the value is initialized from \p DefaultConfig.
     *
     * @param[in] configYaml Yaml node that contains the configuration.
     * @param[in] allowedKeys List of allowed keys.
     * @return instance initialized with values from config.yaml
     * @throws InvalidConfigurationException
     */
    // Config(const YAML::Node &configYamlFile, const std::vector<std::string> &allowedKeys = getAllowedKeys(), const
    // std::vector<std::string> &mandatoryKeys = getMandatoryKeys(), const DefaultConfig &defaults = DefaultConfig());
};

}  // namespace flash

#endif  // LIGHTNING_CONFIG_H`
