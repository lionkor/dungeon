#include "Renderer.h"
#include <assert.h>
#include "PrimitiveRectangle.h"
#include "Tile.h"
#include "TileLayer.h"
#include "World.h"
#include "ResourceManager.h"

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
}

RenderId Renderer::submit(VoidPtrWrapper self, const sf::VertexArray& arr)
{
    m_raw_varrays[self.as_render_id()] = arr;
    return self.as_render_id();
}

RenderId Renderer::submit(VoidPtrWrapper self, const Tile& tile, const std::bitset<8>& walls)
{
    std::array<std::pair<sf::VertexArray, Id>, g_layer_count>& layers = m_tiles[self.as_render_id()];
    for (unsigned char i = 0; i < g_layer_count; ++i)
    {
        sf::VertexArray varray;
        varray.setPrimitiveType(sf::PrimitiveType::Quads);
        varray.append({ sf::Vector2f{ tile.sf_position().x * g_tile_size, tile.sf_position().y * g_tile_size }, { 0, 0 }});
        varray.append({ sf::Vector2f{ tile.sf_position().x * g_tile_size, tile.sf_position().y * g_tile_size } + sf::Vector2f { g_tile_size, 0 }, { g_texture_size, 0 }});
        varray.append({ sf::Vector2f{ tile.sf_position().x * g_tile_size, tile.sf_position().y * g_tile_size } + sf::Vector2f { g_tile_size, g_tile_size }, { g_texture_size, g_texture_size }});
        varray.append({ sf::Vector2f{ tile.sf_position().x * g_tile_size, tile.sf_position().y * g_tile_size } + sf::Vector2f { 0, g_tile_size }, { 0, g_texture_size }});
        
        if (i == Layer::Wall && tile.layers()[i].texture_id() != InvalidId) 
        {
            // FIXME: Use folders or prefixes to find all procedural texture parts.
            // FIXME: Optimize / Refactor this mess.
            std::vector<TextureId> ids;
            ids.reserve(6);
            ids.emplace_back(g_resource_manager->get_texture_id("black"));
            if (!(walls & TileSide::Top     ).any()) ids.push_back(g_resource_manager->get_texture_id("wall_top"));
            if (!(walls & TileSide::Right   ).any()) ids.push_back(g_resource_manager->get_texture_id("wall_right"));
            if (!(walls & TileSide::Bottom  ).any()) ids.push_back(g_resource_manager->get_texture_id("wall_bottom"));
            if (!(walls & TileSide::Left    ).any()) ids.push_back(g_resource_manager->get_texture_id("wall_left"));
            
            if (!(walls & TileSide::Top).any() && !(walls & TileSide::Right).any()) 
                ids.push_back(g_resource_manager->get_texture_id("wall_corner_top_right"));
            if (!(walls & TileSide::Bottom).any() && !(walls & TileSide::Right).any()) 
                ids.push_back(g_resource_manager->get_texture_id("wall_corner_bottom_right"));
            if (!(walls & TileSide::Bottom).any() && !(walls & TileSide::Left).any()) 
                ids.push_back(g_resource_manager->get_texture_id("wall_corner_bottom_left"));
            if (!(walls & TileSide::Top).any() && !(walls & TileSide::Left).any()) 
                ids.push_back(g_resource_manager->get_texture_id("wall_corner_top_left"));
            
            if ((walls & TileSide::Top).any() && 
                (walls & TileSide::Right).any() && 
                !(walls & TileSide::TopRight).any())
                ids.push_back(g_resource_manager->get_texture_id("wall_inner_corner_top_right"));
            if ((walls & TileSide::Bottom).any() && 
                (walls & TileSide::Right).any() && 
                !(walls & TileSide::BottomRight).any())
                ids.push_back(g_resource_manager->get_texture_id("wall_inner_corner_bottom_right"));
            if ((walls & TileSide::Bottom).any() && 
                (walls & TileSide::Left).any() && 
                !(walls & TileSide::BottomLeft).any())
                ids.push_back(g_resource_manager->get_texture_id("wall_inner_corner_bottom_left"));
            if ((walls & TileSide::Top).any() && 
                (walls & TileSide::Left).any() && 
                !(walls & TileSide::TopLeft).any())
                ids.push_back(g_resource_manager->get_texture_id("wall_inner_corner_top_left"));
            layers[i] = std::pair(varray, Id(IdType::ProceduralTextureId, g_resource_manager->make_procedural_texture(ids)));
        }
        else
        {
            layers[i] = std::pair(varray, Id(IdType::TextureId, tile.layers()[i].texture_id()));
        }
    }
    
    
    return self.as_render_id(); // FIXME: RenderId doesn't work with this way of batching.
}

void Renderer::render()
{
    m_window->clear();
    
    for (const std::pair<RenderId, std::array<std::pair<sf::VertexArray, Id>, g_layer_count>>& id_arr_pair : m_tiles)
    for (const std::pair<sf::VertexArray, Id>& varray_id_pair : id_arr_pair.second)
    {
        if (!varray_id_pair.second.invalid_id())
            m_window->draw(varray_id_pair.first, varray_id_pair.second.texture());
    }
    
    for (const auto& pair : m_raw_varrays)
    {
        m_window->draw(pair.second);
    }
    m_raw_varrays.clear();
    
    /*
    for (int i = 0; i < g_layer_count; ++i)
    for (auto& arr_pair : m_tile_texture_batches[i])
    {
        if (arr_pair.first != InvalidId)
        {
            m_window->draw(arr_pair.second, sf::RenderStates{g_resource_manager->get_texture(arr_pair.first)});
        }
    }
    */
    
    m_window->display();
}
