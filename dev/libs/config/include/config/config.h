//
// Created by benedikt on 29.03.24.
//

#ifndef LIGHTNING_CONFIG_H
#define LIGHTNING_CONFIG_H

#include <iostream>
#include <map>
#include <cstdint>
#include <string>
#include <filesystem>
#include <algorithm>
//#include <format>

#include <yaml-cpp/yaml.h>
#include <types/types.h>


namespace config{


    /**
     * @brief Throw when an invalid key is detected.
     */
    class InvalidKeyException : public std::exception {
        private:
            std::string message;

        public:
            InvalidKeyException(std::string msg) : message(msg) {}
            std::string what () {
                return message;
            }
    };


    /**
     * @brief Throw when an invalid configuration is detected.
     */
    class InvalidConfigurationException : public std::exception {
        private:
            std::string message;

        public:
            InvalidConfigurationException(std::string msg) : message(msg) {}
            std::string what () {
                return message;
            }
    };

    /// Options for keys in config.yaml.
    enum Key {
        seed,
        rSampling,
        kSampling, 
        nOccupied,
        nUnoccupied,
        cISDF,
        cvtMaximumSteps,
        cvtConvergenceTolerance,
        nISDFOccupiedUnoccupied,
        nISDFOccupied,
        nISDFUnoccupied,
        nLanczosIterations
    };

    

    /// Options for random seed.
    enum class Seed { fixed, time };


    class Config {

        private:

            /// Seed for ranomization.
            Seed seed{Seed::fixed};

            /// Real space sampling of wave functions.
            typ::SizeTriple rSampling{{10, 10, 10}};

            /// Reciprocal space sampling.
            typ::SizeTriple kSampling{{1, 1, 1}};

            /// Number of occupied states.
            std::size_t nOccupied{0};

            /// Number of occupied states.
            std::size_t nUnoccupied{0};

            /// Constant to determine the number of interpolation points.
            std::float_t cISDF{0};

            /// Maximum steps of centroidal voronoi tessalation algorithm.
            std::size_t cvtMaximumSteps{1000};

            /// Tolerance for average deviation of interpolation points to previous iteration to be converged.
            std::float_t cvtConvergenceTolerance{1e-5};

            /// Number of interpolation points for occupied unoccupied pairing.
            std::size_t nISDFOccupiedUnoccupied{1};

            /// Number of interpolation points for occupied occupied pairing.
            std::size_t nISDFOccupied{1};

            /// Number of interpolation points for unoccupied unoccupied pairing.
            size_t nISDFUnoccupied{0};

            /// Number of Lanczos iterations.
            std::size_t nLanczosIterations{100};

        public:

            /**
             * @brief Default constructor.
             * 
             * @return Instance initialized with default values.
             */
            Config();

            /**
             * @brief Constructor from attributes.
             * 
             * @param[in] seed_in
             * @param[in] rSampling_in
             * @param[in] kSampling_in
             * @param[in] nOccupied_in
             * @param[in] nUnoccupied_in
             * @param[in] cISDF_in
             * @param[in] cvtMaximumSteps_in
             * @param[in] cvtConvergenceTolerance_in
             * @param[in] nISDFOccupiedUnoccupied_in
             * @param[in] nISDFOccupied_in
             * @param[in] nISDFUnoccupied_in
             * @param[in] nLanczosIterations_in
             * @return Instance initialized from attributes.
             */
            Config(
                const Seed &seed_in,
                const typ::SizeTriple &rSampling_in,
                const typ::SizeTriple &kSampling_in,
                const std::size_t &nOccupied_in,
                const std::size_t &nUnoccupied_in,
                const std::float_t &cISDF_in,
                const std::size_t &cvtMaximumSteps_in,
                const std::float_t &cvtConvergenceTolerance_in,
                const std::size_t &nISDFOccupiedUnoccupied_in,
                const std::size_t &nISDFOccupied_in,
                const std::size_t &nISDFUnoccupied_in,
                const std::size_t &nLanczosIterations_in
            );

            /**
             * @brief Initialize instance from a YAML node.
             * 
             * Load the the configuration from a YAML node that can be generatet ,_e.g._,
             * from a yaml file. If an attribute is not defined in \p configYaml node, the
             * value is initialized from \p defaultConfig.
             * 
             * @param[in] configYaml Yaml node that contains the configuration.
             * @param[in] defaultConfig Instance with default values.
             * @return instance initialized with values from config.yaml
             * @throws InvalidConfigurationException
             */
            Config(const YAML::Node &configYaml, const Config &defaultConfig = Config{});

            /**
             * @brief Verify if two instances are the same.
             * 
             * @return True if all attributes are the same.
             */
            bool operator == (const Config &rhs);


            /**
             * @brief Return YAML node from Config instance.
             * 
             * @return YAML node containing the configuration.
             */
            YAML::Node toYamlNode();

            /**
             * Print the configuration to std::out
             */
            void print_to_stdout();

            /**
             * @brief Return number of real space sampling points
             * 
             * @return Number of real space sampling points
             */
            std::size_t nRpoints();

            /**
             * @brief Return number of reziprocal space sampling points
             * 
             * @return Number of reciprocal sampling points
             */
            std::size_t nKpoints();
           
    };


    static const Config defaultConfig{};

  
}





#endif //LIGHTNING_CONFIG_H
