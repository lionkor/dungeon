#include "Rectangle.h"

Rectangle::Rectangle() : m_vertices() {}

bool Rectangle::append_to_buffer(sf::VertexBuffer& buffer)
{
    return buffer.update(m_vertices.data());
}

bool Rectangle::update_in_buffer(sf::VertexBuffer& buffer, unsigned offset)
{
    return buffer.update(m_vertices.data(), 4, offset);
}
