#ifndef WORLD_H
#define WORLD_H

#include "Cell.h"
#include "Global.h"
#include "Player.h"

class World
{
public:
    World();

    void init();

    Player& player() { return *m_player; }
    Cell& cell() { return *m_cell; }

    void update(const sf::Time& dt);

private:
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Cell> m_cell;
};

inline std::unique_ptr<World> g_world;

#endif // WORLD_H
