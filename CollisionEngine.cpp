#include "CollisionEngine.h"
#include "World.h"
#include "Cell.h"
#include "Player.h"
#include "Tile.h"
#include "Renderer.h"
#include <cmath>

CollisionEngine::CollisionEngine()
{
}

void CollisionEngine::update(sf::Time dt)
{
    float dtime = dt.asSeconds();
    
    Player& player = g_world->player();
    Cell& cell = g_world->cell();
    Vector2f player_center = player.position() + player.size() / 2.0f;
    Vector2f player_top_left = player.position();
    Vector2f player_top_right = player.position() + Vector2f(player.size().x, 0);
    Vector2f player_bottom_right = player.position() + player.size();
    Vector2f player_bottom_left = player.position() + Vector2f(0, player.size().y);
    
    for (auto& tile_array : cell.tiles())
    for (Tile& tile : tile_array)
    {
        Vector2f tile_center = tile.position()*g_tile_size + Vector2i(g_tile_size / 2.0f);
        
        if (tile_center.distance(player_center) < g_tile_size + player.size().x /2.0f)
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
