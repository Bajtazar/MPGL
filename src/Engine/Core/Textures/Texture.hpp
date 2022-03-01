/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include "../Context/Buffers/TextureBuffer/TextureBuffer.hpp"
#include "../../IO/ImageLoading/ImageLoader.hpp"
#include "../../Collections/Bitmap.hpp"

namespace mpgl {

    /**
     * Initializes and holds a texture inside the VRAM
     */
    class Texture {
    public:
        /**
         * Texture initialization options. Sets the behaviour
         * of texture when it is being stretched etc.
         */
        struct Options {
            /**
             * Specifies the behaviour when texture is being
             * stretched outside its boundries
             */
            enum class TextureWrapper : GLint {
                /// Repeats the texture
                Repeat
                    = GL_REPEAT,
                /// Repeats and mirrors the texture
                MirroredRepeat
                    = GL_MIRRORED_REPEAT,
                /// Clamps texture to edge
                ClampToEdge
                    = GL_CLAMP_TO_EDGE,
                /// Clamps texture to border
                ClampToBorder
                    = GL_CLAMP_TO_BORDER
            };

            /**
             * Specifies the behaviour when texture is being
             * downscaled
             */
            enum class MinifyingTextureFilter : GLint {
                /// The nearest pixel color
                Nearest
                    = GL_NEAREST,
                /// The linear interpolation of colors of the
                /// neighbour pixels
                Linear
                    = GL_LINEAR,
                /// Takes the nearest mipmam in size and uses
                /// the Nearest mode
                NearestMipmapNearest
                    = GL_NEAREST_MIPMAP_NEAREST,
                /// Takes the nearest mipmam in size and uses
                /// the Linear mode
                NearestMipmapLinear
                    = GL_NEAREST_MIPMAP_LINEAR,
                /// Interpolates linearly between two mipmaps
                /// and uses the Nearest mode
                LinearMipmapNearest
                    = GL_LINEAR_MIPMAP_NEAREST,
                /// Interpolates linearly between two mipmaps
                /// and uses the Linear mode
                LinearMipmapLinear
                    = GL_LINEAR_MIPMAP_LINEAR
            };

            /**
             * Specifies the behaviour when texture is being
             * upscaled
             */
            enum class MagnifyingTextureFilter : GLint {
                /// The nearest pixel color
                Nearest
                    = GL_NEAREST,
                /// Takes the nearest mipmam in size and uses
                /// the Nearest mode
                Linear
                    = GL_LINEAR
            };

            /// Specifies behaviour in the vertical direction
            TextureWrapper                          verticalWrapping;
            /// Specifies behaviour in the horizontal direction
            TextureWrapper                          horizontalWrapping;
            /// Specifies the downscaling behaviour
            MinifyingTextureFilter                  minifyingFilter;
            /// Specifies the upscaling behaviour
            MagnifyingTextureFilter                 magnifyingFilter;
            /// The border color [if border is enabled]
            Color                                   borderColor;
            /// Indicates whether mipmaps are enabled
            bool                                    mipmaps;

            /**
             * Construct the texture options object
             */
            constexpr Options(
                TextureWrapper verticalWrapping
                    = TextureWrapper::Repeat,
                TextureWrapper horizontalWrapping
                    = TextureWrapper::Repeat,
                MinifyingTextureFilter minifyingFilter
                    = MinifyingTextureFilter::Linear,
                MagnifyingTextureFilter magnifyingFilter
                    = MagnifyingTextureFilter::Linear,
                Color const& color = {},
                bool mipmaps = true) noexcept;

            typedef std::pair<GLint, GLint> Filter;
            typedef std::array<Filter, 4>   Underlying;

            /**
             * Returns an array containing filters and modes generated
             * by current options
             *
             * @return the array with filters and modes
             */
            constexpr Underlying getOptions(void) const noexcept;

            /**
             * Returns whether the border is present in the texture
             *
             * @return whether the border is present
             */
            constexpr bool isBorder(void) const noexcept;
        };

        /**
         * Construct a new Texture object. Loads an image from the
         * given path and initializes texture
         *
         * @param fileName the path to the image file
         * @param options the texture initialization options
         */
        explicit Texture(
            std::string const& fileName,
            Options const& options = Options{});

        /**
         * Construct a new Texture object. Loads an image with given
         * policy from the given path and initializes textures
         *
         * @tparam Policy the policy type
         * @param policy the policy tag
         * @param fileName the path to the image file
         * @param options the texture initialization options
         */
        template <security::SecurityPolicy Policy>
        explicit Texture(
            Policy policy,
            std::string const& fileName,
            Options const& options = Options{});

        /**
         * Construct a new Texture object from a given
         * image object with given options
         *
         * @param image the image object
         * @param options the texture initialization options
         */
        explicit Texture(Image const& image,
            Options const& options = Options{});

        /**
         * Construct a new Texture object from a given
         * bitmap object with given options
         *
         * @param bitmap the bitmap object
         * @param options the texture initialization options
         */
        explicit Texture(Bitmap const& bitmap,
            Options const& options = Options{});

        /**
         * Construct an empty texture with given options
         *
         * @param options the texture initialization options
         */
        explicit Texture(Options const& options = Options{});

        Texture(Texture const& texture) = default;
        Texture(Texture&& texture) noexcept = default;

        Texture& operator= (Texture const& texture) = default;
        Texture& operator= (Texture&& texture) noexcept = default;

        /**
         * Returns a constant reference to the texture buffer object
         *
         * @return the constant reference to the texture buffer object
         */
        TextureBuffer const& getTextureBuffer(void) const noexcept
            { return *texturePtr; }

        /**
         * Returns the newly created texture with a pattern indicating
         * default texture in MPGL
         *
         * @param options the texture initialization options
         * @return the texture with the default pattern
         */
        static Texture defaultTexture(Options const& options = {
            Options::TextureWrapper::ClampToEdge,
            Options::TextureWrapper::ClampToEdge,
            Options::MinifyingTextureFilter::Nearest,
            Options::MagnifyingTextureFilter::Nearest});

        /**
         * Destroy the Texture object
         */
        ~Texture(void) noexcept = default;
    private:
        typedef std::shared_ptr<TextureBuffer>      BufferPtr;

        BufferPtr                                   texturePtr;
    };

    template <security::SecurityPolicy Policy>
    Texture::Texture(
        Policy policy,
        std::string const& fileName,
        Options const& options) : Texture{
            ImageLoader{policy, fileName}.getImage(), options} {}

    constexpr Texture::Options::Options(
        TextureWrapper verticalWrapping,
        TextureWrapper horizontalWrapping,
        MinifyingTextureFilter minifyingFilter,
        MagnifyingTextureFilter magnifyingFilter,
        Color const& color,
        bool mipmaps) noexcept
            : verticalWrapping{verticalWrapping},
            horizontalWrapping{horizontalWrapping},
            minifyingFilter{minifyingFilter},
            magnifyingFilter{magnifyingFilter},
            borderColor{color},
            mipmaps{mipmaps} {}

    constexpr Texture::Options::Underlying
        Texture::Options::getOptions(void) const noexcept
    {
        // change static_cast to std::to_underlying in C++23
        return {
            Filter{GL_TEXTURE_WRAP_S,
                static_cast<GLint>(verticalWrapping)},
            Filter{GL_TEXTURE_WRAP_T,
                static_cast<GLint>(horizontalWrapping)},
            Filter{GL_TEXTURE_MIN_FILTER,
                static_cast<GLint>(minifyingFilter)},
            Filter{GL_TEXTURE_MAG_FILTER,
                static_cast<GLint>(magnifyingFilter)},
        };
    }

    constexpr bool Texture::Options::isBorder(void) const noexcept {
        return verticalWrapping == TextureWrapper::ClampToBorder ||
            horizontalWrapping == TextureWrapper::ClampToBorder;
    }

}
