#pragma once

#include "Application.h"

class Walkson : public Application
{
  public:
    bool on_init(sf::Window& window) override;

    void frame(sf::Window& window) override;

  private:
    Shader walkson_compute_;
    Texture2D screen_texture_;

    glm::vec2 direction_{0.0};
    glm::vec3 position_{0.0};
};