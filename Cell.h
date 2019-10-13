#ifndef CELL_H
#define CELL_H

#include "Global.h"
#include "Tile.h"

class Cell
{
public:
    Cell();
    Cell(const std::filesystem::path& path);

    void initialize();

    const std::array<std::array<Tile, g_cell_size>, g_cell_size>& tiles() const
    {
        return m_tiles;
    }
    std::array<std::array<Tile, g_cell_size>, g_cell_size>& tiles() { return m_tiles; }

    void update(const sf::Time& dt);

private:
    std::array<std::array<Tile, g_cell_size>, g_cell_size> m_tiles;
    bool m_initialized{ false };
};

#endif // CELL_H
