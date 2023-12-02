#include <numbers>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLDebugEnable.h"
#include "GUI.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Util.h"

int main()
{
    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    context_settings.stencilBits = 8;
    context_settings.antialiasingLevel = 4;
    context_settings.majorVersion = 4;
    context_settings.minorVersion = 5;
    context_settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window({1600, 900}, "OpenGL", sf::Style::Default, context_settings);
    window.setVerticalSyncEnabled(true);
    bool mouse_locked = false;

    window.setActive(true);

    if (!gladLoadGL())
    {
        std::cerr << "Failed to init OpenGL - Is OpenGL linked correctly?\n";
        return -1;
    }
    glViewport(0, 0, 1600, 900);
    init_opengl_debugging();
    GUI::init(&window);

    VertexArray screen_vao;
    Texture2D screen_texture;
    screen_texture.create(window.getSize().x, window.getSize().y, 1, TextureFormat::RGBA32F);

    // ----------------------
    Shader screen_shader;
    if (!screen_shader.load_from_file("assets/shaders/ScreenVertex.glsl",
                                   "assets/shaders/ScreenFragment.glsl"))
    {
        return -1;
    }

    Shader compute_shader;
    compute_shader.load_compute("assets/shaders/Compute.glsl");



    // PerspectiveCamera camera(window.getSize().x, window.getSize().y, 90.0f);

    Settings settings;

    // -------------------
    // ==== Main Loop ====
    // -------------------
    while (window.isOpen())
    {
        GUI::begin_frame();
        sf::Event e;
        while (window.pollEvent(e))
        {
            GUI::event(window, e);
            if (e.type == sf::Event::Closed)
                window.close();
            else if (e.type == sf::Event::KeyReleased)
                if (e.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (e.key.code == sf::Keyboard::L)
                    mouse_locked = !mouse_locked;
        }
        if (!window.isOpen())
        {
            break;
        }

        if (!mouse_locked)
        {
            window.setMouseCursorVisible(false);
        }
        else
        {
            window.setMouseCursorVisible(true);
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window.getSize().x, window.getSize().y);

        screen_shader.bind();
        screen_texture.bind(0);
        screen_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);


        GUI::render();
        window.display();
    }

    // --------------------------
    // ==== Graceful Cleanup ====
    // --------------------------
    GUI::shutdown();
}