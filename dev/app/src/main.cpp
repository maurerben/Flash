#include <config/nodes/Grid.h>
#include <config/parameters/Matrix.h>
#include <config/parameters/Option.h>
#include <config/parameters/Scalar.h>
#include <config/parameters/Vector.h>
#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <boost/program_options.hpp>
#include <complex>
#include <iostream>

namespace fcp = flash::config::parameters;
namespace fcn = flash::config::nodes;

int main(int argc, char* argv[]) {
    auto configFile = YAML::Load(
        R"(
            myFirstParam: 3
            mySecondParam: 3
            myThirdParam: [1, 2, 3, 4]
            myFourthParam: 
                - a
                - [1, 2, 3]
            myGrid:
                sampling: [1, 2, 4]
                offset: [0.5, 0.5, 0.5]
                lattice:
                    - [1, 0, 0]
                    - [0, 0, 1]
                    - [0, 1, 0]      
            greeting: hello       
        )");

    fcp::Scalar<int> myFirstParam("myFirstParam");
    fcp::Scalar<float> mySecondParam("mySecondParam", 3.14);
    fcp::Vector<int, 4> myThirdParam("myThirdParam");
    fcp::Matrix<int, 2, 3> myFourthParam("myFourthParam");

    fcp::options_t options{"hello", "hi"};
    fcp::Option myStringParam("greeting", options, "hi");

    fcn::RegularGrid myGrid("myGrid");

    myFirstParam.load(configFile);
    mySecondParam.load(configFile);
    myThirdParam.load(configFile);
    myFourthParam.load(configFile);
    myStringParam.load(configFile);
    // std::cout << 2 << std::endl;
    myGrid.load(configFile);
    // std::cout << 2 << std::endl;

    // std::cout << myFirstParam << std::endl;
    // std::cout << mySecondParam << std::endl;
    // std::cout << myThirdParam << std::endl;
    // std::cout << myFourthParam << std::endl;
    // std::cout << myGrid.Lattice << std::endl;
    std::cout << myStringParam << std::endl;
    // std::cout << configFile["greeting"] << std::endl;

    // fcp::Scalar<std::string> myStringParam("cplx");
    // myStringParam.load(YAML::Load("cplx: 1,));

    // auto z = std::complex<int>("1 + 2j")

    // std::cout<<myStringParam<<std::endl;
    // std::cout<<std::complex<int>(1, 2)<<std::endl;

    return 0;
}