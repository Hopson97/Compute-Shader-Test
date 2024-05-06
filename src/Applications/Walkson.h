#pragma once

#include "../Graphics/Camera.h"
#include "../Graphics/Mesh.h"

#include "Application.h"

class Walkson : public Application
{
  public:
    bool on_init(sf::Window& window) override;

    void frame(sf::Window& window) override;

    void handle_event(sf::Event event) override;

  private:
    void update_camera(sf::Time dt);

    Shader walkson_compute_;
    Shader scene_shader_;
    Texture2D screen_texture_;

    PerspectiveCamera camera_;

    Mesh cube_mesh_ = generate_cube_mesh({1, 1, 1});
    Mesh grid_mesh_ = generate_grid_mesh(10);
    Texture2D cube_texture_;

    sf::Window* window_ = nullptr;

    bool mouse_locked_;
};