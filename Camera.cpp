#include "Camera.h"

Camera::Camera(sf::RenderWindow* window)
    : m_window(window), m_view(m_window->getView())
{
    log("Camera constructed.");
}

void Camera::activate()
{
    update_window();
}

void Camera::move_center_to(float x, float y)
{
    m_view.setCenter(x, y);
    update_window();
}

void Camera::move_center_to(sf::Vector2f xy)
{
    m_view.setCenter(xy);
    update_window();
}

void Camera::move(float dx, float dy)
{
    m_view.move(dx, dy);
    update_window();
}

void Camera::resize(float x, float y)
{
    m_view.setSize(x, y);
    update_window();
}

void Camera::zoom(float factor)
{
    m_view.zoom(factor);
    update_window();
}

sf::Vector2f Camera::center()
{
    return m_view.getCenter();
}
