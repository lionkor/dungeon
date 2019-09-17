#ifndef PLAYER_H
#define PLAYER_H

#include "Vector2/Vector2.hpp"
#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(Vector2f position);
    
    Vector2f position() { return m_position; }
    sf::Vector2f sf_position() { return sf::Vector2f(m_position.x, m_position.y); }
    
    void move(Vector2f dir);
    void update();
    
    bool handle_key(sf::Event::KeyEvent key);
private:
    Vector2f m_position;
    float m_movement_speed {1};
};

#endif // PLAYER_H
