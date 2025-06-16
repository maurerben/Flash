//
// Created by bene on 5/8/25.
//

#include <physics/Algorithms.h>

using namespace flashlight::physics::algorithms;

template<typename T>
void differenceMatrix(const vector_t<T>& A, const vector_t<T>& B, matrix_t<T> C) {
    C.resize(A.size(), B.size());

    for (size_t i = 0; i < A.size(); ++i) {
      for (size_t j = 0; j < B.size(); ++j) {
        C(i, j) = A(i) - B(j);
      }
    }
}

void find_transitions(const matrix<real_t>& energies, const matrix<real_t>& occupations, matrix<real_t>& transitions) {

    assertTrue(energies.rows() == occupations.rows());
    assertTrue(energies.cols() == occupations.cols());

    transitions.resize(energies.cols()**2, occupations.rows());



    // transitions.resize(energies.size());
}