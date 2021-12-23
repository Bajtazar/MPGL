#include "Texture.hpp"

#include <memory>

namespace ge {

    Texture::Options::Options(TextureWrapper verticalWrapping,
        TextureWrapper horizontalWrapping,
        MinifyingTextureFilter minifyingFilter,
        MagnifyingTextureFilter magnifyingFilter,
        Color borderColor, bool mipmaps) noexcept
            : verticalWrapping{verticalWrapping},
            horizontalWrapping{horizontalWrapping},
            minifyingFilter{minifyingFilter},
            magnifyingFilter{magnifyingFilter},
            borderColor{borderColor}, mipmaps(mipmaps) {}

    Texture::Options::Underlying
        Texture::Options::getOptions(void) const noexcept
    {
        return {
            Filter{GL_TEXTURE_WRAP_S, static_cast<GLint>(verticalWrapping)},
            Filter{GL_TEXTURE_WRAP_T, static_cast<GLint>(horizontalWrapping)},
            Filter{GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minifyingFilter)},
            Filter{GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magnifyingFilter)},
        };
    }

    Texture::Deleter const Texture::deleter = [](uint32_t* ptr) -> void
        {  glDeleteTextures(1, ptr); if (ptr) delete ptr; };

    bool Texture::Options::isBorder(void) const noexcept {
        return verticalWrapping == TextureWrapper::ClampToBorder ||
            horizontalWrapping == TextureWrapper::ClampToBorder;
    }

    Texture::Texture(Options const& options) noexcept :
        textureID{new uint32_t{0}, deleter}
    {
        glGenTextures(1, textureID.get());
        glBindTexture(GL_TEXTURE_2D, *textureID);
        for (auto const& [filter, mode] : options.getOptions())
            glTexParameteri(GL_TEXTURE_2D, filter, mode);
        if (options.isBorder())
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                &options.borderColor.red);
    }

    Texture::Texture(std::string const& fileName,
        Options const& options) : Texture{options}
    {
        loadImage(ImageLoader<>{fileName}.getImage(), options);
    }

    Texture::Texture(Image const& image,
        Options const& options) noexcept : Texture{options}
    {
        loadImage(image, options);
    }

    void Texture::generateMipmaps(Options const& options) const noexcept {
        if (options.mipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
    }

    Texture::Texture(Bitmap const& bitmap,
        Options const& options) noexcept : Texture{options}
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.getWidth(),
            bitmap.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE,
            bitmap.getMemoryPtr());
        generateMipmaps(options);
    }

    void Texture::loadImage(Image const& image,
        Options const& options) const
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(),
            image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
            image.getMemoryPtr());
        generateMipmaps(options);
    }

    Texture Texture::defaultTexture(Options const& options) {
        Image image{8, 8};
        for (uint8_t i = 0; i != 4; ++i) {
            for (uint8_t j = 0; j != 4; ++j) {
                image[2*i][2*j] = Pixel(0x7F, 0x7F, 0x7F, 0xFF);
                image[2*i][2*j+1] = Pixel(0x99, 0xD9, 0xEA, 0xFF);
                image[2*i+1][2*j] = Pixel(0x99, 0xD9, 0xEA, 0xFF);
                image[2*i+1][2*j+1] = Pixel(0x7F, 0x7F, 0x7F, 0xFF);
            }
        }
        return Texture{image, options};
    }

}