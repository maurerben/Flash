//
// Created by benedikt on 29.03.24.
//

#ifndef LIGHTNING_ELECTRONS_H
#define LIGHTNING_ELECTRONS_H


#include <cstdint>
#include <Eigen/Dense>
#include <types/types.h>



    
namespace electron{


    typedef Eigen::Matrix<std::double_t, Eigen::Dynamic, Eigen::Dynamic> DMatrix;
    typedef Eigen::Matrix<typ::Complex, Eigen::Dynamic, Eigen::Dynamic> CMatrix;


    class ElectronSystem {

        private:

            DMatrix energies;
            DMatrix occupation;
            std::size_t nKPoints;
            std::size_t nStates;

        public:

            ElectronSystem(const DMatrix $energies, const DMatrix $occupation, const std::size_t &nKPoints);

    };

    


  
}





#endif //LIGHTNING_ELECTRONS_H
