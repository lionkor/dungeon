#ifndef WORLD_H
#define WORLD_H

#include "Global.h"
#include "Cell.h"
#include "Player.h"

class World
{
public:
    World();
private:
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Cell> m_cell;
};

inline std::unique_ptr<World> g_world;

#endif // WORLD_H
