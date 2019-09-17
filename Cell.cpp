#include "Cell.h"
#include "Renderer.h"
//#include "CellFileParser.h"

Cell::Cell()
{
    for (int i = 0; i < g_cell_size; ++i)
    for (int k = 0; k < g_cell_size; ++k)
    {
        m_tiles[i][k] = {Vector2f(i * g_tile_size, k * g_tile_size), {"", "wall", "", ""}};
        g_renderer->submit(&m_tiles[i][k], m_tiles[i][k]);
    }
}

Cell::Cell(const std::filesystem::path& path)
{
    //m_tiles = std::move(CellFileParser::parse(path));
}

void Cell::update()
{
    for (auto& arr : m_tiles)
    for (auto& tile : arr)
    {
        tile.update();
    }
}

