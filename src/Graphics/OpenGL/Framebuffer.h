#pragma once

#include "GLResource.h"
#include "Texture.h"

#include <unordered_map>

struct Framebuffer : public GLResource<glCreateFramebuffers, glDeleteFramebuffers>
{
    Framebuffer(GLuint width, GLuint height);
    ~Framebuffer();

    void bind() const;
    void bind_texture(GLuint index, GLuint unit) const;
    GLuint get_texture_id(GLuint index) const;
    Texture2D& get_texture(GLuint index);

    Framebuffer& attach_colour(TextureFormat format, const TextureParameters& filters = {});
    Framebuffer& attach_renderbuffer();
    Framebuffer& attach_depth_buffer();

    bool is_complete() const;

  private:
    std::vector<Texture2D> textures_;
    std::vector<GLuint> renderbuffers_;
    GLuint width = 0;
    GLuint height = 0;
};
