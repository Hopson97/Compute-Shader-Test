#pragma once

#include "../Graphics/Camera.h"
#include "../Graphics/Mesh.h"

#include "Application.h"

class SignedDistanceFields : public Application
{
  public:
    bool on_init(sf::Window& window) override;
    void on_event(sf::Event event) override;
    void on_render(sf::Window& window) override;

  private:
    void update_camera(sf::Time dt);

    Shader cube_compute;
    Shader scene_shader_;
    Texture2D screen_texture_;

    PerspectiveCamera camera_;

    Mesh cube_mesh_ = generate_cube_mesh({2, 2, 2});
    Mesh grid_mesh_ = generate_grid_mesh(10);
    Texture2D cube_texture_;

    sf::Window* window_ = nullptr;

    bool mouse_locked_;

    sf::Clock clock_;

    // Config
    int sdf_kind_ = 0;
    float sdf_distortion_ = 0.2f;
    float sdf_camera_speed_ = 0.5f;
};