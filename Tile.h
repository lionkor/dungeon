#ifndef TILE_H
#define TILE_H

#include "Global.h"
#include "TileLayer.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

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
constexpr inline std::uint32_t Top         = 0b00000001;
constexpr inline std::uint32_t TopRight    = 0b00000010;
constexpr inline std::uint32_t Right       = 0b00000100;
constexpr inline std::uint32_t BottomRight = 0b00001000;
constexpr inline std::uint32_t Bottom      = 0b00010000;
constexpr inline std::uint32_t BottomLeft  = 0b00100000;
constexpr inline std::uint32_t Left        = 0b01000000;
constexpr inline std::uint32_t TopLeft     = 0b10000000;

constexpr inline std::uint32_t All         = 0b11111111;
constexpr inline std::uint32_t None        = 0b00000000;
}

class Tile
{
public:
    Tile();
    Tile(bool walkable, const glm::vec<2,int>& pos, const std::array<std::string, g_layer_count>& textures);
    
    inline const std::array<TileLayer, g_layer_count>& layers() const { return m_layers; }
    inline const glm::vec<2,int>& position() const { return m_position; }
    inline glm::vec2 real_position() const { return glm::vec2(m_position) * float(g_tile_size); }
    // DO NOT USE AS INDEX, ITS `float`
    inline sf::Vector2f sf_position() const { return { float(m_position.x), float(m_position.y) }; }
    inline std::uint32_t walls() const { return m_walls; }
    inline std::uint32_t& walls() { return m_walls; }
    
    inline bool& walkable() { return m_walkable; }
    inline bool walkable() const { return m_walkable; }
    
    void initialize(const class Cell* cell);
    void update(const sf::Time& dt);
private:
    std::array<TileLayer, g_layer_count> m_layers;
    glm::vec<2,int> m_position;
    std::uint32_t m_walls { TileSide::None };
    bool m_walkable;
};

#endif // TILE_H
