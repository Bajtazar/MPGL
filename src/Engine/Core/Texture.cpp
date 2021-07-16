#include "Texture.hpp"

#include <memory>

namespace ge {

    template <typename Counter, class Allocator>
        requires (!std::is_reference_v<Counter>)
    Texture<Counter, Allocator>::Options::Options(TextureWrapper verticalWrapping, TextureWrapper horizontalWrapping, MinifyingTextureFilter minifyingFilter, MagnifyingTextureFilter magnifyingFilter, Color borderColor) noexcept
        : verticalWrapping{verticalWrapping}, horizontalWrapping{horizontalWrapping}, minifyingFilter{minifyingFilter}, magnifyingFilter{magnifyingFilter}, borderColor{borderColor} {}

    // RGBA detection will be added
    template <typename Counter, class Allocator>
        requires (!std::is_reference_v<Counter>)
    Texture<Counter, Allocator>::Texture(const std::string& fileName, const Options& options, const Allocator& alloc) : textureID{0}, allocator{alloc}, connections{std::allocator_traits<Allocator>::allocate(this->allocator, 1)} {
        *connections = 1;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(options.horizontalWrapping));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(options.verticalWrapping));
        if (options.verticalWrapping == Options::TextureWrapper::ClampToBorder || options.horizontalWrapping == Options::TextureWrapper::ClampToBorder)
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &options.borderColor.red);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(options.minifyingFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(options.magnifyingFilter));
        ImageLoader loader{fileName};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loader.getWidth(), loader.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loader.memoryPointer());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    template <typename Counter, class Allocator>
        requires (!std::is_reference_v<Counter>)
    Texture<Counter, Allocator>::Texture(const Texture& texture) : allocator{texture.allocator}, textureID{texture.textureID}, connections{texture.connections} {
        ++(*connections);
    }

    template <typename Counter, class Allocator>
        requires (!std::is_reference_v<Counter>)
    Texture<Counter, Allocator>::Texture(Texture&& texture) : allocator{texture.allocator}, textureID{texture.textureID}, connections{texture.connections} {
        ++(*connections);
    }

    template <typename Counter, class Allocator>
        requires (!std::is_reference_v<Counter>)
    Texture<Counter, Allocator>& Texture<Counter, Allocator>::operator= (const Texture& texture) {
        if (texture.textureID != textureID) {
            this->~Texture();
            textureID = texture.textureID;
            connections = texture.connections;
            ++(*connections);
        }
        return *this;
    }

    template <typename Counter, class Allocator>
        requires (!std::is_reference_v<Counter>)
    Texture<Counter, Allocator>& Texture<Counter, Allocator>::operator= (Texture&& texture) {
        if (texture.textureID != textureID) {
            this->~Texture();
            textureID = texture.textureID;
            connections = texture.connections;
            ++(*connections);
        }
        return *this;
    }

    template <typename Counter, class Allocator>
        requires (!std::is_reference_v<Counter>)
    Texture<Counter, Allocator>::~Texture(void) {
        if(!(--(*connections))) {
            std::allocator_traits<Allocator>::deallocate(allocator, connections, 1);
            glDeleteTextures(1, &textureID);
        }
    }

}