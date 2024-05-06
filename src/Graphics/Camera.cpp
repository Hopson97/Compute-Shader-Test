#include "Camera.h"

#include <imgui.h>

void PerspectiveCamera::init(unsigned width, unsigned height, float fov)
{
    aspect_ = static_cast<float>(width) / static_cast<float>(height);
    fov_ = fov;
    projection_matrix_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
}

void PerspectiveCamera::update()
{
    forwards_ = forward_vector(transform.rotation);
    glm::vec3 centre = transform.position + glm::normalize(forwards_);
    view_matrix_ = glm::lookAt(transform.position, centre, {0, 1, 0});
}

void PerspectiveCamera::gui()
{
    if (ImGui::Begin("Camera")) {
        if (ImGui::SliderFloat("FOV", &fov_, 1.0f, 179.0f))
        {
            projection_matrix_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
        }
        if (ImGui::SliderFloat("Near", &near_, 1.0f, 10.0f))
        {
            projection_matrix_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
        }
        if (ImGui::SliderFloat("Far", &far_, 100.0f, 10000.0f))
        {
            projection_matrix_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
        }
    }
    ImGui::End();
}

const glm::mat4& PerspectiveCamera::get_view_matrix() const
{
    return view_matrix_;
}

const glm::mat4& PerspectiveCamera::get_projection() const
{
    return projection_matrix_;
}

const glm::vec3& PerspectiveCamera::get_forwards() const
{
    return forwards_;
}

