#!/bin/bash
cd bin
make clean
rm dungeon
cd ..
g++ $(find *.cpp) -o ./bin/dungeon -lsfml-window -lsfml-graphics -lsfml-system -lstdc++fs -std=c++17 -g
echo "== DONE =="
