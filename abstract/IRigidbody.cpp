#include "IRigidbody.h"
#include "../CollisionEngine.h"

abstract::IRigidbody::IRigidbody()
{
    m_rbody_id = g_collision_engine->register_rigidbody(this);
}

abstract::IRigidbody::~IRigidbody()
{
    g_collision_engine->unregister_rigidbody(m_rbody_id);
}
