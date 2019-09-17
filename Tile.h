#ifndef TILE_H
#define TILE_H

#include "Global.h"
#include "TileLayer.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

inline static const std::string no_texture { "" };

class Tile
{
public:
    Tile();
    Tile(const Vector2f& pos, const std::array<std::string, g_layer_count>& textures);
    
    inline const std::array<TileLayer, g_layer_count>& layers() const { return m_layers; }
    inline const Vector2f& position() const { return m_position; }
    inline sf::Vector2f sf_position() const { return { m_position.x, m_position.y}; }
    
    void update();
private:
    std::array<TileLayer, g_layer_count> m_layers;
    Vector2f m_position;
};

#endif // TILE_H
