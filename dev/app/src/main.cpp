#include <config/nodes/Input.h>
#include <config/nodes/Output.h>
#include <config/nodes/Grid.h>
#include <config/parameters/Vector.h>
#include <data_processing/hdf5.h>
#include <physics/electrons/ElectronicStates.h>
#include <physics/electrons/Index.h>
#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <boost/program_options.hpp>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <map>
#include <array>

namespace po = boost::program_options;
namespace fcp = flash::config::parameters;
namespace fcn = flash::config::nodes;
namespace fpe = flash::physics::electrons;

/**
 * @brief Parse command line arguments.
 */
std::map<std::string, std::string> parseCommandLine(int argc, char* argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("input-config,i", po::value<std::string>(),
                                                         "input configuration file")(
        "output-config,o", po::value<std::string>(), "output configuration file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        std::exit(0);
    }

    std::string inputConfigFilePath;
    if (vm.count("input")) {
        inputConfigFilePath = vm["input"].as<std::string>();
    } else {
        inputConfigFilePath = "config.yaml";
    }

    std::string outputConfigFilePath;
    if (vm.count("output")) {
        outputConfigFilePath = vm["output"].as<std::string>();
    } else {
        outputConfigFilePath = "config.yaml";
    }

    return std::map<std::string, std::string>{{"input-config", inputConfigFilePath},
                                              {"output-config", outputConfigFilePath}};
}

/**
 * @brief Parse input configuration.
 */
fcn::Input parseInputConfiguration(std::string configFile) {
    auto config = YAML::LoadFile(configFile);

    fcn::Input input("input");
    input.load(config);

    return input;
}

/**
 * @brief Parse output configuration.
 */
fcn::Output parseOutputConfiguration(std::string configFile) {
    auto config = YAML::LoadFile(configFile);

    fcn::Output output("output");
    output.load(config);

    return output;
}

int main(int argc, char* argv[])
{
    std::cout<<"Hello World"<<std::endl;
    auto cmdArgs = parseCommandLine(argc, argv);

    auto inputConfig = parseInputConfiguration(cmdArgs["input-config"]);
    auto outputConfig = parseOutputConfiguration(cmdArgs["output-config"]);

    fpe::ElectronicStates electrons{"electrons.h5", inputConfig.electronicStates};

    for (auto electron = electrons.begin(); electron != electrons.end(); ++electron) {
        auto [energy, occupation, kPoint, band] = *electron;
        std::cout << "Energy: " << energy << " Occupation: " << occupation << " kPoint: " << kPoint << " Band: " << band
                  << std::endl;
    }

    auto electronsAtK1 = electrons.atKPoint(1);
    auto occupiedElectrons = electrons.occupied();
    for (auto electron : occupiedElectrons) {
        auto [energy, occupation, kPoint, band] = electron;
        std::cout << "Energy: " << energy << " Occupation: " << occupation << " kPoint: " << kPoint << " Band: " << band
                  << std::endl;
    }

    return 0;
}