#!/bin/bash
cd bin
#make clean
rm dungeon
cmake -DCMAKE_BUILD_TYPE=Debug ../
make -j 9
echo "== DONE =="
