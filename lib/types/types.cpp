//
// Created by Lucas Capuani
//

#include <iostream>
#include "types.h"

void testSize () {
    std::cout << "Size of BYTE: " << sizeof(BYTE) * 8 << " bits" << std::endl;
    std::cout << "Size of WORD: " << sizeof(WORD) * 8 << " bits" <<  std::endl;
}