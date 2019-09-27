#ifndef IMOVINGTHING_H
#define IMOVINGTHING_H

#include <SFML/Graphics.hpp>
#include "../ResourceManager.h"
#include "../Global.h"
#include "../CollisionEngine.h"

namespace interface
{

/*
 * Anything that moves and obeys to collision MUST inherit from this interface.
 * Anything inheriting from this interface will be rendered after 
 * CollisionEngine::update has occured.
 */
class IRigidbody
{
public:
    IRigidbody();
    IRigidbody(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& vel, const glm::vec2& acc, float max_vel);
    virtual ~IRigidbody();
    
    virtual void update(const sf::Time& dt) = 0;
    virtual void physics_update(const sf::Time& dt) final;
    
    virtual inline vec2& position() { return m_position; }
    virtual inline vec2& acceleration() { return m_acceleration; }
    virtual inline vec2& size() { return m_size; }
    virtual inline vec2& velocity() { return m_velocity; }
    virtual inline float& max_velocity() { return m_max_velocity; }
    
    virtual sf::Vector2f sf_position() { return { m_position.x, m_position.y }; }
    virtual sf::Vector2f sf_size() { return { m_size.x, m_size.y }; }
    
    virtual inline vec2& set_position(const glm::vec2& pos) { return m_position = pos; }
    virtual inline vec2& set_velocity(const glm::vec2& vel) { return m_velocity = vel; }
    virtual inline vec2& set_acceleration(const glm::vec2& acc) { return m_acceleration = acc; }
    virtual inline vec2& set_size(const glm::vec2& siz) { return m_size = siz; }
    virtual inline float& set_max_speed(float f) { return m_max_velocity = f; }
    
    virtual inline vec2& move_by(float dx, float dy) { return m_position += vec2(dx, dy); }
    
    virtual inline const vec2& position() const { return m_position; }
    virtual inline const vec2& acceleration() const { return m_acceleration; }
    virtual inline const vec2& size() const { return m_size; }
    virtual inline const vec2& velocity() const { return m_velocity; }
    virtual inline float max_velocity() const { return  m_max_velocity; }
protected:
    vec2 m_position;
    vec2 m_velocity;
    vec2 m_acceleration;
    vec2 m_size;
    float m_max_velocity;
    RigidbodyId m_rbody_id;
};

}

#endif // IMOVINGTHING_H
