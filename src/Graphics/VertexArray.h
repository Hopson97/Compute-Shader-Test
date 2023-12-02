#pragma once

#include <array>
#include <vector>

// #include "../Mesh.h"
#include "GLResource.h"

enum class BindBufferTarget
{
    AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
    TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
    UniformBuffer = GL_UNIFORM_BUFFER,
    ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
};

struct BufferObject : public GLResource<glCreateBuffers, glDeleteBuffers>
{
    template <typename T>
    void buffer_data(const std::vector<T>& data)
    {
        glNamedBufferStorage(id, sizeof(data[0]) * data.size(), data.data(),
                             GL_DYNAMIC_STORAGE_BIT);
    }

    template <typename T>
    void buffer_data(const T& data)
    {
        glNamedBufferStorage(id, sizeof(data), data, GL_DYNAMIC_STORAGE_BIT);
    }

    template <typename T>
    void buffer_sub_data(GLintptr offset, const T& data)
    {
        glNamedBufferSubData(id, offset, sizeof(data), &data);
    }

    template <typename T, int N>
    void buffer_sub_data(GLintptr offset, const std::array<T, N>& data)
    {
        glNamedBufferSubData(id, offset, sizeof(data[0]) * data.size(), data.data());
    }

    template <typename T>
    void buffer_sub_data(GLintptr offset, const std::vector<T> data)
    {
        glNamedBufferSubData(id, offset, sizeof(data[0]) * data.size(), data.data());
    }

    void create_store(GLsizeiptr size);
    void bind_buffer_base(BindBufferTarget target, GLuint index);
    void bind_buffer_range(BindBufferTarget target, GLuint index, GLsizeiptr bytes);
};

struct VertexArray : public GLResource<glCreateVertexArrays, glDeleteVertexArrays>
{
    VertexArray() = default;
    void bind() const;
    void add_attribute(const BufferObject& vbo, GLsizei stride, GLint size, GLenum type,
                       GLuint offset);
    void reset() override;

  private:
    GLuint attribs_ = 0;
};