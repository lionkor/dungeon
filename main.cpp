#include <iostream>
#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "PrimitiveRectangle.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Player.h"
#include "World.h"

int main () 
{
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Dungeon WIP");
    sf::Event event;

    Camera main_camera(window);
    main_camera.activate();
    
    g_resource_manager = std::make_unique<ResourceManager>("/home/lion/src/games/dungeon/res");
    g_renderer = std::make_unique<Renderer>(window);
    g_world = std::make_unique<World>();
    
    // main loop
    while (window->isOpen())
    {
        main_camera.move_center_to(g_world->player().sf_position());
        
        g_renderer->render();
        g_world->update();

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
            case sf::Event::KeyPressed:
            {
                // GUI handle keypress here
                bool handled = g_world->player().handle_key(event.key);
                if (!handled); // process further here
            }break;
            }
        }
    }

    delete window;
}
