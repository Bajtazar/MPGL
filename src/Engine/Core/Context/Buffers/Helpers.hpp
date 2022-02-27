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

#include "ElementArrayBuffer/ElementArrayBuffer.hpp"
#include "VertexBuffer/VertexBuffer.hpp"
#include "VertexArray/VertexArray.hpp"

namespace mpgl {

    typedef void(*BufferGenerator)(GLsizei, GLuint*);
    typedef void(*BufferDestroyer)(GLsizei, const GLuint*);

    /**
     * Buffet initialization helper. Initializes buffers
     * of the given type
     *
     * @tparam BufferType the buffer type
     * @tparam generator the generating function pointer
     * @tparam Range the range with created buffers
     */
    template <typename BufferType,
        BufferGenerator generator,
        UnderlyingRange<BufferType> Range>
    struct BuffersInitializer {

        /**
         * Mass initalizes buffers and returns the range
         * with created buffer objects. Creates buffers
         * faster than individual constructors
         *
         * @tparam Range the type of the returned range
         * @param size the number of th returned buffers
         * @return the range with created buffers
         */
        [[nodiscard]] Range operator() (uint32 size) const {
            std::vector<uint32> buffers;
            buffers.resize(size);
            generator(size, buffers.data());
            Range handlers;
            handlers.reserve(size);
            std::ranges::transform(
                buffers, std::back_inserter(handlers),
                [](auto const& bufferID) { return BufferType(bufferID); });
            return handlers;
        }

    };

    template <UnderlyingRange<VertexBuffer> Range>
    using initializeVertexBuffers = BuffersInitializer<
        VertexBuffer, glGenBuffers, Range>;

    template <UnderlyingRange<VertexArray> Range>
    using initializeVertexArrays = BuffersInitializer<
        VertexArray, glGenVertexArrays, Range>;

    template <UnderlyingRange<ElementArrayBuffer> Range>
    using initializElementArrayBuffers = BuffersInitializer<
        ElementArrayBuffer, glGenBuffers, Range>;

    /**
     * Buffet destruction helper. Destroys buffers
     * of the given type
     *
     * @tparam BufferType the buffer type
     * @tparam destroyer the destroying function pointer
     * @tparam projection the projection with the buffer
     * member pointer
     * @tparam Range the range with buffers
     */
    template <typename BufferType,
        BufferDestroyer destroyer,
        uint32 BufferType::* projection,
        UnderlyingRange<BufferType> Range>
    struct BuffersDestroyer {

        /**
         * Destroys all of the given buffers and performs
         * faster than individual destructors
         *
         * @tparam Range the type of range with buffers
         * @param range the range with buffers
         */
        void operator() (Range&& range) const {
            auto view = range | std::views::transform(projection);
            std::vector<uint32> bufferIDs{std::ranges::begin(view),
                std::ranges::end(view)};
            destroyer(bufferIDs.size(), bufferIDs.data());
            std::ranges::fill(view, 0);
        }

    };

    template <UnderlyingRange<VertexBuffer> Range>
    using destroyVertexBuffers = BuffersDestroyer<VertexBuffer,
        glDeleteBuffers, &VertexBuffer::bufferID, Range>;

    template <UnderlyingRange<VertexArray> Range>
    using destroyVertexArrays = BuffersDestroyer<VertexArray,
        glDeleteVertexArrays, &VertexArray::arrayID, Range>;

    template <UnderlyingRange<ElementArrayBuffer> Range>
    using destroyElementArrayBuffers = BuffersDestroyer<
        ElementArrayBuffer, glDeleteBuffers,
        &ElementArrayBuffer::elementID, Range>;

}
