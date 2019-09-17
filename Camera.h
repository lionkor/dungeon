#ifndef CAMERA_H
#define CAMERA_H

#include "Global.h"
#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(sf::RenderWindow* m_window);
    
    void activate();
    
    void move_center_to(float x, float y);
    void move_center_to(sf::Vector2f xy);
    
    void move(float dx, float dy);
    
    void zoom(float factor);
private:
    void update_window()
    {
        m_window->setView(m_view);
    }
    
    sf::RenderWindow* m_window;
    sf::View m_view;
};

#endif // CAMERA_H
