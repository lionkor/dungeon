#include "Renderer.h"
#include "Player.h"
#include "PrimitiveRectangle.h"
#include "ResourceManager.h"
#include "Tile.h"
#include "TileLayer.h"
#include "World.h"

#include <SFML/OpenGL.hpp>
#include <assert.h>

Renderer::Renderer(sf::RenderWindow* window) : m_window(window)
{
    if (!sf::Shader::isAvailable())
    {
        throw std::runtime_error(
            "Shaders are not supported on this platform. Contact the developers.");
    }
    // FIXME: Resize m_current_render_texture on resize events.
    m_current_render_texture.create(m_window->getSize().x, m_window->getSize().y);
    m_internal_clock.restart();
}

RenderId Renderer::submit(VoidPtrWrapper self, const sf::VertexArray& arr)
{
    m_raw_varrays[self.reinterpret_as<RenderId>()] = arr;
    return self.reinterpret_as<RenderId>();
}

RenderId Renderer::submit(VoidPtrWrapper self, const Tile& tile,
                          const std::uint32_t& walls)
{
    std::vector<TextureId> ids;
    ids.reserve(3);
    for (unsigned char i = 0; i < g_layer_count; ++i)
    {

        if (i == Layer::Wall && tile.layers()[i].texture_id() != InvalidId)
        {
            // FIXME: Use folders or prefixes to find all procedural texture parts.
            ids.emplace_back(g_resource_manager->get_texture_id("black"));

            if (!(walls & TileSide::Top))
                ids.push_back(g_resource_manager->get_texture_id("wall_top"));
            if (!(walls & TileSide::Right))
                ids.push_back(g_resource_manager->get_texture_id("wall_right"));
            if (!(walls & TileSide::Left))
                ids.push_back(g_resource_manager->get_texture_id("wall_left"));
            if (!(walls & TileSide::Bottom))
                ids.push_back(g_resource_manager->get_texture_id("wall_bottom"));

            if (!(walls & (TileSide::Top | TileSide::Right)))
                ids.push_back(
                    g_resource_manager->get_texture_id("wall_corner_top_right"));
            if (!(walls & (TileSide::Top | TileSide::Left)))
                ids.push_back(g_resource_manager->get_texture_id("wall_corner_top_left"));
            if (!(walls & (TileSide::Bottom | TileSide::Left)))
                ids.push_back(
                    g_resource_manager->get_texture_id("wall_corner_bottom_left"));
            if (!(walls & (TileSide::Bottom | TileSide::Right)))
                ids.push_back(
                    g_resource_manager->get_texture_id("wall_corner_bottom_right"));

            if ((walls & TileSide::Top) && (walls & TileSide::Right) &&
                !(walls & TileSide::TopRight))
                ids.push_back(
                    g_resource_manager->get_texture_id("wall_inner_corner_top_right"));
            if ((walls & TileSide::Bottom) && (walls & TileSide::Right) &&
                !(walls & TileSide::BottomRight))
                ids.push_back(
                    g_resource_manager->get_texture_id("wall_inner_corner_bottom_right"));
            if ((walls & TileSide::Bottom) && (walls & TileSide::Left) &&
                !(walls & TileSide::BottomLeft))
                ids.push_back(
                    g_resource_manager->get_texture_id("wall_inner_corner_bottom_left"));
            if ((walls & TileSide::Top) && (walls & TileSide::Left) &&
                !(walls & TileSide::TopLeft))
                ids.push_back(
                    g_resource_manager->get_texture_id("wall_inner_corner_top_left"));
        }
        else
        {
            ids.push_back(tile.layers()[i].texture_id());
        }
    }
    sf::VertexArray varray;
    varray.setPrimitiveType(sf::PrimitiveType::Quads);
    varray.append({ sf::Vector2f { tile.sf_position().x * g_tile_size,
                                   tile.sf_position().y * g_tile_size },
                    { 0, 0 } });
    varray.append({ sf::Vector2f { tile.sf_position().x * g_tile_size,
                                   tile.sf_position().y * g_tile_size } +
                        sf::Vector2f { g_tile_size, 0 },
                    { g_texture_size, 0 } });
    varray.append({ sf::Vector2f { tile.sf_position().x * g_tile_size,
                                   tile.sf_position().y * g_tile_size } +
                        sf::Vector2f { g_tile_size, g_tile_size },
                    { g_texture_size, g_texture_size } });
    varray.append({ sf::Vector2f { tile.sf_position().x * g_tile_size,
                                   tile.sf_position().y * g_tile_size } +
                        sf::Vector2f { 0, g_tile_size },
                    { 0, g_texture_size } });

    m_tiles.emplace(self.reinterpret_as<RenderId>(),
                    TileRenderInfo(tile.sf_position(), varray,
                                   g_resource_manager->make_procedural_texture(ids),
                                   g_resource_manager->get_full_shader_id("simple")));

    return self.reinterpret_as<RenderId>(); // FIXME: RenderId doesn't work with this way
                                            // of batching.
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

    sf::Vector2f player_pos = g_world->player().sf_position();
    auto& shader = m_tiles.begin()->second.shader;
    g_resource_manager->get_full_shader(shader)->setUniform("player_size",
                                                            g_world->player().sf_size());
    g_resource_manager->get_full_shader(shader)->setUniform("player_position",
                                                            player_pos);
    g_resource_manager->get_full_shader(shader)->setUniform(
        "view_position", m_window->mapPixelToCoords(sf::Mouse::getPosition()));
    for (const auto& layers : m_tiles)
    {
        const TileRenderInfo& layer = layers.second;
        sf::Texture* texture =
            g_resource_manager->get_procedural_texture(layer.texture_id);
        m_window->draw(
            layer.varray,
            sf::RenderStates(sf::BlendMode(), sf::Transform(), texture,
                             g_resource_manager->get_full_shader(layer.shader)));
    }

    // FIXME: Player should be drawn on layer WALL.
    m_window->draw(sf::RectangleShape(m_player->sf_size()),
                   sf::Transform().translate(m_player->sf_position()));

    for (const auto& pair : m_texts)
    {
        m_window->draw(pair.second);
    }
    
    for (const auto& pair : m_raw_varrays)
    {
        m_window->draw(pair.second);
    }
    // WHY m_raw_varrays.clear();

    m_window->display();
    render_mx.unlock();
}
