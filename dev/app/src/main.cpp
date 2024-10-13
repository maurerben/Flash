

// #include <core/data/root.h>

#include <configParameters/parameter.h>
#include <yaml-cpp/yaml.h>

#include <boost/program_options.hpp>
#include <iostream>

namespace fip = flash::configParameters;

int main(int argc, char* argv[]) {
    auto configNode = YAML::Load(
        R"(
            myFirstParam: 3
        )");

    fip::Parameter<int> myFirstParam("myFirstParam");
    fip::Parameter<float> mySecondParam("mySecondParam", 3.14);
    fip::Parameter<char> myThirdParam("MyThirdParam");

    myFirstParam.loadValue(configNode);
    mySecondParam.loadValue(configNode);
    myThirdParam.loadValue(configNode);

    std::cout << myFirstParam << std::endl;
    std::cout << mySecondParam << std::endl;

    return 0;
}
