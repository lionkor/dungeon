#include "Renderer.h"
#include <assert.h>
#include "PrimitiveRectangle.h"
#include "Tile.h"
#include "TileLayer.h"

sf::VertexArray varray_from_tile(const Tile& tile)
{
    
    /*
    sf::VertexArray arr(sf::PrimitiveType::Quads, 4);
    // TODO: Care about the layers.
    arr[0] = sf::Vertex({tile.position().x              , tile.position().y}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    arr[1] = sf::Vertex({tile.position().x + g_tile_size, tile.position().y}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    arr[2] = sf::Vertex({tile.position().x + g_tile_size, tile.position().y + g_tile_size}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    arr[3] = sf::Vertex({tile.position().x              , tile.position().y + g_tile_size}, sf::Color(int(tile.position().x) % 255, int(tile.position().y) % 255, 255));
    return arr;
    */
    assert(false);
    return {};
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
    for (unsigned char i = 0; i < 4; ++i)
    {
        sf::VertexArray& varray = m_tile_texture_batches[i][tile.layers()[i].texture_id()];
        varray.setPrimitiveType(sf::PrimitiveType::Quads); // FIXME: Set this somewhere else, once!
        varray.append({ tile.sf_position(), { 0, 0 }});
        varray.append({ tile.sf_position() + sf::Vector2f { g_tile_size, 0 }, { g_texture_size, 0 }});
        varray.append({ tile.sf_position() + sf::Vector2f { g_tile_size, g_tile_size }, { g_texture_size, g_texture_size }});
        varray.append({ tile.sf_position() + sf::Vector2f { 0, g_tile_size }, { 0, g_texture_size }});
    }
    return self.as_render_id(); // FIXME: RenderId doesn't work with this way of batching.
}

void Renderer::render()
{
    m_window->clear();
    
    /*
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
    */
    
    for (auto& layer : m_tile_texture_batches)
    for (auto& arr_pair : layer)
    {
        if (arr_pair.first != InvalidId)
            m_window->draw(arr_pair.second, sf::RenderStates{g_resource_manager->get_texture(arr_pair.first)});
    }
    
    m_window->display();
}
