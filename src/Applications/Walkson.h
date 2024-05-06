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
};