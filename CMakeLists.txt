cmake_minimum_required(VERSION 3.13)
project(GraphPartitioning)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp src/partitioning_algorithms.cpp src/partitioning_algorithms.h
        src/Graph.cpp src/Graph.h src/Node.cpp src/Node.h)
add_executable(converter src/tools/convertitore_ascii_bin.cpp)
add_executable(readbin src/tools/read_bin.cpp)

