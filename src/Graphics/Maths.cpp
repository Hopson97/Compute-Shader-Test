#include "Maths.h"

glm::mat4 create_model_matrix(const Transform& transform)
{
    glm::mat4 matrix{1.0f};

    matrix = glm::translate(matrix, transform.position);
    matrix = glm::scale(matrix, transform.scale);

    matrix = glm::rotate(matrix, glm::radians(transform.rotation.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(transform.rotation.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(transform.rotation.z), {0, 0, 1});

    return matrix;
}

glm::vec3 forward_vector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y);
    float pitch = glm::radians(rotation.x);

    return {
        glm::cos(yaw) * glm::cos(pitch),
        glm::sin(pitch),
        glm::cos(pitch) * glm::sin(yaw),
    };
}

glm::vec3 backward_vector(const glm::vec3& rotation)
{
    return -forward_vector(rotation);
}

glm::vec3 left_vector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y + 90);
    return {
        -glm::cos(yaw),
        0,
        -glm::sin(yaw),
    };
}

glm::vec3 right_vector(const glm::vec3& rotation)
{
    return -left_vector(rotation);
}
