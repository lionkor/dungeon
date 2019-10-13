#include "TileRenderInfo.h"

TileRenderInfo::TileRenderInfo(const sf::Vector2f& pos, const sf::VertexArray& var,
                               ProceduralTextureId tex, FullShaderId shader_id)
    : position(pos), varray(var), texture_id(tex), shader(shader_id)
{
}
