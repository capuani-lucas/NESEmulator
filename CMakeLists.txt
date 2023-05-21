cmake_minimum_required(VERSION 3.25)
project(NESEmulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(NESEmulator main.cpp lib/types/types.cpp lib/types/types.h lib/cpu/cpu.cpp lib/cpu/cpu.h)
