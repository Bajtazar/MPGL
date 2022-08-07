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
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Angular.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    void Angular::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{vertexArray};
        BindGuard<VertexBuffer> vboGuard{vertexBuffer};
        vertexBuffer.setBufferData(vertices);
        vertexArray.setArrayData(vertices.front());
    }

    Angular::Angular(size_t size, Color const& color)
        : Figure{"MPGL/2D/Default"}, vertices{size,
            Vertex{Vector2f{}, color}}
    {
        initializeBuffers();
    }

    Angular::Angular(Vertices vertices) : Figure{"MPGL/2D/Default"},
        vertices{std::move(vertices)}
    {
        initializeBuffers();
    }

    Angular::Angular(Angular const& shape)
        : Figure{shape}, vertices{shape.vertices}
    {
        initializeBuffers();
    }

    void Angular::actualizeBufferBeforeDraw(void) const noexcept {
        if (isModified) {
            {
                BindGuard<VertexBuffer> vboGuard{vertexBuffer};
                vertexBuffer.changeBufferData(vertices);
            }
            isModified = false;
        }
    }

    Angular& Angular::operator=(Angular const& shape) {
        Figure::operator=(shape);
        vertices.clear();
        vertices.reserve(shape.vertices.size());
        std::ranges::copy(shape.vertices, std::back_inserter(vertices));
        return *this;
    }

    void Angular::transform(
        Transformation2D const& transformator) noexcept
    {
        any::InputRange<Adapter2D> positions{
            vertices | views::position};
        transformator(positions);
        isModified = true;
    }

}
