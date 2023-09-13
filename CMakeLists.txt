cmake_minimum_required(VERSION 3.13)
project(GraphPartitioning)

set(CMAKE_CXX_STANDARD 20)

add_executable(GraphPartitioning src/main.cpp
        src/Graph.h src/Graph.cpp
        src/partitioning.h src/sequential_coarsening.cpp src/kernighanLin.cpp src/sequential_partitioning.cpp
        src/Change.h src/Change.cpp 
        src/timing/timing.h src/timing/timing.cpp src/parallel_partitioning.cpp src/parallel_coarsening.cpp src/loader.cpp)

add_executable(ascii2bin src/tools/ascii_bin_converter.cpp)

add_executable(main src/menu.cpp
               src/Graph.h src/Graph.cpp
               src/Change.h src/Change.cpp
               src/timing/timing.h src/timing/timing.cpp
        src/partitioning.h src/sequential_coarsening.cpp src/kernighanLin.cpp src/sequential_partitioning.cpp
        src/parallel_partitioning.cpp src/parallel_coarsening.cpp src/loader.cpp)


add_executable(testColoring src/testColoring.cpp src/parallel_coarsening.cpp src/Graph.cpp src/Graph.h)