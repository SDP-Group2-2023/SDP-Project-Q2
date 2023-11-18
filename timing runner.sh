#!/bin/bash

which time

if ![$? -eq 0]
then
    echo "Install a package that provides timing functionality"
    exit 1
fi

rm -rf TESTS

mkdir TESTS
cd TESTS

for i in {1..10}
do
    mkdir $i
    cd $i
    /usr/bin/time -v ../../build/Memory_Tester ../../graphs/Graph-A.bin reading 2> time_reading-A.txt
    /usr/bin/time -v ../../build/Memory_Tester ../../graphs/Graph-A.bin sequential 2> time_sequential-A.txt

    /usr/bin/time -v ../../build/Memory_Tester ../../graphs/Graph-B.bin reading 2> time_reading-B.txt
    /usr/bin/time -v ../../build/Memory_Tester ../../graphs/Graph-B.bin sequential 2> time_sequential-B.txt

    /usr/bin/time -v ../../build/Memory_Tester ../../graphs/Graph-C.bin reading 2> time_reading-C.txt
    /usr/bin/time -v ../../build/Memory_Tester ../../graphs/Graph-C.bin sequential 2> time_sequential-C.txt
    mkdir threads
    cd threads
    for j in {1..8}
    do
    mkdir $j
    cd $j
    /usr/bin/time -v ../../../../build/Memory_Tester ../../../../graphs/Graph-A.bin parallel $j 2> time-A.txt
    /usr/bin/time -v ../../../../build/Memory_Tester ../../../../graphs/Graph-B.bin parallel $j 2> time-B.txt
    /usr/bin/time -v ../../../../build/Memory_Tester ../../../../graphs/Graph-C.bin parallel $j 2> time-C.txt
    cd ..
    done
    cd ..
    cd ..
done

exit
