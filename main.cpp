#include <iostream>

#include "lib/types/types.h"
#include "lib/cpu/cpu.h"
#include <bitset>

int main() {

    CPU_Registers registers;

    registers.PF = 0x0F;


    std::cout << std::bitset<8>(registers.C) << std::endl;
    return 0;
}
