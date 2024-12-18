#include "SignedDistanceFields.h"

#include "../Util/Maths.h"

#include <imgui.h>

bool SignedDistanceFields::on_init(sf::Window& window)
{

    window_ = &window;

    if (!cube_compute.load_stage("assets/shaders/SignedDistanceFields.glsl", ShaderType::Compute) ||
        !cube_compute.link_shaders())
    {
        return false;
    }

    // Set up the compute shader output texture
    screen_texture_.create(window.getSize().x, window.getSize().y, 1, TEXTURE_PARAMS_NEAREST,
                           TextureFormat::RGBA32F);

    return true;
}

void SignedDistanceFields::on_render(sf::Window& window)
{
    // Update the camera from keyboard/ mouse
    static sf::Clock clock;

    // Run the compute shader to create a texture
    glDisable(GL_DEPTH_TEST);
    cube_compute.bind();
    cube_compute.set_uniform("time", clock_.getElapsedTime().asSeconds());
    cube_compute.set_uniform("kind", sdf_kind_);
    cube_compute.set_uniform("distortion", sdf_distortion_);
    cube_compute.set_uniform("movement_speed", sdf_camera_speed_);
    glBindImageTexture(0, screen_texture_.id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(ceil(window.getSize().x / 8), ceil(window.getSize().y / 4), 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    // Render the computed output to a screen-wide quad
    screen_shader_.bind();
    screen_texture_.bind(0);
    screen_vao_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // IMGUI
    if (ImGui::Begin("SDF Config"))
    {
        ImGui::Text("Select Kind");
        ImGui::RadioButton("Torus", &sdf_kind_, 0);
        ImGui::RadioButton("Cube", &sdf_kind_, 1);
        ImGui::RadioButton("Fractal 1", &sdf_kind_, 2);
        ImGui::RadioButton("Fractal 2", &sdf_kind_, 3);
        ImGui::RadioButton("Fractal Mix 1 + 2", &sdf_kind_, 4);
        ImGui::RadioButton("Fractal 3", &sdf_kind_, 5);

        ImGui::Text("Parameters");
        ImGui::SliderFloat("Distortion", &sdf_distortion_, 0.1f, 1.0f);
        ImGui::SliderFloat("Speed", &sdf_camera_speed_, 0.1f, 1.0f);
    }
    ImGui::End();
}
