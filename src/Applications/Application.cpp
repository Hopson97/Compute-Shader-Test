#include "Application.h"

bool Application::init(sf::Window& window)
{
    if (!screen_shader_.load_stage("assets/shaders/ScreenVertex.glsl", ShaderType::Vertex) ||
        !screen_shader_.load_stage("assets/shaders/ScreenFragment.glsl", ShaderType::Fragment) ||
        !screen_shader_.link_shaders())
    {
        return false;
    }

    return on_init(window);
}