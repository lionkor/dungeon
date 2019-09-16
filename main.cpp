#include <iostream>
#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "PrimitiveRectangle.h"

int main () 
{
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Dungeon");
    g_renderer = std::make_unique<Renderer>(window);
    sf::Event event;

    sf::Vector2f v[] = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};
    
    g_renderer->submit(window, PrimitiveRectangle(v));
    
    // main loop
    while (window->isOpen())
    {
        g_renderer->render();
        
        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            }
        }
    }

    delete window;
}
