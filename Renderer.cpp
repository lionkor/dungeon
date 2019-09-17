#include "Renderer.h"
#include <assert.h>
#include "PrimitiveRectangle.h"
#include "Tile.h"
#include "TileLayer.h"

sf::VertexArray varray_from_tile(const Tile& tile)
{
    sf::VertexArray arr(sf::PrimitiveType::Quads, 4);
    // TODO: Care about the layers.
    arr[0] = sf::Vertex({tile.position().x              , tile.position().y}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    arr[1] = sf::Vertex({tile.position().x + g_tile_size, tile.position().y}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    arr[2] = sf::Vertex({tile.position().x + g_tile_size, tile.position().y + g_tile_size}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    arr[3] = sf::Vertex({tile.position().x              , tile.position().y + g_tile_size}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    return arr;
}

Renderer::Renderer(sf::RenderWindow* window)
    : m_window(window)
{
    window->setFramerateLimit(60);
}

RenderId Renderer::submit(VoidPtrWrapper self, const PrimitiveRectangle& rect)
{
    m_rectangles.emplace(self.as_render_id(), rect);
    return self.as_render_id();
}

RenderId Renderer::submit(VoidPtrWrapper self, const Tile& tile)
{
    m_tiles.emplace(self.as_render_id(), varray_from_tile(tile));
    return self.as_render_id();
}

void Renderer::render()
{
    m_window->clear();
    
    for (auto& rect : m_rectangles)
    {
        sf::VertexArray arr(sf::PrimitiveType::Quads, 4);
        for (int i = 0; i < 4; ++i)
        {
            arr[i] = rect.second.m_vertices[i];
        }
        //m_window->draw(arr);
    }
    
    for (auto& tile : m_tiles)
    {
        m_window->draw(tile.second);
    }
    
    m_window->display();
}
