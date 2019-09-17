#include "Tile.h"
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

void Tile::update()
{
    // FIXME: What does this update?
}

Tile::Tile(const Vector2f& pos, const std::array<std::string, 4>& textures)
    : m_position(pos), 
      m_layers({ 
                g_resource_manager->get_texture_id(textures[0]), // Ground
                g_resource_manager->get_texture_id(textures[1]), // Ground Decoration
                g_resource_manager->get_texture_id(textures[2]), // Walls / Collidables
                g_resource_manager->get_texture_id(textures[3])  // Overhead-Decoration
      })
{
}
