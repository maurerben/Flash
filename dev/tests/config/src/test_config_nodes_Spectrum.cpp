#define CATCH_CONFIG_MAIN
#include <config/nodes/Spectrum.h>
#include <constants/tensor.h>
#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <catch2/catch_all.hpp>
#include <complex>
#include <cstdint>
#include <iomanip>
#include <numbers>
#include <sstream>
#include <stdexcept>
#include <string>

namespace fcn = flash::config::nodes;

auto configFile = YAML::Load(
    R"(
    valid:
        spectrum:
            energy_interval: [1.0, 2.0]
            n_sampling: 1500
            peak:
                width: 0.002
                form: gauss

        missingPeak:
            energy_interval: [1.0, 2.0]
            n_sampling: 1500

    invalid:
        missingEnergyInterval:
            n_sampling: 1500
            peak:
                width: 0.002
                form: gauss

        missingNSampling:
            energy_interval: [1.0, 2.0]
            peak:
                width: 0.002
                form: gauss

        inValidEnergyInterval:
            energy_interval: [1.0, 1.0]
            n_sampling: 1500
            peak:
                width: 0.002
                form: gauss

        nSamplingEqual0:
            energy_interval: [1.0, 2.0]
            n_sampling: 0
    )"
);

TEST_CASE("Test flash::config::nodes::Spectrum class") {
    // Spectrum loads from config file node with full definition
    Eigen::Vector<std::double_t, 2> referenceEnergyInterval{1.0, 2.0};
    std::size_t referenceNSamplingPoints{1500};
    std::double_t referenceWidth{0.002};
    std::string referenceForm{"gauss"};
    fcn::Spectrum spectrum("spectrum");
    spectrum.load(configFile["valid"]);
    REQUIRE(spectrum.energyInterval == referenceEnergyInterval);
    REQUIRE(spectrum.nSampling == referenceNSamplingPoints);
    REQUIRE(spectrum.peak.width == referenceWidth);
    REQUIRE(spectrum.peak.form == referenceForm);

    // Spectrum loads from config file node with missing peak definition
    auto defaultWidth = fcn::defaultWidth;
    auto defaultForm = fcn::defaultForm;
    fcn::Spectrum missingPeak("missingPeak");
    missingPeak.load(configFile["valid"]);
    REQUIRE(missingPeak.energyInterval == referenceEnergyInterval);
    REQUIRE(missingPeak.nSampling == referenceNSamplingPoints);
    REQUIRE(missingPeak.peak.width == defaultWidth);
    REQUIRE(missingPeak.peak.form == defaultForm);

    // Spectrum attempt to load from missing node throws std::runtime_error
    fcn::Spectrum missingNode("notAKey");
    REQUIRE_THROWS_AS(missingNode.load(configFile["invalid"]), std::runtime_error);

    // Spectrum attempt to load from node with missing energy interval definition throws std::runtime_error
    fcn::Spectrum missingEnergyInterval("missingEnergyInterval");
    REQUIRE_THROWS_AS(missingEnergyInterval.load(configFile["invalid"]), std::runtime_error);

    // Spectrum attempt to load from node with n sampling interval definition throws std::runtime_error
    fcn::Spectrum missingNSampling("missingNSampling");
    REQUIRE_THROWS_AS(missingNSampling.load(configFile["missingNSampling"]), std::runtime_error);

    // Spectrum attempt to load from node with invalid energy interval definition throws std::runtime_error
    fcn::Spectrum inValidEnergyInterval("inValidEnergyInterval");
    REQUIRE_THROWS_AS(inValidEnergyInterval.load(configFile["invalid"]), std::runtime_error);

    // Spectrum attempt to load from node with n sampling defined as zero throws std::runtime_error
    fcn::Spectrum nSamplingEqual0("nSamplingEqual0");
    REQUIRE_THROWS_AS(nSamplingEqual0.load(configFile["invalid"]), std::runtime_error);
}
