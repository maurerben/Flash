#define CATCH_CONFIG_MAIN
#include <config/nodes/Grid.h>
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
        grid:
            sampling: [1, 2, 3]
            offset: [0.1, 0.3, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]  
        
        missingOffset:
            sampling: [1, 2, 3]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]
        
        missingParallelepiped:
            sampling: [1, 2, 3]
            offset: [0.1, 0.3, 0.2]
        
        onlySampling:
            sampling: [1, 2, 3]
    
    invalid:
        missingSampling:
            offset: [0.1, 0.3, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]  
        
        samplingSmallerOne:
            sampling: [1, 0, 1]
            offset: [0.1, 0.3, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]  
        
        offsetSmallerZero:
            sampling: [1, 2, 3]
            offset: [-0.1, 0.3, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]  
        
        offsetEqualOne:
            sampling: [1, 2, 3]
            offset: [0.1, 1.0, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]  
        
        latticeLinearDependend:
            sampling: [1, 2, 3]
            offset: [0.1, 1.0, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 1, 0]
                - [0, 1, 0]  
        
        invalidSamplingForm:
            sampling: [1, 1, 1, 5]
            offset: [0.1, 0.3, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]  
        
        invalidOffsetForm:
            sampling: [1, 0, 1]
            offset: [0.1, 0.2]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1, 0]  
        
        invalidParallelepipedForm:
            sampling: [1, 0, 1]
            offset: [0.1, 0.2, 0.3]
            parallelepiped: 
                - [1, 0, 0]
                - [0, 0, 1]
                - [0, 1]
    )"
);

TEST_CASE("Test flash::config::nodes::RegularGrid class") {
    // RegularGrid loads from config file node with full definition
    Eigen::Vector<std::size_t, 3> referenceSampling{1, 2, 3};
    Eigen::Vector<std::double_t, 3> referenceOffset{0.1, 0.3, 0.2};
    Eigen::Matrix<std::double_t, 3, 3> referenceParallelepiped;
    referenceParallelepiped << 1, 0, 0, 0, 0, 1, 0, 1, 0;
    fcn::RegularGrid grid("grid");
    grid.load(configFile["valid"]);
    REQUIRE(grid.sampling == referenceSampling);
    REQUIRE(grid.offset == referenceOffset);
    REQUIRE(grid.parallelepiped == referenceParallelepiped);

    // RegularGrid loads from config file with missing offset definition
    auto defaultOffset = flash::constants::tensor::origin;
    fcn::RegularGrid missingOffset("missingOffset");
    missingOffset.load(configFile["valid"]);
    REQUIRE(missingOffset.sampling == referenceSampling);
    REQUIRE(missingOffset.offset == defaultOffset);
    REQUIRE(missingOffset.parallelepiped == referenceParallelepiped);

    // RegularGrid loads from config file with missing parallelepiped definition
    auto defaultParallelepiped = flash::constants::tensor::identity3D;
    fcn::RegularGrid missingParallelepiped("missingParallelepiped");
    missingParallelepiped.load(configFile["valid"]);
    REQUIRE(missingParallelepiped.sampling == referenceSampling);
    REQUIRE(missingParallelepiped.offset == referenceOffset);
    REQUIRE(missingParallelepiped.parallelepiped == defaultParallelepiped);

    // RegularGrid loads from config file with only sampling defined
    fcn::RegularGrid onlySampling("onlySampling");
    onlySampling.load(configFile["valid"]);
    REQUIRE(onlySampling.sampling == referenceSampling);
    REQUIRE(onlySampling.offset == defaultOffset);
    REQUIRE(onlySampling.parallelepiped == defaultParallelepiped);

    // Regular node attempts to load from missing node throws std::runtime_error
    fcn::RegularGrid missingNode("notAKey");
    REQUIRE_THROWS_AS(missingNode.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from a node not defining sampling throws std::runtime_error
    fcn::RegularGrid missingSampling("missingSampling");
    REQUIRE_THROWS_AS(missingSampling.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from a node defining sampling < 1 throws std::runtime_error
    fcn::RegularGrid samplingSmallerOne("samplingSmallerOne");
    REQUIRE_THROWS_AS(samplingSmallerOne.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from a node defining offset < 0.0 throws std::runtime_error
    fcn::RegularGrid offsetSmallerZero("offsetSmallerZero");
    REQUIRE_THROWS_AS(offsetSmallerZero.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from a node defining offset == 1.0 throws std::runtime_error
    fcn::RegularGrid offsetEqualOne("offsetEqualOne");
    REQUIRE_THROWS_AS(offsetEqualOne.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from node defining parallelepiped linear dependent throws std::runtime_error
    fcn::RegularGrid latticeLinearDependend("latticeLinearDependend");
    REQUIRE_THROWS_AS(latticeLinearDependend.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from node defining sampling with four elements throws std::runtime_error
    fcn::RegularGrid invalidSamplingForm("invalidSamplingForm");
    REQUIRE_THROWS_AS(invalidSamplingForm.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from node defining offset with two elements throws std::runtime_error
    fcn::RegularGrid invalidOffsetForm("invalidOffsetForm");
    REQUIRE_THROWS_AS(invalidOffsetForm.load(configFile["invalid"]), std::runtime_error);

    // Regular node attempts to load from node defining the last row of parallelepiped with two elements throws std::runtime_error
    fcn::RegularGrid invalidParallelepipedForm("invalidParallelepipedForm");
    REQUIRE_THROWS_AS(invalidParallelepipedForm.load(configFile["invalid"]), std::runtime_error);
}
