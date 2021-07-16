#pragma once

#include <string>
#include <type_traits>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Color.hpp"
#include "../ImageLoading/ImageLoader.hpp"

namespace ge {

    template <typename Counter = std::size_t, class Allocator = std::allocator<Counter>>
        requires (!std::is_reference_v<Counter>)
    class Texture {
    public:
        struct Options {
            enum class TextureWrapper {
                Repeat = GL_REPEAT,
                MirroredRepeat = GL_MIRRORED_REPEAT,
                ClampToEdge = GL_CLAMP_TO_EDGE,
                ClampToBorder = GL_CLAMP_TO_BORDER
            };
            enum class MinifyingTextureFilter {
                Nearest = GL_NEAREST,
                Linear = GL_LINEAR,
                NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
                NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
                LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
                LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
            };
            enum class MagnifyingTextureFilter {
                Nearest = GL_NEAREST,
                Linear = GL_LINEAR
            };
            TextureWrapper verticalWrapping;
            TextureWrapper horizontalWrapping;
            MinifyingTextureFilter minifyingFilter;
            MagnifyingTextureFilter magnifyingFilter;
            Color borderColor;
            Options(TextureWrapper verticalWrapping = TextureWrapper::Repeat,
                    TextureWrapper horizontalWrapping = TextureWrapper::Repeat,
                    MinifyingTextureFilter minifyingFilter = MinifyingTextureFilter::Linear,
                    MagnifyingTextureFilter magnifyingFilter = MagnifyingTextureFilter::Linear,
                    Color borderColor = {}) noexcept;
        };
        explicit Texture(const std::string& fileName, const Options& options = {}, const Allocator& alloc = {});

        Texture(const Texture& texture);
        Texture(Texture&& texture);

        Texture& operator= (const Texture& texture);
        Texture& operator= (Texture&& texture);

        const uint32_t& getTexture(void) const noexcept { return textureID; }

        ~Texture(void);
    private:
        Allocator allocator;
        Counter* connections;
        uint32_t textureID;
    };

    template class Texture<>;

}
