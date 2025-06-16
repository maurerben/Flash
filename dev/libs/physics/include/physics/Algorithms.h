//
// Created by bene on 5/8/25.
//

#pragma once

#include <utils/types.h>
#include <utils/assert.h>

#include <concepts>

namespace flashlight {
namespace physics {
namespace algorithms {

using namespace utils;

//template<std::arithmetic T> TODO: Fix error: ‘std::arithmetic’ has not been declared
template<typename T>
void differenceMatrix(const vector_t<T>& A, const vector_t<T>& B, matrix_t<T> C);

}
}
}
