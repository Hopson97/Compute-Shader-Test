#pragma once

#include <array>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
};

glm::mat4 create_model_matrix(const Transform& transform);
glm::vec3 forward_vector(const glm::vec3& rotation);
glm::vec3 backward_vector(const glm::vec3& rotation);
glm::vec3 left_vector(const glm::vec3& rotation);
glm::vec3 right_vector(const glm::vec3& rotation);
