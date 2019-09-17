#ifndef TILE_H
#define TILE_H

#include "Global.h"
#include "TileLayer.h"
#include <SFML/Graphics.hpp>

class Tile
{
public:
    Tile();
    Tile(const Vector2f& pos, const std::string& texture);
    
    inline const std::array<TileLayer, g_layer_count>& layers() const { return m_layers; }
    inline const Vector2f& position() const { return m_position; }
private:
    std::array<TileLayer, g_layer_count> m_layers;
    sf::Texture* m_texture;
    Vector2f m_position;
};

#endif // TILE_H
