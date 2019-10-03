#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Global.h"
#include <SFML/Graphics.hpp>

/// A wrapper around 4 vertices.
class Rectangle
{
public:
    Rectangle();
    
    /// Ctor directly constructing internal array.
    template<typename ..._Args>
    Rectangle(_Args ... args)
        : m_vertices(args...)
    {}
    
    inline       std::array<sf::Vertex, 4>& vertices()       { return m_vertices; }
    inline const std::array<sf::Vertex, 4>& vertices() const { return m_vertices; }
    
    inline       sf::Vertex& operator[](std::size_t index)       { return m_vertices[index]; }
    inline const sf::Vertex& operator[](std::size_t index) const { return m_vertices[index]; }
    
    inline bool empty() { return m_vertices.empty(); }
    
    /// Adds the vertices of this rectangle to a vertex buffer. Expensive operation.
    bool append_to_buffer(sf::VertexBuffer&);
    /// Updates the vertices at the offset of the buffer. Expensive operation.
    bool update_in_buffer(sf::VertexBuffer&, std::size_t offset);
private:
    std::array<sf::Vertex, 4> m_vertices;
};

#endif // RECTANGLE_H
