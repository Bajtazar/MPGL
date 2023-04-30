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

namespace mpgl {

    template <typename BufferType, auto generator,
        UnderlyingRange<BufferType> Range>
    [[nodiscard]] Range
        BuffersManagement::BuffersInitializer<BufferType,
            generator, Range>::operator() (uint32 size) const
    {
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

    template <UnderlyingRange<FrameBuffer> Range>
    [[nodiscard]] Range BuffersManagement::BuffersInitializer<
        FrameBuffer, nullptr, Range>::operator() (uint32 size) const
    {
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

    template <typename BufferType, auto destroyer,
        UnderlyingRange<BufferType> Range>
    void BuffersManagement::BuffersDestroyer<BufferType,
            destroyer, Range>::operator() (Range&& range) const
    {
        auto view = range | std::views::transform(
            [](auto& element) -> uint32& {
                return element.getBuffer(); });
        std::vector<uint32> bufferIDs{std::ranges::begin(view),
            std::ranges::end(view)};
        destroyer(bufferIDs.size(), bufferIDs.data());
        std::ranges::fill(view, 0);
    }

    template <UnderlyingRange<FrameBuffer> Range>
    void BuffersManagement::BuffersDestroyer<
        FrameBuffer, nullptr, Range>::operator() (Range&& range) const
    {
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

    template <UnderlyingRange<VertexBuffer> Range>
    [[nodiscard]] Range initializeVertexBuffers(uint32 size) {
        return BuffersManagement::BuffersInitializer<VertexBuffer,
            details::generateBuffers, Range>{}(size);
    }

    template <UnderlyingRange<VertexArray> Range>
    [[nodiscard]] Range initializeVertexArrays(uint32 size) {
        return BuffersManagement::BuffersInitializer<VertexArray,
            details::generateArrays, Range>{}(size);
    }

    template <UnderlyingRange<ElementArrayBuffer> Range>
    [[nodiscard]] Range initializeElementArrayBuffers(uint32 size) {
        return BuffersManagement::BuffersInitializer<ElementArrayBuffer,
            details::generateBuffers, Range>{}(size);
    }

    template <UnderlyingRange<FrameBuffer> Range>
    [[nodiscard]] Range initializeFrameBuffers(uint32 size) {
        return BuffersManagement::BuffersInitializer<FrameBuffer,
            nullptr, Range>{}(size);
    }

    template <UnderlyingRange<TextureBuffer> Range>
    [[nodiscard]] Range initializeTextureBuffers(uint32 size) {
        return BuffersManagement::BuffersInitializer<TextureBuffer,
            details::generateTextures, Range>{}(size);
    }

    template <UnderlyingRange<VertexBuffer> Range>
    void destroyVertexBuffers(Range&& range) {
        BuffersManagement::BuffersDestroyer<VertexBuffer,
            details::destroyBuffers, Range>
                {}(std::forward<Range>(range));
    }

    template <UnderlyingRange<VertexArray> Range>
    void destroyVertexArrays(Range&& range) {
        BuffersManagement::BuffersDestroyer<VertexArray,
            details::destroyArrays, Range>
                {}(std::forward<Range>(range));
    }

    template <UnderlyingRange<ElementArrayBuffer> Range>
    void destroyElementArrayBuffers(Range&& range) {
        BuffersManagement::BuffersDestroyer<ElementArrayBuffer,
            details::destroyBuffers, Range>{}(
                std::forward<Range>(range));
    }

    template <UnderlyingRange<FrameBuffer> Range>
    void destroyFrameBuffers(Range&& range) {
        BuffersManagement::BuffersDestroyer<FrameBuffer, nullptr,
            Range>{}(std::forward<Range>(range));
    }

    template <UnderlyingRange<TextureBuffer> Range>
    void destroyTextureBuffers(Range&& range) {
        BuffersManagement::BuffersDestroyer<TextureBuffer,
            details::destroyTextures, Range>{}(
                std::forward<Range>(range));
    }

}
