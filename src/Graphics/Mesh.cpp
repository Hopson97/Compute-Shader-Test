#include "Mesh.h"

#include <numeric>

#include <SFML/Graphics/Image.hpp>

Mesh generate_quad_mesh(float w, float h)
{
    Mesh mesh;
    mesh.vertices = {{{w, h, 0.0f}, {0.0f, 1.0f}, {0, 0, 1}},
                     {{0, h, 0.0f}, {1.0f, 1.0f}, {0, 0, 1}},
                     {{0, 0, 0.0f}, {1.0f, 0.0f}, {0, 0, 1}},
                     {{w, 0, 0.0f}, {0.0f, 0.0f}, {0, 0, 1}}

    };

    mesh.indices = {0, 1, 2, 2, 3, 0};

    mesh.buffer();
    return mesh;
}

Mesh generate_cube_mesh(const glm::vec3& dimensions)
{
    Mesh mesh;

    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;

    // clang-format off
    mesh.vertices = {
        {{w, h, d}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},  
        {{0, h, d}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0, 0, d}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},  
        {{w, 0, d}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

        {{0, h, d}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, 
        {{0, h, 0}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0, 0, 0}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}}, 
        {{0, 0, d}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

        {{0, h, 0}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, 
        {{w, h, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{w, 0, 0}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}}, 
        {{0, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

        {{w, h, 0}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  
        {{w, h, d}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{w, 0, d}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},  
        {{w, 0, 0}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},

        {{w, h, 0}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  
        {{0, h, 0}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0, h, d}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},  
        {{w, h, d}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

        {{0, 0, 0}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
        {{w, 0, 0}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{w, 0, d}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}, 
        {{0, 0, d}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
    };
    // clang-format on

    int index = 0;
    for (int i = 0; i < 6; i++)
    {
        mesh.indices.push_back(index);
        mesh.indices.push_back(index + 1);
        mesh.indices.push_back(index + 2);
        mesh.indices.push_back(index + 2);
        mesh.indices.push_back(index + 3);
        mesh.indices.push_back(index);
        index += 4;
    }
    mesh.buffer();

    return mesh;
}

Mesh generate_terrain_mesh(int size, int edgeVertices)
{
    float fEdgeVertexCount = static_cast<float>(edgeVertices);

    Mesh mesh;
    for (int z = 0; z < edgeVertices; z++)
    {
        for (int x = 0; x < edgeVertices; x++)
        {
            GLfloat fz = static_cast<GLfloat>(z);
            GLfloat fx = static_cast<GLfloat>(x);

            Vertex vertex;
            vertex.position.x = fx / fEdgeVertexCount * size;
            vertex.position.y = 0.0f;
            vertex.position.z = fz / fEdgeVertexCount * size;

            vertex.texture_coord.s = (fx / fEdgeVertexCount) * edgeVertices / 4.0f;
            vertex.texture_coord.t = (fz / fEdgeVertexCount) * edgeVertices / 4.0f;

            vertex.normal = {0, 1, 0};

            mesh.vertices.push_back(vertex);
        }
    }

    for (int z = 0; z < edgeVertices - 1; z++)
    {
        for (int x = 0; x < edgeVertices - 1; x++)
        {
            int topLeft = (z * edgeVertices) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((z + 1) * edgeVertices) + x;
            int bottomRight = bottomLeft + 1;

            mesh.indices.push_back(topLeft);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(topRight);
            mesh.indices.push_back(topRight);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(bottomRight);
        }
    }
    mesh.buffer();
    return mesh;
}

Mesh generate_grid_mesh(int size)
{
    Mesh mesh;
    float y = -0.02f;

    int index = 0;
    auto createLine = [&](const glm::vec3& begin, const glm::vec3& end)
    {
        auto bx = begin[0];
        auto bz = begin[2];
        auto ex = end[0];
        auto ez = end[2];

        Vertex vertex1 = {.position{bx, y, bz}};
        Vertex vertex2 = {.position{ex, y, ez}};
        mesh.vertices.push_back(vertex1);
        mesh.vertices.push_back(vertex2);

        mesh.indices.push_back(index++);
        mesh.indices.push_back(index++);
    };

    for (int x = 0; x <= size; x++)
    {
        createLine({x, y, 0}, {x, y, size});
    }

    for (int z = 0; z <= size; z++)
    {
        createLine({0, y, z}, {size, y, z});
    }
    mesh.buffer();
    return mesh;
}

void Mesh::buffer()
{
    vao_.reset();
    vbo_.reset();
    ebo_.reset();
    indices_ = static_cast<GLuint>(indices.size());

    // Buffer the EBO
    ebo_.buffer_data(indices);
    glVertexArrayElementBuffer(vao_.id, ebo_.id);

    // Buffer vertices + Add attribs
    vbo_.buffer_data(vertices);
    vao_.add_attribute(vbo_, sizeof(Vertex), 3, GL_FLOAT, offsetof(Vertex, position));
    vao_.add_attribute(vbo_, sizeof(Vertex), 2, GL_FLOAT, offsetof(Vertex, texture_coord));
    vao_.add_attribute(vbo_, sizeof(Vertex), 3, GL_FLOAT, offsetof(Vertex, normal));
}

void Mesh::bind() const
{
    vao_.bind();
}

void Mesh::draw(GLenum draw_mode) const
{
    assert(indices_ > 0);
    glDrawElements(draw_mode, indices_, GL_UNSIGNED_INT, nullptr);
}
