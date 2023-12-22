#pragma once

#include <filesystem>
#include <string_view>
#include <unordered_map>

#include <SFML/Graphics/Image.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace mus
{

    enum class TextureInternalFormat
    {
        RGB = GL_RGB,
        RGBA = GL_RGBA,
    };

    enum class TextureFormat
    {
        RGB8 = GL_RGB8,
        RGBA8 = GL_RGBA8,
        RGBA16F = GL_RGBA16F,
        RGBA32F = GL_RGBA32F
    };

    enum class TextureMinFilter
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
    };

    enum class TextureMagFilter
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
    };

    enum class TextureWrap
    {
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        Repeat = GL_REPEAT,
        MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
    };

    struct GLTextureResource
    {
        GLuint id = 0;

        // clang-format off
        GLTextureResource(GLenum target) { glCreateTextures(target, 1, &id); }   
        virtual ~GLTextureResource() { if(id != 0) glDeleteTextures(1, &id); }

        GLTextureResource           (const GLTextureResource& other) = delete;  
        GLTextureResource& operator=(const GLTextureResource& other) = delete;  

        GLTextureResource& operator=(GLTextureResource&& other) noexcept { id = other.id;  other.id = 0; return *this; }   
        GLTextureResource (GLTextureResource&& other) noexcept : id  (other.id){ other.id = 0; }
        // clang-format on

        void bind(GLuint unit) const
        {
            assert(id);
            glBindTextureUnit(unit, id);
        }

        void set_min_filter(TextureMinFilter filter);
        void set_mag_filter(TextureMagFilter filter);
        void set_wrap_s(TextureWrap wrap);
        void set_wrap_t(TextureWrap wrap);
    };

    struct Texture2D : public GLTextureResource
    {
        Texture2D();

        GLuint create(GLsizei width, GLsizei height, GLsizei levels = 1,
                      TextureFormat format = TextureFormat::RGB8);

        bool
        load_from_file(const std::filesystem::path& path, GLsizei levels, bool flip_vertically,
                       bool flip_horizontally,
                       TextureInternalFormat internal_format = TextureInternalFormat::RGBA,
                       TextureFormat format = TextureFormat::RGBA8);

        bool is_loaded() const;

      private:
        bool is_loaded_ = false;
    };

    struct CubeMapTexture : public GLTextureResource
    {
        CubeMapTexture();

        bool load_from_file(const std::filesystem::path& folder);

        bool is_loaded() const;

      private:
        bool is_loaded_ = false;
    };
} // namespace mus