#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <boost/program_options.hpp>
#include <yaml-cpp/yaml.h>

#include <constants/constants.h>
#include <data_processing/config.h>

namespace po = boost::program_options;
namespace dpc = flash::data_processing::config;
namespace tp = flash::types;

po::variables_map loadCommandLineArguments(int argc, char* argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("config-file,i", po::value<std::string>()->default_value("config.yaml"), "set path to a config.yaml");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        std::exception();
    }

    return vm;
}

dpc::Config setupConfigParameters(po::variables_map commandLineArguments){

    auto configFile = commandLineArguments["config-file"].as<std::string>();
    std::cout << "Load configuration from " << configFile << std::endl;
    try{

        auto configNode = YAML::LoadFile(configFile);
        auto params = dpc::Config(configNode);
        params.validate();
        return params;
        
    } catch (YAML::BadFile BF){
        std::cerr << "Flash is terminating: " << "Could not open " << configFile << std::endl;
    } catch (dpc::InvalidKeyException IKE){
        std::cerr << "Flash is terminating: " << IKE.what() << std::endl;
    } catch (dpc::MissingKeyException MKE){
        std::cerr << "Flash is terminating: " << MKE.what() << std::endl;
    } catch (dpc::InvalidConfigurationException ICE){
        std::cerr << "Flash is terminating: " << ICE.what() << std::endl;
    }

}

int main(int argc, char* argv[]) {

    auto commandLineArguments = loadCommandLineArguments(argc, argv);
    auto configParameters     = setupConfigParameters(commandLineArguments);

    return 0;
}
