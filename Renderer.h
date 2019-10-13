#ifndef RENDERER_H
#define RENDERER_H

#include "Global.h"
#include "PrimitiveRectangle.h"
#include "Tile.h"
#include "TileRenderInfo.h"
#include "VoidPtrWrapper.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <mutex>

class Renderer
{
public:
    Renderer(sf::RenderWindow* window);

    std::mutex render_mx;

    // FIXME: 'submit' should take refcounted pointer, to make sure the resource cannot go
    // out of scope.
    RenderId submit(VoidPtrWrapper, const sf::VertexArray& arr);
    RenderId submit(VoidPtrWrapper, const Tile&, const std::uint32_t& walls);
    RenderId submit(class Player* player);

    void render();
    inline sf::RenderWindow* window() { return m_window; }

private:
    sf::RenderWindow* m_window;
    sf::Clock m_internal_clock;
    // Cell Tiles
    // std::map<RenderId, std::array<std::shared_ptr<TileLayerRenderInfo>, g_layer_count>>
    // m_tiles_old;
    std::map<RenderId, TileRenderInfo> m_tiles;
    Player* m_player;
    std::unordered_map<RenderId, sf::Text> m_texts;
    std::map<RenderId, sf::VertexArray> m_raw_varrays;


    sf::RenderTexture m_current_render_texture;
};

inline std::unique_ptr<Renderer> g_renderer;

#endif // RENDERER_H
