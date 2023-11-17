cmake_minimum_required(VERSION 3.13)
project(GraphPartitioning)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g --print-memory-usage")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(GraphPartitioning src/main.cpp
        src/Graph.h src/Graph.cpp
        src/partitioning.h src/sequential_coarsening.cpp src/kernighanLin.cpp src/sequential_partitioning.cpp
        src/Change.h src/Change.cpp
        src/parallel_partitioning.cpp src/parallel_coarsening.cpp src/loader.cpp src/parallel_kernighanLin.cpp
        src/clusterCutSize.h src/clusterCutSize.cpp src/output.cpp)

add_executable(main src/menu.cpp
        src/Graph.h src/Graph.cpp
        src/Change.h src/Change.cpp
        src/partitioning.h src/sequential_coarsening.cpp src/kernighanLin.cpp src/sequential_partitioning.cpp
        src/parallel_partitioning.cpp src/parallel_coarsening.cpp src/loader.cpp src/parallel_kernighanLin.cpp
        src/clusterCutSize.h src/clusterCutSize.cpp src/output.cpp)


add_executable(Tester src/test/tester.cpp src/partitioning.h src/loader.cpp src/Graph.h src/Graph.cpp src/Graph.h src/Graph.cpp
        src/Change.h src/Change.cpp
        src/partitioning.h src/sequential_coarsening.cpp src/kernighanLin.cpp src/sequential_partitioning.cpp
        src/parallel_partitioning.cpp src/parallel_coarsening.cpp src/loader.cpp src/parallel_kernighanLin.cpp
        src/output.cpp src/Graph.h src/Graph.cpp
        src/clusterCutSize.h src/clusterCutSize.cpp src/output.cpp )