#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <mutex>
#include "Tile.h"
#include "PrimitiveRectangle.h"
#include "Global.h"

using RenderId=Id_t;

// "Safely" converts void* to RenderId.
class VoidPtrWrapper
{
private:
    void* m_ptr;
public:
    template<typename _T>
    VoidPtrWrapper(_T* ptr) : m_ptr(static_cast<void*>(ptr)) { }
    
    inline RenderId as_render_id() const { return RenderId(m_ptr); }
};

class TileLayerRenderInfo
{
public:
    TileLayerRenderInfo(Id texture_id, const sf::VertexArray& varray, FullShaderId shader_id, sf::Vector2f position = {0, 0});
    
    void set_uniforms() const;
    
    bool valid_id;
    sf::Vector2f position;
    sf::VertexArray varray;
    Id texture_id; 
    FullShaderId shader_id;
};


class Renderer
{
public:
    Renderer(sf::RenderWindow* window);
    
    std::mutex render_mx;
    
    // FIXME: 'submit' should take refcounted pointer, to make sure the resource cannot go out of scope.
    RenderId submit(VoidPtrWrapper, const sf::VertexArray& arr);
    RenderId submit(VoidPtrWrapper, const Tile&, const std::bitset<8>& walls);
    void render();
    inline sf::RenderWindow* window() { return m_window; }
private:
    sf::RenderWindow* m_window;
    sf::Clock m_internal_clock;
    // Cell Tiles
    std::map<RenderId, std::array<std::shared_ptr<TileLayerRenderInfo>, g_layer_count>> m_tiles;
    
    // Raw VertexArrays
    std::map<RenderId, sf::VertexArray> m_raw_varrays;
    
    sf::RenderTexture m_current_render_texture;
};

inline std::unique_ptr<Renderer> g_renderer;

#endif//RENDERER_H
