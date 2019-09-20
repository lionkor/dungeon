#ifndef TILE_H
#define TILE_H

#include "Global.h"
#include "TileLayer.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

#include <bitset>

inline static const std::string no_texture { "" };

enum Layer : unsigned char
{
    Ground = 0,
    GroundDecoration,
    Wall,
    Ceiling
};

namespace TileSide
{
const inline std::bitset<8> Top         = 0b00000001;
const inline std::bitset<8> TopRight    = 0b00000010;
const inline std::bitset<8> Right       = 0b00000100;
const inline std::bitset<8> BottomRight = 0b00001000;
const inline std::bitset<8> Bottom      = 0b00010000;
const inline std::bitset<8> BottomLeft  = 0b00100000;
const inline std::bitset<8> Left        = 0b01000000;
const inline std::bitset<8> TopLeft     = 0b10000000;

const inline std::bitset<8> All         = 0b11111111;
const inline std::bitset<8> None        = 0b00000000;
}

class Tile
{
public:
    Tile();
    Tile(const Vector2i& pos, const std::array<std::string, g_layer_count>& textures);
    
    inline const std::array<TileLayer, g_layer_count>& layers() const { return m_layers; }
    inline const Vector2i& position() const { return m_position; }
    // DO NOT USE AS INDEX, ITS `float`
    inline sf::Vector2f sf_position() const { return { float(m_position.x), float(m_position.y) }; }
    inline const std::bitset<8>& walls() const { return m_walls; }
    
    void initialize(const class Cell* cell);
    void update(const sf::Time& dt);
private:
    std::array<TileLayer, g_layer_count> m_layers;
    Vector2i m_position;
    std::bitset<8> m_walls { TileSide::None };
};

#endif // TILE_H
