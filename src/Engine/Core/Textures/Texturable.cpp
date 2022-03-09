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
#include "Texturable.hpp"

#include "../Figures/Views.hpp"
#include "../../Mathematics/Systems.hpp"
#include "../Context/Buffers/BindGuard.hpp"

namespace mpgl {

    template <bool IsColorable>
    Texturable<IsColorable>::Vertices
        Texturable<IsColorable>::makeVertices(
            Color const& color,
            Positions const& positions
            ) requires IsColorable
    {
        return {
            Vertex{positions[0], Vector2f{0.f, 0.f}, color},
            Vertex{positions[1], Vector2f{0.f, 1.f}, color},
            Vertex{positions[2], Vector2f{1.f, 1.f}, color},
            Vertex{positions[3], Vector2f{1.f, 0.f}, color}
        };
    }

    template <bool IsColorable>
    Texturable<IsColorable>::Vertices
        Texturable<IsColorable>::makeVertices(
            Positions const& positions)
    {
        if constexpr (IsColorable)
            return {Vertex{positions[0], Vector2f{0.f, 0.f}, Color{}},
                Vertex{positions[1], Vector2f{0.f, 1.f}, Color{}},
                Vertex{positions[2], Vector2f{1.f, 1.f}, Color{}},
                Vertex{positions[3], Vector2f{1.f, 0.f}, Color{}}};
        else
            return {Vertex{positions[0], Vector2f{0.f, 0.f}},
                Vertex{positions[1], Vector2f{0.f, 1.f}},
                Vertex{positions[2], Vector2f{1.f, 1.f}},
                Vertex{positions[3], Vector2f{1.f, 0.f}}};
    }

    template <bool IsColorable>
    void Texturable<IsColorable>::initializeBuffers(
        void) const noexcept
    {
        BindGuard<VertexArray> vaoGuard{vertexArray};
        BindGuard<VertexBuffer> vboGuard{vertexBuffer};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
        vertexBuffer.setBufferData(vertices);
        vertexArray.setArrayData(vertices.front());
    }

    template <bool IsColorable>
    Texturable<IsColorable>::Texturable(
        Texture const& texture,
        Color const& color) requires (IsColorable)
            : vertices{makeVertices(color)},
                texture{texture}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    Texturable<IsColorable>::Texturable(
        Texture const& texture) : vertices{makeVertices()},
            texture{texture}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    Texturable<IsColorable>::Texturable(
        Positions vertices,
        Texture const& texture)
            : vertices{makeVertices(vertices)}, texture{texture}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    Texturable<IsColorable>::Texturable(
        Positions vertices,
        Texture const& texture,
        Color const& color) requires (IsColorable)
            : vertices{makeVertices(color, vertices)},
                texture{texture}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    Texturable<IsColorable>::Texturable(Texturable const& texturable)
        : Shape{texturable}, vertices{texturable.vertices},
            texture{texturable.texture}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    Texturable<IsColorable>&
        Texturable<IsColorable>::operator=(
            Texturable const& texturable)
    {
        texture = texturable.texture;
        isModified = true;
        vertices.clear();
        vertices.reserve(texturable.vertices.size());
        std::ranges::copy(texturable.vertices, std::back_inserter(vertices));
        return *this;
    }

    template <bool IsColorable>
    Texturable<IsColorable>&
        Texturable<IsColorable>::operator=(
            Texturable&& texturable) noexcept
    {
        vertexArray = std::move(texturable.vertexArray);
        vertexBuffer = std::move(texturable.vertexBuffer);
        elementBuffer = std::move(texturable.elementBuffer);
        isModified = std::move(texturable.isModified);
        vertices = std::move(texturable.vertices);
        texture = std::move(texturable.texture);
        return *this;
    }

    template <bool IsColorable>
    void Texturable<IsColorable>::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        for (auto& vertexPosition : vertices | views::position) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(
                oldDimensions) / static_cast<Vector2f>(
                    context.windowDimensions) - 1.f;
        }
        isModified = true;
    }

    template <bool IsColorable>
    void Texturable<IsColorable>::translate(
        Vector2f const& shift) noexcept
    {
        for (auto& position : vertices | views::position)
            position = Vector2f(position) + shift;
        isModified = true;
    }

    template <bool IsColorable>
    void Texturable<IsColorable>::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        for (auto& position : vertices | views::position)
            position = (static_cast<Vector2f>(position)
                - center) * factor + center;
        isModified = true;
    }

    template <bool IsColorable>
    void Texturable<IsColorable>::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        rotate(center, rotationMatrix<float32>(angle));
    }

    template <bool IsColorable>
    void Texturable<IsColorable>::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        for (auto& position : vertices | views::position)
            position = rotation * (
                Vector2f(position) - center) + center;
        isModified = true;
    }

    template <bool IsColorable>
    void Texturable<IsColorable>::actualizeBufferBeforeDraw(
        void) const noexcept
    {
        if (isModified) {
            {
                BindGuard<VertexBuffer> vboGuard{vertexBuffer};
                vertexBuffer.changeBufferData(vertices);
            }
            isModified = false;
        }
    }

}
