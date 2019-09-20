#include "World.h"
#include "Player.h"
#include "Cell.h"
#include "Vector2/Vector2.hpp"

World::World()
{
}

void World::init()
{
    m_player = std::make_unique<Player>(Vector2f(5, 5));
    m_cell = std::make_unique<Cell>();
}

void World::update(const sf::Time& dt)
{
    m_cell->update(dt);
    m_player->update(dt);
}


