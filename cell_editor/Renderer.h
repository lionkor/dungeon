#ifndef RENDERER_H
#define RENDERER_H

#include "Global.h"

typedef unsigned long TextureId;

static const inline constexpr TextureId InvalidId = 0;

class Renderer
{
public:
    Renderer();
    
private:
    TextureId m_last_id { 1 };
};

inline std::unique_ptr<Renderer> g_renderer;

#endif // RENDERER_H
