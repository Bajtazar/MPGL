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
#include "Angular.hpp"
#include "../../Mathematics/Systems.hpp"
#include "../Context/Buffers/BindGuard.hpp"

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

    void Angular::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        for (auto& vertexPosition : vertices | views::position) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * vectorCast<float32>(
                oldDimensions) / vectorCast<float32>(
                    context.windowDimensions) - 1.f;
        }
        isModified = true;
    }

    void Angular::translate(Vector2f const& shift) noexcept {
        for (auto& vertexPosition : vertices | views::position)
            vertexPosition = static_cast<Vector2f>(
                vertexPosition) + shift;
        isModified = true;
    }

    void Angular::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        rotate(center, rotationMatrix<float32>(angle));
    }

    void Angular::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        for (auto& vertexPosition : vertices | views::position) {
            Vector2f position = vertexPosition;
            Vector2f radius = position - center;
            vertexPosition = rotation * radius + center;
        }
        isModified = true;
    }

    void Angular::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        for (auto& vertexPos : vertices | views::position)
            vertexPos = (static_cast<Vector2f>(vertexPos) - center
                ) * factor + center;
        isModified = true;
    }

}
