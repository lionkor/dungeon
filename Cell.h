#ifndef CELL_H
#define CELL_H

#include "Global.h"
#include "Tile.h"

class Cell
{
public:
    Cell();
private:
    std::array<std::array<Tile, g_cell_size>, g_cell_size> m_tiles;
};

#endif // CELL_H
