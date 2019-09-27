#ifndef COLLISIONENGINE_H
#define COLLISIONENGINE_H

#include "Global.h"
#include <SFML/Graphics.hpp>

typedef unsigned long long RigidbodyId;

namespace interface
{
class IRigidbody;
}

class CollisionEngine
{
public:
    CollisionEngine();
    
    RigidbodyId register_rigidbody(interface::IRigidbody* rigidbody);
    bool unregister_rigidbody(const RigidbodyId&);
    //void move_request(interface::IRigidbody* rb, const vec2& delta);
    void update(sf::Time dt);
private:
    RigidbodyId m_last_rigidbody_id{ 1 };
    std::unordered_map<RigidbodyId, interface::IRigidbody*> m_rigidbodies;
};

inline std::unique_ptr<CollisionEngine> g_collision_engine;

#endif // COLLISIONENGINE_H
