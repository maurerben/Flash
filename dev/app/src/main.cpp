#include <config/nodes/Input.h>
#include <config/nodes/Output.h>
#include <config/nodes/Grid.h>
#include <config/parameters/Vector.h>
#include <utils/IndexMap.h>
#include <dataProcessing/hdf5.h>
#include <physics/electrons/Data.h>
#include <physics/electrons/Index.h>

#include <yaml-cpp/yaml.h>
#include <H5Cpp.h>
#include <Eigen/Dense>
#include <boost/program_options.hpp>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <map>
#include <array>

namespace po = boost::program_options;
namespace fc = flashlight::config;
namespace fp = flashlight::physics;
namespace fu = flashlight::utils;

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

fp::electrons::Data readElectrons(fc::nodes::Input inputConfig, std::string filename) {
  auto data = fp::electrons::Data();
  try{
      auto file = H5::H5File(filename, H5F_ACC_RDONLY);
      data = fp::electrons::Data(file);
  } catch (const std::exception& e) {
      std::cerr << "Error reading electrons from file " << filename << "\n";
      std::cout << e.what() << "\n";
  }


  if (inputConfig.electronicStates.nBands != data.NumBands()) {
      std::cerr << "Number of bands given in inputConfig is " + std::to_string(inputConfig.electronicStates.nBands) + ".\n";
      std::cerr << "Number of bands obtained from " + filename + " is " + std::to_string(data.NumBands()) + ".\n";
      throw std::exception();
  }
  if (inputConfig.electronicStates.kGrid.sampling.prod() != data.NumKpoints()) {
      std::cerr << "Number of k-points given in inputConfig is " +
                     std::to_string(inputConfig.electronicStates.kGrid.sampling.prod()) + ".\n";
      std::cerr << "Number of k-points obtained from " + filename + " is " + std::to_string(data.NumKpoints()) + ".\n";
      throw std::exception();
  }

  return data;

}

int main(int argc, char* argv[])
{
    std::cout<<"Hello World"<<std::endl;
    auto cmdArgs = parseCommandLine(argc, argv);

    auto inputConfig = parseInputConfiguration(cmdArgs["input-config"]);
    auto outputConfig = parseOutputConfiguration(cmdArgs["output-config"]);

    auto electronData = readElectrons(inputConfig, "electrons.h5");
    std::cout<<"electrons read"<<std::endl;



    return 0;
}