#include "Texture.h"

#include <SFML/Graphics/Image.hpp>
#include <array>
#include <iostream>

//=======================
// == Helper functions ==
//=======================
namespace
{
    const std::array<std::string, 6> CUBE_TEXTURE_NAMES = {"right.png",  "left.png",  "top.png",
                                                           "bottom.png", "back.png", "front.png"};

    bool load_image_from_file(const std::filesystem::path& path, bool flip_vertically,
                              bool flip_horizontally, sf::Image& out_image)
    {
        if (!out_image.loadFromFile(path.string()))
        {
            return false;
        }

        if (flip_vertically)
        {
            out_image.flipVertically();
        }
        if (flip_horizontally)
        {
            out_image.flipHorizontally();
        }

        return true;
    }
} // namespace

//=======================================
// == GLTextureResource Implementation ==
//=======================================
void GLTextureResource::set_min_filter(TextureMinFilter filter)
{
    assert(id != 0);
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filter));
}

void GLTextureResource::set_mag_filter(TextureMagFilter filter)
{
    assert(id != 0);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filter));
}

void GLTextureResource::set_wrap_s(TextureWrap wrap)
{
    assert(id != 0);
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrap));
}

void GLTextureResource::set_wrap_t(TextureWrap wrap)
{
    assert(id != 0);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrap));
}

//===============================
// == Texture2D Implementation ==
//================================
Texture2D::Texture2D()
    : GLTextureResource(GL_TEXTURE_2D)
{
}

GLuint Texture2D::create(GLsizei width, GLsizei height, GLsizei levels, TextureFormat format)
{
    glTextureStorage2D(id, levels, static_cast<GLenum>(format), width, height);

    set_min_filter(TextureMinFilter::Linear);
    set_mag_filter(TextureMagFilter::Linear);

    return id;
}

bool Texture2D::load_from_file(const std::filesystem::path& path, GLsizei levels,
                               bool flip_vertically, bool flip_horizontally,
                               TextureInternalFormat internal_format, TextureFormat format)
{
    sf::Image image;
    if (!load_image_from_file(path, flip_vertically, flip_horizontally, image))
        return false;

    const auto w = image.getSize().x;
    const auto h = image.getSize().y;
    const auto data = image.getPixelsPtr();

    // Allocate the storage
    glTextureStorage2D(id, levels, static_cast<GLenum>(format), w, h);

    // Uplodad the pixels
    glTextureSubImage2D(id, 0, 0, 0, w, h, static_cast<GLenum>(internal_format), GL_UNSIGNED_BYTE,
                        data);
    glGenerateTextureMipmap(id);

    // Set some default wrapping
    set_min_filter(TextureMinFilter::LinearMipmapLinear);
    set_mag_filter(TextureMagFilter::Linear);
    set_wrap_s(TextureWrap::Repeat);
    set_wrap_t(TextureWrap::Repeat);
    is_loaded_ = true;
    return true;
}

bool Texture2D::is_loaded() const
{
    return is_loaded_;
}

CubeMapTexture::CubeMapTexture()
    : GLTextureResource(GL_TEXTURE_CUBE_MAP)
{
}

bool CubeMapTexture::load_from_file(const std::filesystem::path& folder)
{
    // Assumes textures are called the following in the given folder:
    // 1 RIGHT
    // 2 LEFT
    // 3 TOP
    // 4 BOTTOM
    // 5 FRONT
    // 6 BACK

    bool created = false;



    for (int i = 0; i < 6; i++)
    {
        sf::Image image;
        if (!load_image_from_file(folder / CUBE_TEXTURE_NAMES[i], false, false, image))
            return false;

        const auto w = image.getSize().x;
        const auto h = image.getSize().y;
        const auto data = image.getPixelsPtr();

        if (!created)
        {
            glTextureStorage2D(id, 1, GL_RGBA8, w, h);
            created = true;     
        }

        glTextureSubImage3D(id, 0, 0, 0, i, w, h, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    set_min_filter(TextureMinFilter::Linear);
    set_mag_filter(TextureMagFilter::Linear);
    set_wrap_s(TextureWrap::ClampToEdge);
    set_wrap_t(TextureWrap::ClampToEdge);
}

bool CubeMapTexture::is_loaded() const
{
    return false;
}
