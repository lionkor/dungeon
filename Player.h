#ifndef PLAYER_H
#define PLAYER_H

#include "abstract/IRigidbody.h"
#include <SFML/Graphics.hpp>

class Player : public interface::IRigidbody
{
public:
    Player();
    Player(const vec2& pos);
    
    void update(const sf::Time& dt) override;
    //void move(Vector2f dir);
    
    inline Id& texture() { return m_texture; }
    bool handle_key_down_event(sf::Event::KeyEvent key);
    bool handle_key_up_event(sf::Event::KeyEvent key);
private:
    Id m_texture;
    bool m_forward_pressed{false};
    bool m_back_pressed{false};
    bool m_left_pressed{false};
    bool m_right_pressed{false};
};

#endif // PLAYER_H
