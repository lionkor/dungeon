#ifndef WORLD_H
#define WORLD_H

#include "Global.h"

class World
{
public:
    World();
private:
    std::unique_ptr<class Player> m_player;
    std::unique_ptr<class Cell> m_cell;
};

#endif // WORLD_H
