#include "CollisionEngine.h"
#include "Cell.h"
#include "Player.h"
#include "Renderer.h"
#include "Tile.h"
#include "World.h"
#include "abstract/IRigidbody.h"
#include <cmath>

CollisionEngine::CollisionEngine() {}

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
    return r.x < s.x + s_size.x && r.x + r_size.x > s.x && r.y < s.y + s_size.y &&
           r.y + r_size.y > s.y; // FIXME: Non-square collision is untested.
}


void handle_collision(interface::IRigidbody& rbody, const Tile& tile)
{
    if (collides(rbody.position(), rbody.size(), tile.real_position(),
                 vec2(float(g_tile_size))))
    {
        vec2 diff = (rbody.position() + rbody.size() / 2.f) -
                    (tile.real_position() + float(g_tile_size / 2.f));
        vec2 delta = (rbody.size() / 2.f + float(g_tile_size / 2.f)) - glm::abs(diff);
        // log(delta.x << " " << delta.y);

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

void CollisionEngine::update(sf::Time)
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
}
