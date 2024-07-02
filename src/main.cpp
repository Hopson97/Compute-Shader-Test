#include <memory>

#include <SFML/Window/Event.hpp>

#include "Applications/Application.h"
#include "Applications/GameOfLife.h"
#include "Applications/Walkson.h"
#include "Graphics/OpenGL/GLDebugEnable.h"

int main()
{
    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    context_settings.stencilBits = 8;
    context_settings.antialiasingLevel = 4;
    context_settings.majorVersion = 4;
    context_settings.minorVersion = 5;
    context_settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window({WIDTH, HEIGHT}, "Compute Shader Tests", sf::Style::Close, context_settings);
    window.setVerticalSyncEnabled(true);
    bool mouse_locked = false;

    window.setActive(true);

    if (!gladLoadGL())
    {
        std::cerr << "Failed to init OpenGL - Is OpenGL linked correctly?\n";
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    init_opengl_debugging();
    // GUI::init(&window);

    // std::unique_ptr<Application> app = std::make_unique<GameOfLife>();
    std::unique_ptr<Application> app = std::make_unique<Walkson>();
    if (!app->init(window))
    {
        return -1;
    }

    sf::Clock timer;

    // -------------------
    // ==== Main Loop ====
    // -------------------
    while (window.isOpen())
    {

        // GUI::begin_frame();
        sf::Event e;
        while (window.pollEvent(e))
        {
            app->handle_event(e);
            // GUI::event(window, e);

            switch (e.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyReleased:
                    switch (e.key.code)
                    {
                        case sf::Keyboard::Escape:
                            window.close();
                            break;

                        case sf::Keyboard::L:
                            mouse_locked = !mouse_locked;
                            break;
                        default:
                            break;
                    }

                default:
                    break;
            }
        }
        if (!window.isOpen())
        {
            break;
        }

        // window.setMouseCursorVisible(mouse_locked);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window.getSize().x, window.getSize().y);

        app->frame(window);

        // GUI::render();
        window.display();
    }

    // --------------------------
    // ==== Graceful Cleanup ====
    // --------------------------
    // GUI::shutdown();
}