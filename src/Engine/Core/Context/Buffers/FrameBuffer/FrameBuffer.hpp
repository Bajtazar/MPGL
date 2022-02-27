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

#include "../../Context.hpp"

namespace mpgl {

    /**
     * Buffers Management declaration
     */
    class BuffersManagement;

    /**
     * Manages the handle to the OpenGL frame buffer object and
     * render buffer object
     */
    class FrameBuffer : private GraphicalObject {
    public:
        /**
         * Construct a new Frame Buffer object. Allocates
         * the new OpenGL frame buffer object and
         * render buffer object
         */
        explicit FrameBuffer(void) noexcept;

        FrameBuffer(FrameBuffer const&) = delete;

        /**
         * Construct a new Frame Buffer object from a given
         * rvalue Frame Buffer object
         *
         * @param buffer the given rvalue frame buffer object
         */
        FrameBuffer(FrameBuffer&& buffer) noexcept;

        FrameBuffer& operator=(FrameBuffer const&) = delete;

        /**
         * Assigns the given rvalue Frame Buffer object
         *
         * @param buffer the given rvalue frame buffer object
         * @return reference to this object
         */
        FrameBuffer& operator=(FrameBuffer&& buffer) noexcept;

        /**
         * Binds the framebuffer object
         */
        void bind(void) const noexcept;

        /**
         * Unbinds the framebuffer object
         */
        void unbind(void) const noexcept;

        /**
         * Attaches render buffer object to the frame buffer
         * object
         */
        void connectRenderBuffer(void) const noexcept;

        /**
         * Clears the framebuffer background
         *
         * @param cleaning the cleaning options
         * @param color the background color
         */
        void clear(CleaningOptions const& cleaning,
            Color const& color) const noexcept;

        /**
         * Returns the framebuffer compilation status
         *
         * @return the framebuffer compilation status
         */
        [[nodiscard]] bool status(void) const noexcept;

        /**
         * Destroy the Frame Buffer object
         */
        ~FrameBuffer(void) noexcept;

        friend class BuffersManagement;
    private:
        /**
         * Construct a new Frame Buffer object. Moves the already
         * existing handle to the frame buffer and render buffer
         *
         * @param frameID the moved frame buffer id
         * @param renderID the moved render buffer id
         */
        explicit FrameBuffer(uint32 frameID, uint32 renderID) noexcept;

        uint32                              frameID;
        uint32                              renderID;
    };

}
