#include "IRigidbody.h"
#include "../CollisionEngine.h"

interface::IRigidbody::IRigidbody()
    : m_rbody_id(g_collision_engine->register_rigidbody(this))
{
}

interface::IRigidbody::IRigidbody(const glm::vec2& pos, const glm::vec2& size, 
                                  const glm::vec2& vel, const glm::vec2& acc, 
                                  float max_vel)
    : m_position(pos), m_size(size), 
      m_velocity(vel), m_acceleration(acc), 
      m_max_velocity(max_vel),
      m_rbody_id(g_collision_engine->register_rigidbody(this))
{
}

interface::IRigidbody::~IRigidbody()
{
    g_collision_engine->unregister_rigidbody(m_rbody_id);
}

void interface::IRigidbody::physics_update(const sf::Time& dt) // FINAL
{
    m_velocity += m_acceleration;
    m_velocity = glm::length(m_velocity) <= m_max_velocity ? m_velocity : glm::normalize(m_velocity) * m_max_velocity;
    m_position += m_velocity;
}
