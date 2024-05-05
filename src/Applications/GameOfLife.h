#pragma once

#include "Application.h"

class GameOfLife : public Application
{
  public:
    bool on_init(sf::Window& window) override;

    void frame(sf::Window& window) override;

  private:
    Shader game_of_life_compute_;
    Texture2D screen_texture_;
    Texture2D screen_texture2_;

    int generation_ = 0;
};