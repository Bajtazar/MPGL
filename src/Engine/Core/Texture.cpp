#include "Texture.hpp"
#include "../ImageLoading/ImageLoader.hpp"

namespace ge {

    Texture::Options::Options(TextureWrapper verticalWrapping, TextureWrapper horizontalWrapping, MinifyingTextureFilter minifyingFilter, MagnifyingTextureFilter magnifyingFilter, Color borderColor) noexcept
        : verticalWrapping{verticalWrapping}, horizontalWrapping{horizontalWrapping}, minifyingFilter{minifyingFilter}, magnifyingFilter{magnifyingFilter}, borderColor{borderColor} {}

    // RGBA detection will be added
    Texture::Texture(const std::string& fileName, Options options) {
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

}
