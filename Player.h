#ifndef PLAYER_H
#define PLAYER_H

#include "Vector2/Vector2.hpp"
#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(Vector2f position);
    
    Vector2f& position() { return m_position; }
    const Vector2f& position() const { return m_position; }
    sf::Vector2f sf_position() { return sf::Vector2f(m_position.x, m_position.y); }
    
    Vector2f& size() { return m_size; }
    
    void move(Vector2f dir);
    void update(const sf::Time& dt);
    
    bool handle_key_down(sf::Event::KeyEvent key);
    bool handle_key_up(sf::Event::KeyEvent key);
private:
    Vector2f m_position;
    Vector2f m_size;
    float m_movement_speed { 50.0 };
    bool m_forward_pressed{false};
    bool m_back_pressed{false};
    bool m_left_pressed{false};
    bool m_right_pressed{false};
};

#endif // PLAYER_H
