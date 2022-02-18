#pragma once

#include <string>
#include <memory>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Color.hpp"
#include "../Traits/Concepts.hpp"
#include "../Collections/Bitmap.hpp"
#include "../IO/ImageLoading/ImageLoader.hpp"

namespace mpgl {

    class Texture {
    public:
        class Options {
        public:
            enum class TextureWrapper : GLint {
                Repeat                    = GL_REPEAT,
                MirroredRepeat            = GL_MIRRORED_REPEAT,
                ClampToEdge               = GL_CLAMP_TO_EDGE,
                ClampToBorder             = GL_CLAMP_TO_BORDER
            };
            enum class MinifyingTextureFilter : GLint {
                Nearest                   = GL_NEAREST,
                Linear                    = GL_LINEAR,
                NearestMipmapNearest      = GL_NEAREST_MIPMAP_NEAREST,
                NearestMipmapLinear       = GL_NEAREST_MIPMAP_LINEAR,
                LinearMipmapNearest       = GL_LINEAR_MIPMAP_NEAREST,
                LinearMipmapLinear        = GL_LINEAR_MIPMAP_LINEAR
            };
            enum class MagnifyingTextureFilter : GLint {
                Nearest                   = GL_NEAREST,
                Linear                    = GL_LINEAR
            };
            TextureWrapper                  verticalWrapping;
            TextureWrapper                  horizontalWrapping;
            MinifyingTextureFilter          minifyingFilter;
            MagnifyingTextureFilter         magnifyingFilter;
            Color                           borderColor;
            bool                            mipmaps;

            Options(TextureWrapper verticalWrapping =
                        TextureWrapper::Repeat,
                    TextureWrapper horizontalWrapping =
                        TextureWrapper::Repeat,
                    MinifyingTextureFilter minifyingFilter =
                        MinifyingTextureFilter::Linear,
                    MagnifyingTextureFilter magnifyingFilter =
                        MagnifyingTextureFilter::Linear,
                    Color borderColor = {},
                    bool mipmaps = true) noexcept;

            friend class Texture;
        private:
            typedef std::pair<GLint, GLint> Filter;
            typedef std::array<Filter, 4>   Underlying;

            Underlying getOptions(void) const noexcept;
            bool isBorder(void) const noexcept;
        };

        explicit Texture(std::string const& fileName,
            Options const& options = {});
        template <security::SecurityPolicy Policy>
        explicit Texture(Policy policy, std::string const& fileName,
            Options const& options = {});
        explicit Texture(Image const& image,
            Options const& options = {}) noexcept;
        explicit Texture(Bitmap const& bitmap,
            Options const& options = {}) noexcept;

        Texture(Texture const& texture) = default;
        Texture(Texture&& texture) = default;

        Texture& operator= (Texture const& texture) = default;
        Texture& operator= (Texture&& texture) = default;

        uint32 getTexture(void) const noexcept
            { return *textureID; }

        static Texture defaultTexture(Options const& options = {
            Options::TextureWrapper::ClampToEdge,
            Options::TextureWrapper::ClampToEdge,
            Options::MinifyingTextureFilter::Nearest,
            Options::MagnifyingTextureFilter::Nearest
        });

        ~Texture(void) = default;
    private:
        typedef std::function<void(
            uint32*)>                       Deleter;

        explicit Texture(Options const& options) noexcept;

        void loadImage(Image const& image,
            Options const& options) const;
        void generateMipmaps(Options const& options) const noexcept;

        std::shared_ptr<uint32>             textureID;

        static const Deleter                deleter;
    };

    template <security::SecurityPolicy Policy>
    Texture::Texture(Policy policy, std::string const& fileName,
        Options const& options)
            : Texture{options}
    {
        loadImage(ImageLoader{policy, fileName}.getImage());
    }

}
