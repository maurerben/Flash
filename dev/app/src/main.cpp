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
        Config config("config.yaml");
    } catch (InvalidKeyException ike) {
        std::cout << "Caught InvalidKeyException: " << ike.what() << std::endl;
    } catch (MissingKeyException mke) {
        std::cout << "Caught MissingKeyExceptionr " << mke.what() << std::endl;
    } catch (InvalidConfigurationException ice) {
        std::cout << "Caught InvalidConfigurationException: " << ice.what() << std::endl;
    } catch (...) {
        std::cout << "Caught an unknown runtime error." << std::endl;
    }
}
