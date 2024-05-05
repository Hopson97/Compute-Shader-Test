#pragma once

#include <cstdlib>
#include <numbers>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/GLDebugEnable.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Util.h"

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;

int main()
{
    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    context_settings.stencilBits = 8;
    context_settings.antialiasingLevel = 4;
    context_settings.majorVersion = 4;
    context_settings.minorVersion = 5;
    context_settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window({WIDTH, HEIGHT}, "Compute Conway Game of Life", sf::Style::Default, context_settings);
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

    VertexArray screen_vao;

    Shader screen_shader;
    if (!screen_shader.load_stage("assets/shaders/ScreenVertex.glsl", ShaderType::Vertex) ||
        !screen_shader.load_stage("assets/shaders/ScreenFragment.glsl", ShaderType::Fragment) ||
        !screen_shader.link_shaders())
    {
        return -1;
    }

    Shader compute_shader;
    if (!compute_shader.load_stage("assets/shaders/ConwayCompute.glsl", ShaderType::Compute) ||
        !compute_shader.link_shaders())
    {
        return -1;
    }

    sf::Image image;
    image.create(window.getSize().x, window.getSize().y);
    for (int y = 0; y < image.getSize().y; y++)
    {
        for (int x = 0; x < image.getSize().x; x++)
        {
            image.setPixel(x, y, rand() % 50 > 20 ? sf::Color::Black : sf::Color::White);
        }
    }

    // Set up the double buffer
    Texture2D screen_texture;
    Texture2D screen_texture2;
    screen_texture.load_from_image(image, 1, TextureInternalFormat::RGBA, TextureFormat::RGBA32F);
    screen_texture.set_wrap_s(TextureWrap::Repeat);
    screen_texture.set_wrap_t(TextureWrap::Repeat);
    screen_texture.set_min_filter(TextureMinFilter::Nearest);
    screen_texture.set_mag_filter(TextureMagFilter::Nearest);

    screen_texture2.create(window.getSize().x, window.getSize().y, 1, TextureFormat::RGBA32F);
    screen_texture2.set_wrap_s(TextureWrap::Repeat);
    screen_texture2.set_wrap_t(TextureWrap::Repeat);
    screen_texture2.set_min_filter(TextureMinFilter::Nearest);
    screen_texture2.set_mag_filter(TextureMagFilter::Nearest);

    // The simulation's generation (or frame count)
    int generation = 0;

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
            // GUI::event(window, e);
            if (e.type == sf::Event::Closed)
                window.close();
            else if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (e.key.code == sf::Keyboard::L)
                {
                    mouse_locked = !mouse_locked;
                }
            }
        }
        if (!window.isOpen())
        {
            break;
        }

        // window.setMouseCursorVisible(mouse_locked);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window.getSize().x, window.getSize().y);

        // The compute shader uses a double buffer to prevent read/write to same pixel by different work groups
        compute_shader.bind();
        if (generation % 2 == 0)
        {
            glBindImageTexture(0, screen_texture.id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
            glBindImageTexture(1, screen_texture2.id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
            screen_texture2.bind(0);
        }
        else
        {
            glBindImageTexture(0, screen_texture2.id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
            glBindImageTexture(1, screen_texture.id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
            screen_texture.bind(0);
        }
        generation++;

        glDispatchCompute(ceil(window.getSize().x / 8), ceil(window.getSize().y / 4), 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        screen_shader.bind();
        screen_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // GUI::render();
        window.display();
    }

    // --------------------------
    // ==== Graceful Cleanup ====
    // --------------------------
    // GUI::shutdown();
}