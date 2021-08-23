#pragma once

#include <string>
#include <memory>
#include <type_traits>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Color.hpp"
#include "../Traits/Concepts.hpp"
#include "../ImageLoading/ImageLoader.hpp"

namespace ge {

    template <Allocator Alloc = std::allocator<uint32_t>>
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
        typedef Alloc                           allocator_type;

        explicit Texture(const std::string& fileName, const Options& options = {}, const Alloc& alloc = {});
        template <security::SecurityPolicy Policy>
        explicit Texture(Policy policy, const std::string& fileName, const Options& options = {}, const Alloc& alloc = {});
        explicit Texture(const Image& image, const Options& options = {}, const Alloc& alloc = {}) noexcept;

        Texture(const Texture& texture) = default;
        Texture(Texture&& texture) = default;

        Texture& operator= (const Texture& texture) = default;
        Texture& operator= (Texture&& texture) = default;

        const uint32_t& getTexture(void) const noexcept { return *textureID; }

        static Texture defaultTexture(const Options& options = {
            Options::TextureWrapper::ClampToEdge,
            Options::TextureWrapper::ClampToEdge,
            Options::MinifyingTextureFilter::Nearest,
            Options::MagnifyingTextureFilter::Nearest
        });

        ~Texture(void) = default;
    private:
        class TextureDeleter {
        public:
            explicit TextureDeleter(const Alloc& alloc) noexcept : alloc{alloc} {}

            void operator()(uint32_t* ptr) const noexcept;
        private:
            [[no_unique_address]] mutable Alloc alloc;
        };

        explicit Texture(const Options& options, const Alloc& alloc) noexcept;

        void loadImage(const Image& image) const;

        [[no_unique_address]] Alloc alloc;
        std::shared_ptr<uint32_t> textureID;
    };

    template class Texture<>;

    template <Allocator Alloc>
    template <security::SecurityPolicy Policy>
    Texture<Alloc>::Texture(Policy policy, const std::string& fileName, const Options& options, const Alloc& alloc)
        : Texture{options, alloc}
    {
        loadImage(ImageLoader{policy, fileName}.getImage());
    }

}
