#include <SFML/Graphics.hpp>
#include <iostream>

#include "Camera.h"
#include "CollisionEngine.h"
#include "Player.h"
#include "PrimitiveRectangle.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "World.h"

#include <X11/Xlib.h>

int main()
{
    XInitThreads();

    sf::RenderWindow* window =
        new sf::RenderWindow(sf::VideoMode(1280, 720), "Dungeon WIP", sf::Style::Default);
    window->setVerticalSyncEnabled(false);
    // window->setFramerateLimit(30);
    sf::Clock delta_clock {};
    sf::Event event {};
    sf::Time dt {};

    Camera main_camera(window);
    main_camera.activate();

    g_collision_engine = std::make_unique<CollisionEngine>();
    g_resource_manager =
        std::make_unique<ResourceManager>("/home/lion/src/games/dungeon/res");
    g_renderer = std::make_unique<Renderer>(window);
    g_world = std::make_unique<World>();
    g_world->init();

    bool paused { false };

    // main loop
    while (window->isOpen())
    {
        // log("FPS: " << (1. / dt.asSeconds()));
        dt = delta_clock.restart();

        if (!paused)
        {
            g_world->update(dt);
            g_collision_engine->update(dt);
        }
        // Moved these around!
        sf::Vector2f vec = g_world->player().sf_position() +
                           sf::Vector2f(g_world->player().size().x / 2.0f,
                                        g_world->player().size().y / 2.0f);
        if (main_camera.center() != vec)
            main_camera.move_center_to(vec);
        g_renderer->render();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window->close();
        }

        while (window->pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::MouseButtonPressed:
                    break;
                case sf::Event::LostFocus:
                    paused = true;
                    break;
                case sf::Event::GainedFocus:
                    paused = false;
                    break;
                case sf::Event::Closed:
                    window->close();
                    break;
                case sf::Event::Resized:
                {
                    main_camera.resize(event.size.width, event.size.height);
                }
                break;
                case sf::Event::MouseWheelMoved:
                {
                    if (event.mouseWheel.delta > 0)
                        main_camera.zoom(0.9f);
                    else
                        main_camera.zoom(1.1f);
                }
                break;
                case sf::Event::KeyPressed:
                {
                    // GUI handle keypress here
                    bool handled = g_world->player().handle_key_down_event(event.key);
                    if (!handled)
                        ; // process further here
                }
                break;
                case sf::Event::KeyReleased:
                {
                    bool handled = g_world->player().handle_key_up_event(event.key);
                    if (!handled)
                        ; // process further
                }
                break;
            }
        }
    }

    delete window;
}
