#include "World.h"
#include "Player.h"
#include "Cell.h"
#include "Vector2/Vector2.hpp"

World::World()
    : m_player(std::make_unique<Player>(Vector2f(0, 0))),
      m_cell(std::make_unique<Cell>())
{
}
