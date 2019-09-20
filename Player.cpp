#include "Player.h"
#include "Global.h"
#include "Renderer.h"
#include <assert.h>

Player::Player(Vector2f position)
    : m_position(position.x, position.y), m_size(g_tile_size * 0.8f)
{
}

void Player::move(Vector2f dir)
{
    assert(false);
    // FIXME: Add mouse click auto move.
    dir = dir.normalized();
    m_position += dir * m_movement_speed;
    //log("Position: " << m_position.x << " " << m_position.y);
}

void Player::update(const sf::Time& dt)
{
    Vector2f dir(0, 0);
    
    if (m_forward_pressed)
    {
        dir += { 0, -1 };
    }
    if (m_left_pressed)
    {
        dir += { -1, 0 };
    }
    if (m_back_pressed)
    {
        dir += { 0, 1 };
    }
    if (m_right_pressed)
    {
        dir += { 1, 0 };
    }

    if (dir != Vector2f{0, 0})
    {
        m_position += dir.normalized() * m_movement_speed * dt.asSeconds();
    }
}

sf::Vector2f Player::sf_position()
{
    return sf::Vector2f(m_position.x, m_position.y);
}

sf::Vector2f Player::sf_size()
{
    return sf::Vector2f(m_size.x, m_size.y);
}


bool Player::handle_key_down(sf::Event::KeyEvent key)
{
    bool rc { true };
    
    switch(key.code)
    {
    case sf::Keyboard::W:
        m_forward_pressed = true;
        break;
    case sf::Keyboard::A:
        m_left_pressed = true;
        break;
    case sf::Keyboard::S:
        m_back_pressed = true;
        break;
    case sf::Keyboard::D:
        m_right_pressed = true;
        break;
    default:
        rc = false;
    }
    
    return rc;
}

bool Player::handle_key_up(sf::Event::KeyEvent key)
{
    bool rc { true };
    
    switch(key.code)
    {
    default:
    case sf::Keyboard::W:
        m_forward_pressed = false;
        break;
    case sf::Keyboard::A:
        m_left_pressed = false;
        break;
    case sf::Keyboard::S:
        m_back_pressed = false;
        break;
    case sf::Keyboard::D:
        m_right_pressed = false;
        break;
        rc = false;
    }
    
    return rc;
}
