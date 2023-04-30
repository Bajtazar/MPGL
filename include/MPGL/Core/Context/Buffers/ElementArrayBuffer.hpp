/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
     * Manages the handle to the OpenGL element array buffer object
     */
    class ElementArrayBuffer : private GraphicalObject {
    public:
        /**
         * Available types of the element array buffer object
         */
        enum class BufferType : uint16 {
            Static                            = GL_STATIC_DRAW,
            Stream                            = GL_STREAM_DRAW,
            Dynamic                           = GL_DYNAMIC_DRAW
        };

        /**
         * Construct a new Element Array Buffer object. Allocates
         * the new OpenGL element array buffer object
         */
        explicit ElementArrayBuffer(void) noexcept;

        ElementArrayBuffer(ElementArrayBuffer const&) = delete;

        /**
         * Construct a new Element Array Buffer object from the given
         * rvalue Element Array Buffer object
         *
         * @param buffer the given rvalue to the element
         * array buffer object
         */
        ElementArrayBuffer(ElementArrayBuffer&& buffer) noexcept;

        ElementArrayBuffer& operator=(
            ElementArrayBuffer const&) = delete;

        /**
         * Assigns the given rvalue Element Array Buffer object
         *
         * @param buffer the given rvalue to the Element Array
         * Buffer object
         * @return reference to this object
         */
        ElementArrayBuffer& operator=(
            ElementArrayBuffer&& buffer) noexcept;

        /**
         * Copy the given data to the element array buffer object
         * and sets the type of the buffer
         *
         * @tparam Range the data range type
         * @param range the range with given data
         * @param type the type of the buffer
         */
        template <std::ranges::contiguous_range Range>
            requires std::ranges::sized_range<Range>
        void setBufferData(Range&& range,
            BufferType const& type = BufferType::Static) const noexcept;

        /**
         * Binds the element array buffer object
         */
        void bind(void) const noexcept;

        /**
         * Unbinds the element array buffer object
         */
        void unbind(void) const noexcept;

        /**
         * Destroy the Element Array Buffer object
         */
        ~ElementArrayBuffer(void) noexcept;

        friend class BuffersManagement;
    private:
        /**
         * Construct a new Element Array Buffer object. Moves the
         * already existing handle to the element array buffer
         *
         * @param elementID the moved element id
         */
        explicit ElementArrayBuffer(uint32 elementID) noexcept;

        /**
         * Returns the reference to the inner buffer
         *
         * @return the reference to the inner buffer
         */
        uint32& getBuffer(void) noexcept
            { return elementID; }

        /**
         * Destroys the buffer handled by this object. Does not
         * violate the object lifetime
         */
        void destroyBuffer(void) noexcept;

        uint32                                  elementID;
    };

}

#include <MPGL/Core/Context/Buffers/ElementArrayBuffer.tpp>
