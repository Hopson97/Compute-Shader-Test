#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "OpenGL/VertexArray.h"

struct Vertex
{
    glm::vec3 position{0.0f};
    glm::vec2 texture_coord{0.0f};
    glm::vec3 normal{0.0f};
};

class Mesh
{
  public:
    void buffer();
    void bind() const;
    void draw(GLenum draw_mode = GL_TRIANGLES) const;

  public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

  private:
    VertexArray vao_;
    BufferObject vbo_;
    BufferObject ebo_;
    GLuint indices_ = 0;
};

[[nodiscard]] Mesh generate_quad_mesh(float w, float h);
[[nodiscard]] Mesh generate_cube_mesh(const glm::vec3& size);
[[nodiscard]] Mesh generate_terrain_mesh(int size, int edgeVertices);
[[nodiscard]] Mesh generate_grid_mesh(int size);