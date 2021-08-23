#include "Texture.hpp"

#include <memory>

namespace ge {

    template <Allocator Alloc>
    Texture<Alloc>::Options::Options(TextureWrapper verticalWrapping, TextureWrapper horizontalWrapping, MinifyingTextureFilter minifyingFilter, MagnifyingTextureFilter magnifyingFilter, Color borderColor) noexcept
        : verticalWrapping{verticalWrapping}, horizontalWrapping{horizontalWrapping}, minifyingFilter{minifyingFilter}, magnifyingFilter{magnifyingFilter}, borderColor{borderColor} {}

    template <Allocator Alloc>
    Texture<Alloc>::Texture(const Options& options, const Alloc& alloc) noexcept : alloc{alloc},
        textureID{std::allocator_traits<Alloc>::allocate(this->alloc, 1), TextureDeleter{this->alloc}, this->alloc}
    {
        glGenTextures(1, textureID.get());
        glBindTexture(GL_TEXTURE_2D, *textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(options.horizontalWrapping));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(options.verticalWrapping));
        if (options.verticalWrapping == Options::TextureWrapper::ClampToBorder || options.horizontalWrapping == Options::TextureWrapper::ClampToBorder)
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &options.borderColor.red);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(options.minifyingFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(options.magnifyingFilter));
    }

    template <Allocator Alloc>
    Texture<Alloc>::Texture(const std::string& fileName, const Options& options, const Alloc& alloc) : Texture{options, alloc} {
        loadImage(ImageLoader<>{fileName}.getImage());
    }

    template <Allocator Alloc>
    Texture<Alloc>::Texture(const Image& image, const Options& options, const Alloc& alloc) noexcept : Texture{options, alloc} {
        loadImage(image);
    }

    template <Allocator Alloc>
    void Texture<Alloc>::TextureDeleter::operator() (uint32_t* ptr) const noexcept {
        glDeleteTextures(1, ptr);
        std::allocator_traits<Alloc>::deallocate(alloc, ptr, 1);
    }

    template <Allocator Alloc>
    void Texture<Alloc>::loadImage(const Image& image) const {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getMemoryPtr());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    template <Allocator Alloc>
    Texture<Alloc> Texture<Alloc>::defaultTexture(const Options& options) {
        Image image{8, 8};
        for (uint8_t i = 0; i != 4; ++i) {
            for (uint8_t j = 0; j != 4; ++j) {
                image[2*i][2*j] = Image::Pixel(0x7F, 0x7F, 0x7F, 0xFF);
                image[2*i][2*j+1] = Image::Pixel(0x99, 0xD9, 0xEA, 0xFF);
                image[2*i+1][2*j] = Image::Pixel(0x99, 0xD9, 0xEA, 0xFF);
                image[2*i+1][2*j+1] = Image::Pixel(0x7F, 0x7F, 0x7F, 0xFF);
            }
        }
        return Texture<Alloc>{image, options};
    }

}