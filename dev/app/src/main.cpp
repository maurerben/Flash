#include <H5Cpp.h>
#include <config/nodes/Grid.h>
#include <config/nodes/Input.h>
#include <config/nodes/Output.h>
#include <config/parameters/Vector.h>
#include <utils/IndexMap.h>
#include <dataProcessing/access.h>

#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>
#include <array>
#include <boost/program_options.hpp>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <map>

namespace po = boost::program_options;
namespace fc = flashlight::config;
namespace fd = flashlight::dataProcessing;

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
fc::nodes::Input parseInputConfiguration(std::string configFile) {
    auto config = YAML::LoadFile(configFile);

    fc::nodes::Input input("input");
    input.load(config);

    return input;
}

/**
 * @brief Parse output configuration.
 */
fc::nodes::Output parseOutputConfiguration(std::string configFile) {
    auto config = YAML::LoadFile(configFile);

    fc::nodes::Output output("output");
    output.load(config);

    return output;
}



int main(int argc, char* argv[]) {
    using namespace flashlight::utils;

    std::cout << "Hello World" << std::endl;
    auto cmdArgs = parseCommandLine(argc, argv);

    // Implementing independent particle apprixmation

    // 1. Read $\epsilon_{i\mathbf{k}}$
    const std::string filename = "electrons.h5"; // yet a constant
    index_t numKpoints = 0;
    index_t numElectrons = 0;
    vector_t<real_t> energies;
    vector_t<real_t> occupations;
    auto file = H5::H5File(filename, H5F_ACC_RDONLY);
    fd::readElectrons(file, numKpoints, numElectrons,
                      energies, occupations);




    return 0;
}