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
#include "FrameBuffer/FrameBuffer.hpp"

namespace mpgl {

    namespace details {

        /// Generates normal buffers
        inline auto generateBuffers = [] (uint32 size, uint32* ptr) {
            glGenBuffers(size, ptr);
        };

        /// Generates vertex arrays
        inline auto generateArrays = [] (uint32 size, uint32* ptr) {
            glGenVertexArrays(size, ptr);
        };

        /// Destroys normal buffers
        inline auto destroyBuffers = [] (uint32 size, uint32* const ptr)
        {
            glDeleteBuffers(size, ptr);
        };

        /// Destroys vertex arrays
        inline auto destroyArrays = [] (uint32 size, uint32* const ptr)
        {
            glDeleteVertexArrays(size, ptr);
        };

    }

    /**
     * Allows to allocate many buffers at once, saving time on
     * individual draw calls
     */
    struct BuffersManagement {

        /**
         * Buffer initialization helper. Initializes buffers
         * of the given type
         *
         * @tparam BufferType the buffer type
         * @tparam generator the generating function pointer
         * @tparam Range the type of the returned range
         */
        template <typename BufferType, auto generator,
            UnderlyingRange<BufferType> Range>
        struct BuffersInitializer {

            /**
             * Mass initalizes buffers and returns the range
             * with created buffer objects. Creates buffers
             * faster than individual constructors
             *
             * @tparam Range the type of the returned range
             * @param size the number of the returned buffers
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

        /**
         * Buffer initialization helper. Initializes the Frame
         * Buffers
         *
         * @tparam Range the Frame Buffers range
         */
        template <UnderlyingRange<FrameBuffer> Range>
        struct BuffersInitializer<FrameBuffer, nullptr, Range> {

            /**
             * Mass initalizes frame buffers and returns the range
             * with created buffer objects. Creates buffers
             * faster than individual constructors
             *
             * @param size the number of the returned frame buffers
             * @return the range with created buffers
             */
            [[nodiscard]] Range operator() (uint32 size) const {
                std::vector<uint32> framebuffers;
                std::vector<uint32> renderbuffers;
                framebuffers.resize(size);
                renderbuffers.resize(size);
                glGenFramebuffers(size, framebuffers.data());
                glGenRenderbuffers(size, renderbuffers.data());
                Range handlers;
                handlers.reserve(size);
                for (uint32 i = 0;i < size; ++i)
                    handlers.emplace_back(FrameBuffer{framebuffers[i],
                        renderbuffers[i]});
                return handlers;
            }

        };

        /**
         * Buffet destruction helper. Destroys buffers
         * of the given type
         *
         * @tparam BufferType the buffer type
         * @tparam destroyer the destroying function pointer
         * @tparam Range the type of range with buffers
         */
        template <typename BufferType, auto destroyer,
            UnderlyingRange<BufferType> Range>
        struct BuffersDestroyer {

            /**
             * Destroys all of the given buffers and performs
             * it faster than individual destructors
             *
             * @tparam Range the type of range with buffers
             * @param range the range with buffers
             */
            void operator() (Range&& range) const {
                auto view = range | std::views::transform(
                    [](auto& element) -> uint32& {
                        return element.getBuffer(); });
                std::vector<uint32> bufferIDs{std::ranges::begin(view),
                    std::ranges::end(view)};
                destroyer(bufferIDs.size(), bufferIDs.data());
                std::ranges::fill(view, 0);
            }

        };

        /**
         * Buffet destruction helper. Destroys frame buffers
         *
         * @tparam Range the type of range with frame buffers
         */
        template <UnderlyingRange<FrameBuffer> Range>
        struct BuffersDestroyer<FrameBuffer, nullptr, Range> {

            /**
             * Destroys all of the given frame buffers and
             * performs it faster than individual destructors
             *
             * @tparam Range the type of range with frame buffers
             * @param range the range with frame buffers
             */
            void operator() (Range&& range) const {
                auto frame = range | std::views::transform(
                    &FrameBuffer::frameID);
                auto render = range | std::views::transform(
                    &FrameBuffer::renderID);
                std::vector<uint32> frameIDs{frame.begin(), frame.end()};
                std::vector<uint32> renderIDs{render.begin(), render.end()};
                glDeleteFramebuffers(frameIDs.size(), frameIDs.data());
                glDeleteRenderbuffers(renderIDs.size(), renderIDs.data());
                std::ranges::fill(frame, 0);
                std::ranges::fill(render, 0);
            }

        };

    };

    /**
     * Initializes the vertex buffers
     *
     * @tparam Range the type of the retured range
     * @param size the number of created buffers
     * @return the range with created buffers
     */
    template <UnderlyingRange<VertexBuffer> Range>
    Range initializeVertexBuffers(uint32 size) {
        return BuffersManagement::BuffersInitializer<VertexBuffer,
            details::generateBuffers, Range>{}(size);
    }

    /**
     * Initializes the vertex arrays
     *
     * @tparam Range the type of the retured range
     * @param size the number of created arrays
     * @return the range with created arrays
     */
    template <UnderlyingRange<VertexArray> Range>
    Range initializeVertexArrays(uint32 size) {
        return BuffersManagement::BuffersInitializer<VertexArray,
            details::generateArrays, Range>{}(size);
    }

    /**
     * Initializes the element array buffers
     *
     * @tparam Range the type of the retured range
     * @param size the number of created buffers
     * @return the range with created buffers
     */
    template <UnderlyingRange<ElementArrayBuffer> Range>
    Range initializeElementArrayBuffers(uint32 size) {
        return BuffersManagement::BuffersInitializer<ElementArrayBuffer,
            details::generateBuffers, Range>{}(size);
    }

    /**
     * Initializes the frame buffers
     *
     * @tparam Range the type of the retured range
     * @param size the number of created buffers
     * @return the range with created buffers
     */
    template <UnderlyingRange<FrameBuffer> Range>
    Range initializeFrameBuffers(uint32 size) {
        return BuffersManagement::BuffersInitializer<FrameBuffer,
            nullptr, Range>{}(size);
    }

    /**
     * Destroyes the vertex buffers
     *
     * @tparam Range the type of the range with buffers
     * @param range the range with buffers
     */
    template <UnderlyingRange<VertexBuffer> Range>
    void destroyVertexBuffers(Range&& range) {
        BuffersManagement::BuffersDestroyer<VertexBuffer,
            details::destroyBuffers, Range>
                {}(std::forward<Range>(range));
    }

    /**
     * Destroyes the vertex arrays
     *
     * @tparam Range the type of the range with arrays
     * @param range the range with arrays
     */
    template <UnderlyingRange<VertexArray> Range>
    void destroyVertexArrays(Range&& range) {
        BuffersManagement::BuffersDestroyer<VertexArray,
            details::destroyArrays, Range>
                {}(std::forward<Range>(range));
    }

    /**
     * Destroyes the element array buffers
     *
     * @tparam Range the type of the range with buffers
     * @param range the range with buffers
     */
    template <UnderlyingRange<ElementArrayBuffer> Range>
    void destroyElementArrayBuffers(Range&& range) {
        BuffersManagement::BuffersDestroyer<ElementArrayBuffer,
            details::destroyBuffers, Range>{}(
                std::forward<Range>(range));
    }

    /**
     * Destroyes the frame buffers
     *
     * @tparam Range the type of the range with buffers
     * @param range the range with buffers
     */
    template <UnderlyingRange<FrameBuffer> Range>
    void destroyFrameBuffers(Range&& range) {
        BuffersManagement::BuffersDestroyer<FrameBuffer, nullptr,
            Range>{}(std::forward<Range>(range));
    }

}
