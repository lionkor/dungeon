#include "Cell.h"
#include "Renderer.h"
//#include "CellFileParser.h"

Cell::Cell()
{
    for (int i = 0; i < g_cell_size; ++i)
    for (int k = 0; k < g_cell_size; ++k)
    {
        if (i % 3 == 0 && k % 4 == 0)
        {
            m_tiles[i][k] = {true, glm::vec<2, int>(i, k), {"ground", "", "", ""}};
        }
        else if (i % 4 == 0 || k % 3 == 0)
        {
            m_tiles[i][k] = {true, glm::vec<2, int>(i, k), {"ground", "", "", ""}};
        }
        else 
        {
            m_tiles[i][k] = {false, glm::vec<2, int>(i, k), {"ground", "", "wall", ""}};
        }
    }
}

Cell::Cell(const std::filesystem::path& path)
{
    //m_tiles = std::move(CellFileParser::parse(path));
}

void Cell::initialize()
{
    for (auto& arr : m_tiles)
    for (auto& tile : arr)
    {
        tile.initialize(this);
    }
}

void Cell::update(const sf::Time& dt)
{
    if (!m_initialized)
    {
        initialize();
        m_initialized = true;
    }
    
    for (auto& arr : m_tiles)
    for (auto& tile : arr)
    {
        tile.update(dt);
    }
}


