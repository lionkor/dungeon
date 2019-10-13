#ifndef PRIMITIVERECTANGLE_H
#define PRIMITIVERECTANGLE_H

#include <SFML/Graphics.hpp>

class PrimitiveRectangle
{
public:
    sf::Vector2f m_vertices[4];

    PrimitiveRectangle(sf::Vector2f vertices[4])
    {
        for (int i = 0; i < 4; ++i)
        {
            m_vertices[i] = vertices[i];
        }
    }
};

#endif // PRIMITIVERECTANGLE_H
