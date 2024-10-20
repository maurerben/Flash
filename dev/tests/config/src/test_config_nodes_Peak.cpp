#define CATCH_CONFIG_MAIN
#include <config/nodes/Peak.h>
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
        peak: 
            width: 0.002
            form: gauss

        missingWidth: 
            form: gauss

        missingForm: 
            width: 0.002

    invalid:
        widthEqualZero:
            width: 0.000
            form: lorentz

        formIsNotAnOption:
            width: 0.002
            form: sin
    )"
);

TEST_CASE("Test flash::config::nodes::Peak class") {
    // Peak loads from config file node with full definition
    std::double_t referenceWidth{0.002};
    std::string referenceForm{"gauss"};
    fcn::Peak peak("peak");
    peak.load(configFile["valid"]);
    REQUIRE(peak.width == referenceWidth);
    REQUIRE(peak.form == referenceForm);

    // Peak loads from config file node with undefined width
    auto defaultWidth = fcn::defaultWidth;
    fcn::Peak missingWidth("missingWidth");
    missingWidth.load(configFile["valid"]);
    REQUIRE(missingWidth.width == defaultWidth);
    REQUIRE(missingWidth.form == referenceForm);

    // Peak loads from config file node with undefined width
    auto defaultForm = fcn::defaultForm;
    fcn::Peak missingForm("missingForm");
    missingForm.load(configFile["valid"]);
    REQUIRE(missingForm.width == referenceWidth);
    REQUIRE(missingForm.form == defaultForm);

    // Peak attemts to load from missing key loads default values
    fcn::Peak missingNode("notAKey");
    missingNode.load(configFile["valid"]);
    REQUIRE(missingNode.width == defaultWidth);
    REQUIRE(missingNode.form == defaultForm);

    // Peak attempts to load from config file node with zero width throws std::runtime_error
    fcn::Peak widthEqualZero("widthEqualZero");
    REQUIRE_THROWS_AS(widthEqualZero.load(configFile["invalid"]), std::runtime_error);

    // Peak attempts to load from config file node defining a form that is not an option throws std::runtime_error
    fcn::Peak formIsNotAnOption("formIsNotAnOption");
    REQUIRE_THROWS_AS(formIsNotAnOption.load(configFile["invalid"]), std::runtime_error);

}
