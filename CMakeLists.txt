cmake_minimum_required(VERSION 3.13)
project(sonar_scanner_example)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp src/partitioning_algorithms.cpp src/partitioning_algorithms.h src/graph.h src/node.h)
add_executable(converter src/tools/convertitore_ascii_bin.cpp)
add_executable(readbin src/tools/read_bin.cpp)

