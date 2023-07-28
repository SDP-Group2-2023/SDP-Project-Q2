cmake_minimum_required(VERSION 3.13)
project(GraphPartitioning)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp
        src/partitioning_algorithms.cpp src/partitioning_algorithms.h
        src/Graph.cpp src/Graph.h src/loader.cpp)

add_executable(converter src/tools/ascii_bin_converter.cpp)

add_executable(read_bin src/tools/read_bin.cpp)

add_executable(sequential_benchmark src/sequential_benchmark.cpp
        src/partitioning_algorithms.cpp src/partitioning_algorithms.h
        src/Graph.cpp src/Graph.h src/loader.cpp)