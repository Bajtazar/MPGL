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
#include "Views.hpp"
#include "ResizableAngular.hpp"
#include "../../Utility/Ranges.hpp"
#include "../Context/Buffers/BindGuard.hpp"

namespace mpgl {

    ResizableAngular::ResizableAngular(size_t size,
        Color const& color) : Angular{size, color} {}

    ResizableAngular::ResizableAngular(Vertices vertices)
        : Angular{vertices} {}

    ResizableAngular& ResizableAngular::operator= (
        ResizableAngular const& shape)
    {
        Angular::operator=(shape);
        isExtended = true;
        return *this;
    }

    ResizableAngular::OptionalVec2f
        ResizableAngular::getCenter(void) const noexcept
    {
        if (!vertices.size())
            return {};
        return { accumulate(*this | views::position, Vector2f{},
            [](auto const& x){ return std::forward<Vector2f>(x); })
                / static_cast<float32>(vertices.size()) };
    }

    void ResizableAngular::resize(size_type size) {
        vertices.resize(size, Vertex{Vector2f{}, Color{}});
        isExtended = true;
    }

    void ResizableAngular::reserve(size_type size) {
        vertices.reserve(size);
    }

    void ResizableAngular::push(Vertex vertex) {
        vertices.push_back(std::move(vertex));
        isExtended = true;
    }

    void ResizableAngular::emplace(Vector2f const& position,
        Color const& color)
    {
        vertices.emplace_back(position, color);
        isExtended = true;
    }

    void ResizableAngular::pop(void) noexcept {
        vertices.pop_back();
        isExtended = true;
    }

    void ResizableAngular::actualizeBufferBeforeDraw(
        void) const noexcept
    {
        if (isExtended) {
            {
                BindGuard<VertexBuffer> vboGuard{vertexBuffer};
                vertexBuffer.setBufferData(vertices);
            }
            isExtended = isModified = false;
        } else
            Angular::actualizeBufferBeforeDraw();
    }

}
