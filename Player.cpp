#include "Player.h"
#include "Global.h"

Player::Player(Vector2f position)
    : m_position(position.x, position.y)
{
    log(position.x << " " << position.y);
}

void Player::move(Vector2f dir)
{
    // FIXME: Add mouse click auto move.
    // FIXME: Consider delta time.
    dir = dir.normalized();
    m_position += dir * m_movement_speed;
    log(m_position.x << " " << m_position.y);
}

void Player::update()
{
    Vector2f dir(0, 0);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        dir += { 0, -1 };
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        dir += { -1, 0 };
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        dir += { 0, 1 };
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        dir += { 1, 0 };
    }

    if (dir != Vector2f{0, 0}) move(dir);
}

bool Player::handle_key(sf::Event::KeyEvent key)
{
    bool rc { true };
    
    switch(key.code)
    {
    default:
        rc = false;
    }
    
    return rc;
}
