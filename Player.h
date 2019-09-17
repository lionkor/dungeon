#ifndef PLAYER_H
#define PLAYER_H

#include "Vector2/Vector2.hpp"

class Player
{
public:
    Player(Vector2f position);
private:
    Vector2f m_position;
};

#endif // PLAYER_H
