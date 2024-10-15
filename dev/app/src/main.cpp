

// #include <core/data/root.h>

#include <configParameters/MatrixParameter.h>
#include <configParameters/RegularGrid.h>
#include <configParameters/ScalarParameter.h>
#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <boost/program_options.hpp>
#include <iostream>
#include <complex>

namespace fip = flash::configParameters;

int main(int argc, char* argv[]) {
    auto configNode = YAML::Load(
        R"(
            myFirstParam: 3
            myThirdParam: [1, 2, 4, 3]
            myFourthParam: 
                - [1, 2, 3]
                - [4, 5, 6]
                - [7, 8, 9]
        )"
    );


    

    auto node2 = configNode["bla"];

    fip::ScalarParameter<int> myFirstParam("myFirstParam");
    fip::ScalarParameter<float> mySecondParam("mySecondParam", 3.14);
    // fip::MatrixParameter<int, 4, 1> myThirdParam("myThirdParam");
    // fip::MatrixParameter<int, 3, 3> myFourthParam("myFourthParam");

    std::cout << 1 << std::endl;
    myFirstParam.load(configNode);
    std::cout << 1 << std::endl;
    mySecondParam.load(configNode);
    std::cout << 1 << std::endl;

    std::cout << !configNode["myThirdParam"].IsSequence() << std::endl;
    std::cout << configNode["mySecondParam"].size() << std::endl;
    std::cout << myFirstParam << std::endl;
    std::cout << mySecondParam << std::endl;
    // std::cout << myFourthParam << std::endl;
    
    

    // fip::ScalarParameter<std::string> myStringParam("cplx");
    // myStringParam.load(YAML::Load("cplx: 1,));

    // auto z = std::complex<int>("1 + 2j")
    
    // std::cout<<myStringParam<<std::endl;
    // std::cout<<std::complex<int>(1, 2)<<std::endl;

    return 0;
}