cmake_minimum_required(VERSION 3.6)
project(BIN_EVO_projekt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -fopenmp")

set(SOURCE_FILES main.cpp CellularAutomata.cpp CellularAutomata.h NumberGenerator.cpp NumberGenerator.h Evolution.cpp Evolution.h Chromosome.cpp Chromosome.h params.h)
add_executable(BIN_EVO_projekt ${SOURCE_FILES})