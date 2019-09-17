#ifndef TILELAYER_H
#define TILELAYER_H

#include "ResourceManager.h"

class TileLayer
{
public:
    TileLayer(TextureId texture_id);
    
    inline const TextureId& texture_id() const { return m_texture_id; }
private:
    TextureId m_texture_id;
};

#endif // TILELAYER_H
