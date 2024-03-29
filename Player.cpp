#include "Player.h"
#include "Global.h"
#include "Renderer.h"
#include <assert.h>

Player::Player(const glm::vec2& pos)
    : interface::IRigidbody(pos, vec2(g_tile_size * 0.3f), vec2(0), vec2(0), 0.5f)
{
    g_renderer->submit(this);
}

void Player::update(const sf::Time& dt)
{
    glm::vec<2, int> dir(0);

    if (m_forward_pressed)
    {
        dir += vec2(0, -1);
    }
    if (m_left_pressed)
    {
        dir += vec2(-1, 0);
    }
    if (m_back_pressed)
    {
        dir += vec2(0, 1);
    }
    if (m_right_pressed)
    {
        dir += vec2(1, 0);
    }

    if (dir != glm::vec<2, int>(0))
    {
        if (dir.x == 0)
            m_velocity.x = 0;
        if (dir.y == 0)
            m_velocity.y = 0;
        m_acceleration = glm::normalize(vec2(dir)) * (float(dt.asMilliseconds() / 100.f));
    }
    else
    {
        m_acceleration = vec2(0);
        m_velocity *= 0.1f;
    }
}

bool Player::handle_key_down_event(sf::Event::KeyEvent key)
{
    bool return_code{ true };

    switch (key.code)
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
            return_code = false;
    }

    return return_code;
}

bool Player::handle_key_up_event(sf::Event::KeyEvent key)
{
    bool return_code{ true };

    switch (key.code)
    {
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
        default:
            return_code = false;
    }

    return return_code;
}
