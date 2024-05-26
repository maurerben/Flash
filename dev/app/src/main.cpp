#include <iostream>

#include "config/config.h"
#include "constants/constants.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "Is this your constant?  " << precison::testConst << std::endl;
    return 0;
}
