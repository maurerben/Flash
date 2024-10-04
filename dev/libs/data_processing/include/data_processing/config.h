#ifndef FLASH_CONFIG_H
#define FLASH_CONFIG_H

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <cmath>

#include <yaml-cpp/yaml.h>

#include <types/types.h>

namespace flash {
namespace data_processing{
namespace config{

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
    static const std::vector<std::string> allowedKeys = {"n_k_points",
                                                         "n_occupied_total",
                                                         "n_unoccupied_total",
                                                         "n_isdf_vexc",
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
    static const std::vector<std::string> allowedKeys = {"n_k_points",
                                                         "n_occupied_total",
                                                         "n_unoccupied_total",
                                                         "n_isdf_vexc",
                                                         "n_isdf_wscr_occupied",
                                                         "n_isdf_wscr_unoccupied",
                                                         "max_lanczos_iterations",
                                                         "omega_range",
                                                         "n_omega"};
    return allowedKeys;
}

struct DefaultConfig {
    types::size max_cvt_Itereations = 1000;
    types::real_dp cvt_convergence_criterium = 1.e-5;
    Seed seed_source = Seed::clock;

    DefaultConfig() = default;
};

/// @brief Class for handling the configuration of the program
class Config {
   public:
    /**
     * @brief Number of **k**-points.
     */
    types::size n_k_points;

    /**
     * @brief Number of occupied bands cumulative over the **k**-points.
     * @details \p n_occupied_total + \p n_occupied_total must be divisable by \p n_k_points
     */
    types::size n_occupied_total;

    /**
     * @brief Number of unoccupied bands cumulative over the **k**-points.
     */
    types::size n_unoccupied_total;

    /**
     * @brief Number of interpolation points for occupied unoccupied pairing.
     * @details Must be <= \p n_occupied_total * \p n_unoccupied_total / \p n_k_points.
     */
    types::size n_isdf_vexc;

    /**
     * @brief Number of interpolation points for occupied unoccupied pairing.
     * @details Must be <= \p n_occupied_total ** 2.
     */
    types::size n_isdf_wscr_occupied;

    /**
     * @brief Number of interpolation points for occupied unoccupied pairing.
     * @details Must be <= \p n_unoccupied_total ** 2.
     */
    types::size n_isdf_wscr_unoccupied;

    /**
     * @brief Number of Lanczos iterations.
     * @details Must be <= \p n_occupied_total * \p n_unoccupied_total / \p n_k_points.
     */
    types::size max_lanczos_iterations;

    /**
     * @brief Energy range for calculating the spectrum.
     * @details The lower limit (left value) must be smaller than the upper limit (right value).
     */
    std::array<types::real_dp, 2> omega_range;

    /**
     * @brief Number of energy sampling points.
     */
    types::size n_omega;

    /**
     * @brief Max. iterations for CVT.
     */
    types::size max_cvt_iterations;

    /**
     * @brief CVT convergence criterium
     */
    types::real_dp cvt_convergence_criterium;

    /**
     * @brief Seed source for random number generation.
     * @details Must be one of
     *     - `"fixed"` for using a fixed seed
     *     - `"clock"` for using the system clock as seed.
     */
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
     * @throws InvalidKeyException
     * @throws MissingKeyException
     * @throws InvalidConfigurationException
     */
    Config(const YAML::Node &configYamlNode, const std::vector<std::string> &allowedKeys = getAllowedKeys(),
           const std::vector<std::string> &mandatoryKeys = getMandatoryKeys(),
           const DefaultConfig &defaults = DefaultConfig());

    /**
     * @brief Validate configurataions.
     * @details A valid configuration follows the rules described in the documentation of each attribute.
     * @throws InvalidConfigurationException
     */
    void validate();
};

}  // namespace config
}  // namespace data_processing
}  // namespace flash

#endif  // FLASH_CONFIG_H`
