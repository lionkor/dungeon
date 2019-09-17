#include <iostream>
#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "World.h"
#include "PrimitiveRectangle.h"
#include "ResourceManager.h"

int main () 
{
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Dungeon");
    g_resource_manager = std::make_unique<ResourceManager>("/home/lion/src/games/dungeon/res");
    g_renderer = std::make_unique<Renderer>(window);
    g_world = std::make_unique<World>();
    sf::Event event;

    sf::Vector2f v[] = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};
    
    g_renderer->submit(window, PrimitiveRectangle(v));
    
    // main loop
    while (window->isOpen())
    {
        g_renderer->render();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window->close();
        }
        
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
