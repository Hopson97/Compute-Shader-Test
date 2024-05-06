#include "VertexArray.h"

// VertexArray::VertexArray(const BasicMesh& mesh)
// {
//     buffer_mesh(mesh);
// }

void VertexArray::bind() const
{
    assert(id);
    glBindVertexArray(id);
}

void VertexArray::add_attribute(const BufferObject& vbo, GLsizei stride, GLint size,
                                GLenum type, GLuint offset)
{
    glEnableVertexArrayAttrib(id, attribs_);
    glVertexArrayVertexBuffer(id, attribs_, vbo.id, 0, stride);
    glVertexArrayAttribFormat(id, attribs_, size, type, GL_FALSE, offset);
    glVertexArrayAttribBinding(id, attribs_, 0);
    attribs_++;
}

void VertexArray::reset()
{
    GLResource::destroy();
    GLResource::create();
    attribs_ = 0;
}

void BufferObject::bind_buffer_base(BindBufferTarget target, GLuint index)
{
    glBindBufferBase(static_cast<GLenum>(target), index, id);
}

void BufferObject::bind_buffer_range(BindBufferTarget target, GLuint index, GLsizeiptr bytes)
{
    // @TODO is this ever anything other than 0?
    glBindBufferRange(static_cast<GLenum>(target), index, id, 0, bytes);
}

void BufferObject::create_store(GLsizeiptr size)
{
    glNamedBufferStorage(id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
}
