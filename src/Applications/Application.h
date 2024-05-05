#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Graphics/VertexArray.h"

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;

class Application
{
  public:
    bool init(sf::Window& window);

    virtual void frame(sf::Window& window) = 0;

  protected:
    virtual bool on_init(sf::Window& window) = 0;

    Shader screen_shader_;
    VertexArray screen_vao_;
};
