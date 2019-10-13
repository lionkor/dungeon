#!/bin/bash
cd bin
#make clean
rm dungeon
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j 9
echo "== DONE =="
