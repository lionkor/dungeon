#include "Renderer.h"
#include <assert.h>
#include "PrimitiveRectangle.h"
#include "Tile.h"
#include "TileLayer.h"
#include "World.h"
#include "Player.h"
#include "ResourceManager.h"
#include <SFML/OpenGL.hpp>

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
    if (!sf::Shader::isAvailable()) 
    {
        throw std::runtime_error("Shaders are not supported on this platform. Contact the developers.");
    }
    // FIXME: Resize m_current_render_texture on resize events.
    m_current_render_texture.create(m_window->getSize().x, m_window->getSize().y);
    m_internal_clock.restart();
}

RenderId Renderer::submit(VoidPtrWrapper self, const sf::VertexArray& arr)
{
    m_raw_varrays[self.as_render_id()] = arr;
    return self.as_render_id();
}

RenderId Renderer::submit(VoidPtrWrapper self, const Tile& tile, const std::bitset<8>& walls)
{
    std::array<std::shared_ptr<TileLayerRenderInfo>, g_layer_count>& layers = m_tiles[self.as_render_id()];
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
            layers[i] = std::make_shared<TileLayerRenderInfo>
                    (
                        Id(IdTextureType::ProceduralTextureId, g_resource_manager->make_procedural_texture(ids)), 
                        varray, 
                        g_resource_manager->get_full_shader_id("simple"), 
                        tile.sf_position()
                    );
        }
        else
        {
            layers[i] = std::make_shared<TileLayerRenderInfo>
                    (
                        Id(IdTextureType::TextureId, tile.layers()[i].texture_id()), 
                        varray, 
                        g_resource_manager->get_full_shader_id("simple"), 
                        tile.sf_position()
                    );
        }
    }
    
    
    return self.as_render_id(); // FIXME: RenderId doesn't work with this way of batching.
}

RenderId Renderer::submit(Player* player)
{
    m_player = player;
    return 1;
}

void Renderer::render()
{
    render_mx.lock();
    m_window->clear();
    
    auto time = m_internal_clock.getElapsedTime();
    
    //m_current_render_texture.clear();
    for (const std::pair<RenderId, std::array<std::shared_ptr<TileLayerRenderInfo>, g_layer_count>>& layers : m_tiles)
    for (std::shared_ptr<TileLayerRenderInfo> layer : layers.second)
    {
        if (layer && !layer->texture_id.invalid_id())
        {
            sf::Shader* shader = g_resource_manager->get_full_shader(layer->shader_id);
            shader->setUniform("tile_position", sf::Glsl::Vec2(layer->position.x * g_tile_size, layer->position.y * g_tile_size));
            shader->setUniform("player_position", g_world->player().sf_position());
            shader->setUniform("time", float(time.asMilliseconds()));
            shader->setUniform("texture_size", sf::Glsl::Vec2(g_texture_size, g_texture_size));
            sf::Sprite sp(*layer->texture_id.texture(), sf::IntRect(0, 0, g_tile_size, g_tile_size));
            sp.setPosition(layer->position.x * g_tile_size, layer->position.y * g_tile_size);
            //m_current_render_texture.draw(layer->varray, g_resource_manager->get_full_shader(layer->shader_id));
            m_window->draw(sp, g_resource_manager->get_full_shader(layer->shader_id));
        }
        
        if (layer == layers.second[Layer::Wall])
        {
            // FIXME: Draw player properly.
            m_window->draw(sf::RectangleShape(m_player->sf_size()), sf::Transform().translate(m_player->sf_position()));
        }
    }
    //m_current_render_texture.display();
    
    //m_window->clear();
    //sf::Shader* sh = g_resource_manager->get_fragment_shader(g_resource_manager->get_fragment_shader_id("simple"));
    //sh->setUniform("texture", m_current_render_texture.getTexture());
    //sf::Sprite sprite(m_current_render_texture.getTexture());
    //m_window->draw(sprite);
    
    for (const auto& pair : m_raw_varrays)
    {
        m_window->draw(pair.second);
    }
    // WHY m_raw_varrays.clear();
    
    m_window->display();
    render_mx.unlock();
}

TileLayerRenderInfo::TileLayerRenderInfo(Id texture_id, const sf::VertexArray& varray, FullShaderId shader_id, sf::Vector2f position)
    : varray(varray), position(position), shader_id(shader_id), texture_id(texture_id)
{
    g_resource_manager->get_full_shader(shader_id)->setUniform("texture", sf::Shader::CurrentTexture);
}

void TileLayerRenderInfo::set_uniforms() const
{
    
    //shader->setUniform("texture", *texture_id.texture());
}
