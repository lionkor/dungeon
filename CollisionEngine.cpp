#include "CollisionEngine.h"
#include "World.h"
#include "Cell.h"
#include "Player.h"
#include "Tile.h"
#include "Renderer.h"
#include "abstract/IRigidbody.h"
#include <cmath>

CollisionEngine::CollisionEngine()
{
}

RigidbodyId CollisionEngine::register_rigidbody(abstract::IRigidbody* rigidbody)
{
    RigidbodyId id = ++m_last_rigidbody_id;
    m_rigidbodies[id] = rigidbody;
    return id;
}

bool CollisionEngine::unregister_rigidbody(const RigidbodyId& id)
{
    if (m_rigidbodies.find(id) == m_rigidbodies.end())
        return false;
    m_rigidbodies.erase(id);
    return true;
}

void CollisionEngine::update(sf::Time dt)
{
    float dtime = dt.asSeconds();
    
    for (auto& pair : m_rigidbodies)
    {
        abstract::IRigidbody& rbody = *pair.second;
        Cell& cell = g_world->cell();
        Vector2f rbody_center = rbody.position() + rbody.size() / 2.0f;
        Vector2f rbody_top_left = rbody.position();
        Vector2f rbody_top_right = rbody.position() + Vector2f(rbody.size().x, 0);
        Vector2f rbody_bottom_right = rbody.position() + rbody.size();
        Vector2f rbody_bottom_left = rbody.position() + Vector2f(0, rbody.size().y);
        
        for (auto& tile_array : cell.tiles())
        for (Tile& tile : tile_array)
        {
            Vector2f tile_center = tile.position()*g_tile_size + Vector2i(g_tile_size / 2.0f);
            
            if (tile_center.distance(rbody_center) < g_tile_size + rbody.size().x)
            {
                sf::VertexArray tile_arr(sf::PrimitiveType::Quads, 4);
                tile_arr[0] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size, tile.position().y*g_tile_size), sf::Color(255, 0, 0, 64));
                tile_arr[1] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size + g_tile_size, tile.position().y*g_tile_size), sf::Color(255, 0, 0, 64));
                tile_arr[2] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size + g_tile_size, tile.position().y*g_tile_size + g_tile_size), sf::Color(255, 0, 0, 64));
                tile_arr[3] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size, tile.position().y*g_tile_size + g_tile_size), sf::Color(255, 0, 0, 64));
                g_renderer->submit(&tile, tile_arr);
            }
        }
    }
}
