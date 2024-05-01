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

int main()
{
    sf::ContextSettings context_settings;
    context_settings.depthBits = 24;
    context_settings.stencilBits = 8;
    context_settings.antialiasingLevel = 4;
    context_settings.majorVersion = 4;
    context_settings.minorVersion = 5;
    context_settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window({1024, 1024}, "Compute Conway Game of Life", sf::Style::Default, context_settings);
    window.setVerticalSyncEnabled(true);
    bool mouse_locked = false;

    window.setActive(true);

    if (!gladLoadGL())
    {
        std::cerr << "Failed to init OpenGL - Is OpenGL linked correctly?\n";
        return -1;
    }
    glViewport(0, 0, 900, 900);
    init_opengl_debugging();
    // GUI::init(&window);

    mus::VertexArray screen_vao;

    mus::Shader screen_shader;
    if (!screen_shader.load_stage("assets/shaders/ScreenVertex.glsl", mus::ShaderType::Vertex) ||
        !screen_shader.load_stage("assets/shaders/ScreenFragment.glsl", mus::ShaderType::Fragment) ||
        !screen_shader.link_shaders())
    {
        return -1;
    }

    mus::Shader compute_shader;
    if (!compute_shader.load_stage("assets/shaders/ConwayCompute.glsl", mus::ShaderType::Compute) ||
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
    mus::Texture2D screen_texture;
    mus::Texture2D screen_texture2;
    screen_texture.load_from_image(image, 1, mus::TextureInternalFormat::RGBA, mus::TextureFormat::RGBA32F);
    // screen_texture.create(window.getSize().x, window.getSize().y, 1,
    //                       mus::TextureFormat::RGBA32F);
    screen_texture.set_wrap_s(mus::TextureWrap::ClampToEdge);
    screen_texture.set_wrap_t(mus::TextureWrap::ClampToEdge);
    screen_texture.set_min_filter(mus::TextureMinFilter::Nearest);
    screen_texture.set_mag_filter(mus::TextureMagFilter::Nearest);

    screen_texture2.create(window.getSize().x, window.getSize().y, 1, mus::TextureFormat::RGBA32F);
    screen_texture2.set_wrap_s(mus::TextureWrap::ClampToEdge);
    screen_texture2.set_wrap_t(mus::TextureWrap::ClampToEdge);
    screen_texture2.set_min_filter(mus::TextureMinFilter::Nearest);
    screen_texture2.set_mag_filter(mus::TextureMagFilter::Nearest);

    int gen = 0;

    // This is needed for the compute shader
    // glBindImageTexture(0, screen_texture.id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

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

        compute_shader.bind();

        if (gen % 2 == 0)
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
        gen++;

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