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

RigidbodyId CollisionEngine::register_rigidbody(interface::IRigidbody* rigidbody)
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


inline bool collides(const vec2& r, const vec2& r_size, const vec2& s, const vec2& s_size)
{
    return r.x < s.x + s_size.x &&
            r.x + r_size.x > s.x &&
            r.y < s.y + s_size.y &&
            r.y + r_size.y > s.y; // FIXME: Non-square collision is untested.
}
/*
void CollisionEngine::move_request(interface::IRigidbody* rb, const Vector2f& delta)
{
    Vector2f new_delta = delta;
    bool flag = false;
    for (auto& tile_array : g_world->cell().tiles())
    for (Tile& tile : tile_array)
    {
        // Assess whether the requested delta will cause an intersection. If not, ignore.
        if (tile.layers()[Layer::Wall].texture_id() != InvalidId && collides(rb->position() + new_delta, rb->size().x, tile.position() * g_tile_size, g_tile_size))
        {
            // At this point we know an intersection will happen if we use delta raw.
            // We need to move as close to pos+delta as possible now, without causing an intersection.
            Vector2f rbpos = (rb->position() + rb->size().x/2.) + new_delta;
            Vector2f tilepos = (tile.position() * double(g_tile_size)) + g_tile_size/2.;
            
            Vector2f diff = (tilepos - rbpos).absolute() - new_delta;
            
            // FIXME: Fix this dumb shit.)
            
            if (diff.x > diff.y)
            {
                if (rbpos.x > tilepos.x) 
                {
                    //new_delta.x = diff.x - (rb->size().x/2. + g_tile_size/2.);a
                    new_delta.x = diff.x - rb->size().x;
                    log(std::fixed << new_delta.x << "," << std::fixed << new_delta.y << " diff: " << std::fixed << diff.x << "," << std::fixed << diff.y);
                }
                if (rbpos.x < tilepos.x)
                {
                    //new_delta.x = -(diff.x - (rb->size().x/2. + g_tile_size/2.));
                    new_delta.x = -(diff.x - rb->size().x);
                    log(std::fixed << new_delta.x << "," << std::fixed << new_delta.y << " diff: " << std::fixed << diff.x << "," << std::fixed << diff.y);
                }
            }
            else
            {
                if (rbpos.y > tilepos.y) 
                {
                    //new_delta.y = diff.y - (rb->size().y/2. + g_tile_size/2.);
                }
                if (rbpos.y < tilepos.y) 
                {
                    //new_delta.y = -(diff.y - (rb->size().y/2. + g_tile_size/2.));
                }
            }
            rb->set_position(rb->position() + new_delta);
            flag = true;
        }
    }
    if (!flag)
    {
        rb->set_position(rb->position() + new_delta);
    }
}
*/

void handle_collision(interface::IRigidbody& rbody, const Tile& tile)
{
    //if (glm::distance(rbody.position() + rbody.size()/2.f, tile.real_position() + float(g_tile_size)) < rbody.size().x * rbody.size().y)
    if (collides(rbody.position(), rbody.size(), tile.real_position(), vec2(float(g_tile_size))))
    {
        vec2 diff = (rbody.position() + rbody.size()/2.f) - (tile.real_position() + float(g_tile_size/2.f));
        vec2 delta = (rbody.size()/2.f + float(g_tile_size/2.f)) - glm::abs(diff);
        log(delta.x << " " << delta.y);
        
        // do we need to correct x (true) or y (false)?
        if (delta.x < delta.y)
        {            
            // are we left (true) or right (false)?
            if (rbody.position().x < tile.real_position().x)
            {
                // right
                rbody.move_by(-delta.x, 0);
                // this prevents gaining velocity while running against a wall
                rbody.set_velocity(rbody.velocity() * vec2(0, 1));
            }
            else
            {
                // left
                rbody.move_by(delta.x, 0);
                rbody.set_velocity(rbody.velocity() * vec2(0, 1));
            }
        }
        else
        {
            // are we above (true) or below (false)?
            if (rbody.position().y < tile.real_position().y)
            {
                // down
                rbody.move_by(0, -delta.y);
                rbody.set_velocity(rbody.velocity() * vec2(1, 0));
            }
            else
            {
                // up
                rbody.move_by(0, delta.y);
                rbody.set_velocity(rbody.velocity() * vec2(1, 0));
            }
        }
    }
    else
    {
    }
}

void CollisionEngine::update(sf::Time dt)
{
    for (auto& rbody_pair : m_rigidbodies)
    {
        interface::IRigidbody& rbody = *rbody_pair.second;
        for (auto& tile_arrays : g_world->cell().tiles())
        for (auto& tile : tile_arrays)
        {
            if (!tile.walkable())
            {
                handle_collision(rbody, tile);
            }
        }
    }
    
    return;
    // FIXME: Dead code
    /*
    float dtime = dt.asSeconds();
    
    for (auto& pair : m_rigidbodies)
    {
        abstract::IRigidbody& rbody = *pair.second;
        Cell& cell = g_world->cell();
        
        for (auto& tile_array : cell.tiles())
        for (Tile& tile : tile_array)
        {
            Vector2f tile_center = tile.position()*g_tile_size + Vector2i(g_tile_size / 2.0f);
            
            if (tile.layers()[Layer::Wall].texture_id() != InvalidId && 
                tile_center.distance(rbody.position() + rbody.size()/2.) < g_tile_size + rbody.size().x)
            {
                sf::VertexArray tile_arr(sf::PrimitiveType::Quads, 4);
                tile_arr[0] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size, tile.position().y*g_tile_size), sf::Color(255, 0, 0, 64));
                tile_arr[1] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size + g_tile_size, tile.position().y*g_tile_size), sf::Color(255, 0, 0, 64));
                tile_arr[2] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size + g_tile_size, tile.position().y*g_tile_size + g_tile_size), sf::Color(255, 0, 0, 64));
                tile_arr[3] = sf::Vertex(sf::Vector2f(tile.position().x*g_tile_size, tile.position().y*g_tile_size + g_tile_size), sf::Color(255, 0, 0, 64));
                g_renderer->submit(&tile, tile_arr);
                
                // AABB collision detection.
                // Caution: Assumes square bodies.
                Vector2f delta = solve_collision(rbody.position(), rbody.size().x, tile.position()*g_tile_size, g_tile_size);
                rbody.set_position(rbody.position() + delta);
            }
        }
    }
    */
}
