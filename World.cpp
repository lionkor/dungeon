#include "World.h"
#include "Cell.h"
#include "Player.h"

World::World() {}

void World::init()
{
    m_player = std::make_unique<Player>(vec2(0, 0));
    m_cell = std::make_unique<Cell>();
}

void World::update(const sf::Time& dt)
{
    m_cell->update(dt);
    m_player->physics_update(dt);
    m_player->update(dt);
}
