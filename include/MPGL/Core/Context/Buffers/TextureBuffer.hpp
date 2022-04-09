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

#include <MPGL/Core/Context/Context.hpp>

namespace mpgl {

    /**
     * Buffers Management declaration
     */
    class BuffersManagement;

    /**
     * Manages the handle to the OpenGL texture buffer object
     */
    class TextureBuffer : private GraphicalObject {
    public:
        /**
         * Available image's pixel formats
         */
        enum class PixelFormat : uint16 {
            /// Only red byte
            R                                 = GL_RED,
            /// Red and green byte
            RG                                = GL_RG,
            /// Red, green and blue byte
            RGB                               = GL_RGB,
            /// Blue, green and red byte
            BGR                               = GL_BGR,
            /// Red, green, blue and alpha byte
            RGBA                              = GL_RGBA,
            /// Blue, green, red and alpha byte
            BGRA                              = GL_BGRA
        };

        /**
         * Construct a new Texture Buffer object. Allocates the new
         * OpenGL texture buffer object
         */
        explicit TextureBuffer(void) noexcept;

        TextureBuffer(TextureBuffer const&) = delete;

        /**
         * Construct a new Texture Buffer object from the given rvalue
         * Texture Buffer object
         *
         * @param buffer the given rvalue to the texture buffer object
         */
        TextureBuffer(TextureBuffer&& buffer) noexcept;

        TextureBuffer& operator=(TextureBuffer const&) = delete;

        /**
         * Assings the given rvalue Texture Buffer object
         *
         * @param buffer the given rvalue to the Texture Buffer
         * object
         * @return reference to this object
         */
        TextureBuffer& operator=(TextureBuffer&& buffer) noexcept;

        /**
         * Loads the given image to the texture buffer object
         *
         * @param format the format of the image
         * @param width the width of the image
         * @param height the height of the image
         * @param imagePtr the constant pointer to the image
         */
        void loadImage(
            PixelFormat const& format,
            std::size_t width,
            std::size_t height,
            void const* imagePtr) const noexcept;

        /**
         * Generates mipmaps
         */
        void generateMipmaps(void) const noexcept;

        /**
         * Sets the given parameters on the texture buffer object
         *
         * @param filter the given filter
         * @param mode the given mode
         */
        void setParameter(GLint filter, GLint mode) const noexcept;

        /**
         * Sets the texture border color
         *
         * @param color the texture border color
         */
        void setBorderColor(Color const& color) const noexcept;

        /**
         * Attaches the texture buffer object to the active
         * framebuffer object
         */
        void connectToFramebuffer(void) const noexcept;

        /**
         * Attaches the depth and stencil buffer to the
         * texture buffer object
         */
        void connectToDepthAndStencilBuffer(void) const noexcept;

        /**
         * Activates the given texture buffer object
         */
        void activate(void) const noexcept;

        /**
         * Binds the texture buffer object
         */
        void bind(void) const noexcept;

        /**
         * Unbinds the texture buffer object
         */
        void unbind(void) const noexcept;

        /**
         * Destroy the Texture Buffer object
         */
        ~TextureBuffer(void) noexcept;

        friend class BuffersManagement;
    private:
        /**
         * Construct a new Texture Buffer object. Moves the
         * already existing handle to the texture buffer
         *
         * @param textureID the moved texture id
         */
        explicit TextureBuffer(uint32 textureID) noexcept;

        /**
         * Returns the reference to the inner buffer
         *
         * @return the reference to the inner buffer
         */
        uint32& getBuffer(void) noexcept
            { return textureID; }

        uint32                                  textureID;
    };

}
