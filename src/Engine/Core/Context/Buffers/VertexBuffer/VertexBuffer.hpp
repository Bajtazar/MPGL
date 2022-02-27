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
     * Manages the handle to the OpenGL vertex buffer object
     */
    class VertexBuffer : private GraphicalObject {
    public:
        /**
         * Available types of the vertex buffer object
         */
        enum class BufferType : uint16 {
            Static                            = GL_STATIC_DRAW,
            Stream                            = GL_STREAM_DRAW,
            Dynamic                           = GL_DYNAMIC_DRAW
        };

        /**
         * Construct a new Vertex Buffer object. Allocates
         * the new OpenGL vertex buffer object
         */
        explicit VertexBuffer(void) noexcept;

        VertexBuffer(VertexBuffer const&) = delete;

        /**
         * Construct a new Vertex Buffer object from a given
         * rvalue Vertex Buffer object
         *
         * @param buffer the given rvalue vertex buffer object
         */
        VertexBuffer(VertexBuffer&& buffer) noexcept;

        VertexBuffer& operator=(VertexBuffer const&) = delete;

        /**
         * Assigns the given rvalue Vertex Buffer object
         *
         * @param buffer the given rvalue vertex buffer object
         * @return reference to this object
         */
        VertexBuffer& operator=(VertexBuffer&& buffer) noexcept;

        /**
         * Copy the given data to the vertex buffer object
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
         * Maps the pointer to the given buffer
         *
         * @return the pointer to the mapped memory
         */
        [[nodiscard]] void* bindMap(void) const noexcept;

        /**
         * Unbinds the pointer to the given buffer
         */
        void unbindMap(void) const noexcept;

        /**
         * Binds the default buffer
         */
        static void bindDefaultBuffer(void) noexcept;

        /**
         * Destroy the Vertex Buffer object
         */
        ~VertexBuffer(void) noexcept;

        /**
         * Construct a new Vertex Buffer object. Moves the
         * already existing handle to the vertex buffer
         *
         * @param bufferID the moved buffer id
         */
        explicit VertexBuffer(uint32 bufferID) noexcept;
    private:
        uint32                                  bufferID;
    };

    /**
     * Wrapps the Vertex Buffer objects. Allows to use the
     * mapping bindings inside the Bind Guard
     *
     */
    class VertexBufferMapper {
    public:
        /**
         * Construct a new Vertex Buffer Mapper object
         *
         * @param vertexBuffer the constant reference to
         * the vertex buffer
         */
        explicit VertexBufferMapper(
            VertexBuffer const& vertexBuffer) noexcept;

        VertexBufferMapper(VertexBufferMapper const&) noexcept = default;
        VertexBufferMapper(VertexBufferMapper &&) noexcept = default;

        VertexBufferMapper& operator=(
            VertexBufferMapper const&) noexcept = default;
        VertexBufferMapper& operator=(
            VertexBufferMapper&&) noexcept = default;

        /**
         * Calls the bindMap method on vertex buffer reference
         * and returns the pointer to the mapped memory
         *
         * @return the pointer to the mapped memory
         */
        [[nodiscard]] void* bind(void) const noexcept
            { return reference.get().bindMap(); }

        /**
         * Calls the unbindMap method on vertex buffer reference.
         * Using pointer obtained from bind method after calling
         * this method will result in undefined behaviour
         */
        void unbind(void) const noexcept
            { return reference.get().unbindMap(); }

        /**
         * Destroy the Vertex Buffer Mapper object
         */
        ~VertexBufferMapper(void) noexcept = default;
    private:
        using Reference = std::reference_wrapper<VertexBuffer const>;

        Reference                               reference;
    };

    template <std::ranges::contiguous_range Range>
        requires std::ranges::sized_range<Range>
    void VertexBuffer::setBufferData(
        Range&& range,
        BufferType const& type) const noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(std::ranges::range_value_t<Range>) * range.size(),
            // change to std::to_underlying in C++23
            std::ranges::data(range), static_cast<uint16>(type));
        bindDefaultBuffer();
    }

}
