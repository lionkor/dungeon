#ifndef COLLISIONENGINE_H
#define COLLISIONENGINE_H

#include "Global.h"
#include <SFML/Graphics.hpp>

class CollisionEngine
{
public:
    CollisionEngine();
    
    void update(sf::Time dt);
private:
};

inline std::unique_ptr<CollisionEngine> g_collision_engine;

#endif // COLLISIONENGINE_H
