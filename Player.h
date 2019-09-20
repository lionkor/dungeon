#ifndef PLAYER_H
#define PLAYER_H

#include "Vector2/Vector2.hpp"
#include "abstract/IRigidbody.h"
#include <SFML/Graphics.hpp>

class Player : public abstract::IRigidbody
{
public:
    Player(Vector2f position);
    
    void move(Vector2f dir);
    
    bool handle_key_down(sf::Event::KeyEvent key);
    bool handle_key_up(sf::Event::KeyEvent key);
private:
    Vector2f m_position;
    Vector2f m_size;
    Id m_texture;
    float m_movement_speed { 50.0 };
    bool m_forward_pressed{false};
    bool m_back_pressed{false};
    bool m_left_pressed{false};
    bool m_right_pressed{false};
    
    // IRigidbody interface
public:
    void update(const sf::Time& dt);
    
    inline Vector2f& position() { return m_position; }
    inline Vector2f& size() { return m_size; }
    
    inline const Id& texture() const { return m_texture; }
    inline const Vector2f& position() const { return m_position; }
    inline const Vector2f& size() const { return m_size; }
    
    sf::Vector2f sf_position();
    sf::Vector2f sf_size();
    
    inline void set_position(const Vector2f& pos) { m_position = pos; }
    inline void set_position(const sf::Vector2f& pos) { m_position = Vector2f(pos.x, pos.y); }
};

#endif // PLAYER_H
