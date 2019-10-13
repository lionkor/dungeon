#ifndef TILERENDERINFO_H
#define TILERENDERINFO_H

#include "Global.h"
#include <SFML/Graphics.hpp>

class TileRenderInfo
{
public:
    TileRenderInfo(const sf::Vector2f& pos, const sf::VertexArray& var,
                   ProceduralTextureId tex, FullShaderId shader_id);

    sf::Vector2f position;
    sf::VertexArray varray;
    ProceduralTextureId texture_id;
    FullShaderId shader;
};

#endif // TILERENDERINFO_H
