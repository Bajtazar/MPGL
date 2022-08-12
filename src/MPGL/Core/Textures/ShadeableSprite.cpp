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

#include <MPGL/Core/Textures/ShadeableSprite.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::Vertices
        ShadeableSprite<IsColorable>::makeVertices(
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
    ShadeableSprite<IsColorable>::Vertices
        ShadeableSprite<IsColorable>::makeVertices(
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
    void ShadeableSprite<IsColorable>::initializeBuffers(
        void) const noexcept
    {
        BindGuard<VertexArray> vaoGuard{vertexArray};
        BindGuard<VertexBuffer> vboGuard{vertexBuffer};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
        vertexBuffer.setBufferData(vertices);
        vertexArray.setArrayData(VertexArray::VertexTag<Vertex>{});
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::transform(
        Transformation2D const& transformator) noexcept
    {
        any::InputRange<Adapter2D> positions{
            vertices | views::position};
        transformator(positions);
        isModified = true;
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Texture const& texture,
        std::string const& shaderName,
        Executable shaderInit)
            : Texturable{texture},
                Figure{shaderName, shaderInit},
                vertices{makeVertices()}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Texture const& texture,
        std::string const& shaderName,
        Executable shaderInit,
        Color const& color) requires (IsColorable)
            : Texturable{texture},
                Figure{shaderName, shaderInit},
                vertices{makeVertices(color)}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Positions positions,
        Texture const& texture,
        std::string const& shaderName,
        Executable shaderInit)
            : Texturable{texture},
                Figure{shaderName, shaderInit},
                vertices{makeVertices(positions)}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        Positions positions,
        Texture const& texture,
        std::string const& shaderName,
        Executable shaderInit,
        Color const& color) requires (IsColorable)
            : Texturable{texture},
                Figure{shaderName, shaderInit},
                vertices{makeVertices(color, positions)}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>::ShadeableSprite(
        ShadeableSprite const& sprite)
            : Texturable{sprite}, Figure{sprite},
                vertices{sprite.vertices}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>&
        ShadeableSprite<IsColorable>::operator=(
            ShadeableSprite const& sprite)
    {
        texture = sprite.texture;
        shaderProgram = sprite.shaderProgram;
        isModified = true;
        vertices.clear();
        vertices.reserve(sprite.vertices.size());
        std::ranges::copy(sprite.vertices, std::back_inserter(vertices));
        return *this;
    }

    template <bool IsColorable>
    ShadeableSprite<IsColorable>&
        ShadeableSprite<IsColorable>::operator=(
            ShadeableSprite&& sprite) noexcept
    {
        this->vertices = std::move(sprite.vertices);
        shaderProgram = std::move(sprite.shaderProgram);
        vertexArray = std::move(sprite.vertexArray);
        vertexBuffer = std::move(sprite.vertexBuffer);
        this->elementBuffer = std::move(sprite.elementBuffer);
        this->texture = std::move(sprite.texture);
        return *this;
    }

    template <bool IsColorable>
    void ShadeableSprite<IsColorable>::actualizeBufferBeforeDraw(
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
