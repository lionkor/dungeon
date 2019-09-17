#include "Cell.h"
#include "Renderer.h"

Cell::Cell()
{
    for (int i = 0; i < g_cell_size; ++i)
    for (int k = 0; k < g_cell_size; ++k)
    {
        m_tiles[i][k] = { Vector2f(i * g_tile_size + g_tile_size, k * g_tile_size + g_tile_size), "wall" };
        g_renderer->submit(&m_tiles[i][k], m_tiles[i][k]);
    }
}

Cell::Cell(const std::filesystem::path& cell_file)
{
    // FIXME: Check for errors.
    // TODO: This.
}

