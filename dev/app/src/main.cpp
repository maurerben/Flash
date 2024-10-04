#include <constants/constants.h>
#include <data_processing/config.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace flash;

int main() {
    
    try {
        Config params("config.yaml");
        std::cout<<params.n_isdf_wscr_occupied <<" "<< params.n_occupied_total<<" "<<params.n_unoccupied_total<<std::endl;
        params.validate();
    } catch (InvalidConfigurationException ice) {
        std::cout << "Caught InvalidConfigurationException: " << ice.what() << std::endl;
    } catch (...) {
        std::cout << "Caught an unknown runtime error." << std::endl;
    }
}
