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

#include <MPGL/Core/Context/Buffers/ElementArrayBuffer.hpp>
#include <MPGL/Core/Context/Buffers/TextureBuffer.hpp>
#include <MPGL/Core/Context/Buffers/VertexBuffer.hpp>
#include <MPGL/Core/Context/Buffers/VertexArray.hpp>
#include <MPGL/Core/Context/Buffers/FrameBuffer.hpp>

namespace mpgl {

    namespace details {

        typedef void(*BuffersGenerator)(uint32, uint32*);
        typedef void(*BuffersDestroyer)(uint32, uint32* const);

        /**
         * Generates the normal buffers
         *
         * @param size the number of generated buffers
         * @param ptr the pointer to the buffers array
         */
        void generateBuffers(
            uint32 size,
            uint32* ptr) noexcept;

        /**
         * Generates the vertex arrays
         *
         * @param size the number of generated buffers
         * @param ptr the pointer to the buffers array
         */
        void generateArrays(
            uint32 size,
            uint32* ptr) noexcept;

        /**
         * Destroys the texture buffers
         *
         * @param size the number of destroyed buffers
         * @param ptr the pointer to the buffers array
         */
        void generateTextures(
            uint32 size,
            uint32* const ptr) noexcept;

        /**
         * Destroys the normal buffers
         *
         * @param size the number of destroyed buffers
         * @param ptr the pointer to the buffers array
         */
        void destroyBuffers(
            uint32 size,
            uint32* const ptr) noexcept;

        /**
         * Destroys the vertex arrays
         *
         * @param size the number of destroyed buffers
         * @param ptr the pointer to the buffers array
         */
        void destroyArrays(
            uint32 size,
            uint32* const ptr) noexcept;

        /**
         * Destroys the texture buffers
         *
         * @param size the number of destroyed buffers
         * @param ptr the pointer to the buffers array
         */
        void destroyTextures(
            uint32 size,
            uint32* const ptr) noexcept;

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
            [[nodiscard]] Range operator() (uint32 size) const;

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
            [[nodiscard]] Range operator() (uint32 size) const;

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
            void operator() (Range&& range) const;

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
            void operator() (Range&& range) const;

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
    [[nodiscard]] Range initializeVertexBuffers(uint32 size);

    /**
     * Initializes the vertex arrays
     *
     * @tparam Range the type of the retured range
     * @param size the number of created arrays
     * @return the range with created arrays
     */
    template <UnderlyingRange<VertexArray> Range>
    [[nodiscard]] Range initializeVertexArrays(uint32 size);

    /**
     * Initializes the element array buffers
     *
     * @tparam Range the type of the retured range
     * @param size the number of created buffers
     * @return the range with created buffers
     */
    template <UnderlyingRange<ElementArrayBuffer> Range>
    [[nodiscard]] Range initializeElementArrayBuffers(uint32 size);

    /**
     * Initializes the frame buffers
     *
     * @tparam Range the type of the retured range
     * @param size the number of created buffers
     * @return the range with created buffers
     */
    template <UnderlyingRange<FrameBuffer> Range>
    [[nodiscard]] Range initializeFrameBuffers(uint32 size);

    /**
     * Initalizes the texture buffers
     *
     * @tparam Range the type of the retured range
     * @param size the number of created buffers
     * @return the range with created buffers
     */
    template <UnderlyingRange<TextureBuffer> Range>
    [[nodiscard]] Range initializeTextureBuffers(uint32 size);

    /**
     * Destroys the vertex buffers
     *
     * @tparam Range the type of the range with buffers
     * @param range the range with buffers
     */
    template <UnderlyingRange<VertexBuffer> Range>
    void destroyVertexBuffers(Range&& range);

    /**
     * Destroys the vertex arrays
     *
     * @tparam Range the type of the range with arrays
     * @param range the range with arrays
     */
    template <UnderlyingRange<VertexArray> Range>
    void destroyVertexArrays(Range&& range);

    /**
     * Destroys the element array buffers
     *
     * @tparam Range the type of the range with buffers
     * @param range the range with buffers
     */
    template <UnderlyingRange<ElementArrayBuffer> Range>
    void destroyElementArrayBuffers(Range&& range);

    /**
     * Destroys the frame buffers
     *
     * @tparam Range the type of the range with buffers
     * @param range the range with buffers
     */
    template <UnderlyingRange<FrameBuffer> Range>
    void destroyFrameBuffers(Range&& range);

    /**
     * Destroys the texture buffers
     *
     * @tparam Range the type of the range with buffers
     * @param range the range with buffers
     */
    template <UnderlyingRange<TextureBuffer> Range>
    void destroyTextureBuffers(Range&& range);

}

#include <MPGL/Core/Context/Buffers/Helpers.tpp>
