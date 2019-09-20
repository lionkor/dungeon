#ifndef IMOVINGTHING_H
#define IMOVINGTHING_H

#include <SFML/Graphics.hpp>
#include "../ResourceManager.h"
#include "../Global.h"

namespace abstract
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
    virtual ~IRigidbody();
    
    virtual void update(sf::Time dt) = 0;
    virtual Vector2f& position() = 0;
    virtual Vector2f& size() = 0;
    virtual sf::Vector2f& sf_position() = 0;
    virtual sf::Vector2f& sf_size() = 0;
    
    // set_position is needed by CollisionEngine.
    virtual void set_position(Vector2f) = 0;
    virtual void set_position(sf::Vector2f) = 0;
    
    virtual const Id& texture() const = 0;
    virtual const Vector2f& position() const = 0;
    virtual const Vector2f& size() const = 0;
    virtual const sf::Vector2f& sf_position() const = 0;
    virtual const sf::Vector2f& sf_size() const = 0;
private:
};

}

#endif // IMOVINGTHING_H
