#include "Tile.h"
#include "Renderer.h"
#include "ResourceManager.h"

Tile::Tile()
    : m_texture(g_invalid_texture)
{
}

Tile::Tile(const Vector2f& pos, const std::string& texture)
    : m_position(pos), m_texture(g_resource_manager->get_texture(texture))
{
}

