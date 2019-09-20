#include "Tile.h"
#include "Cell.h"
#include "Renderer.h"

Tile::Tile()
    : m_position(0), 
      m_layers({ 
                InvalidId, // Ground
                InvalidId, // Ground Decoration
                InvalidId, // Walls / Collidables
                InvalidId  // Overhead-Decoration
      })
{
}

// Safe
bool is_valid_texture_id(const Vector2i& pos, const Cell* cell)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= g_cell_size || pos.y >= g_cell_size)
        return false;
    return cell->tiles()[pos.x][pos.y].layers()[Layer::Wall].texture_id() != InvalidId;
}

bool is_same_texture_id(const Vector2i& pos, const Cell* cell, const Vector2i& my_pos)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= g_cell_size || pos.y >= g_cell_size)
        return false;
    return cell->tiles()[pos.x][pos.y].layers()[Layer::Wall].texture_id() == cell->tiles()[my_pos.x][my_pos.y].layers()[Layer::Wall].texture_id();
}

void Tile::initialize(const Cell* cell)
{
    // Generate bitset here
    if (is_same_texture_id(m_position + Vector2i( 0, -1), cell, m_position))
        m_walls |= TileSide::Top;
    if (is_same_texture_id(m_position + Vector2i( 1, -1), cell, m_position))
        m_walls |= TileSide::TopRight;
    if (is_same_texture_id(m_position + Vector2i( 1,  0), cell, m_position))
        m_walls |= TileSide::Right;
    if (is_same_texture_id(m_position + Vector2i( 1,  1), cell, m_position))
        m_walls |= TileSide::BottomRight;
    if (is_same_texture_id(m_position + Vector2i( 0,  1), cell, m_position))
        m_walls |= TileSide::Bottom;
    if (is_same_texture_id(m_position + Vector2i(-1,  1), cell, m_position))
        m_walls |= TileSide::BottomLeft;
    if (is_same_texture_id(m_position + Vector2i(-1,  0), cell, m_position))
        m_walls |= TileSide::Left;
    if (is_same_texture_id(m_position + Vector2i(-1, -1), cell, m_position))
        m_walls |= TileSide::TopLeft;
    g_renderer->submit(this, *this, m_walls);
}

void Tile::update(const sf::Time& dt)
{
}

Tile::Tile(const Vector2i& pos, const std::array<std::string, 4>& textures)
    : m_position(pos), 
      m_layers({ 
                g_resource_manager->get_texture_id(textures[0]), // Ground
                g_resource_manager->get_texture_id(textures[1]), // Ground Decoration
                g_resource_manager->get_texture_id(textures[2]), // Walls / Collidables
                g_resource_manager->get_texture_id(textures[3])  // Overhead-Decoration
      })
{
}
