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

#include <MPGL/Core/Text/GlyphSprite.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <bool IsColorable>
    GlyphSprite<IsColorable>::Vertices
        GlyphSprite<IsColorable>::makeVertices(
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
    GlyphSprite<IsColorable>::Vertices
        GlyphSprite<IsColorable>::makeVertices(
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
    void GlyphSprite<IsColorable>::initializeBuffers(
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
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Color const& color) requires (IsColorable)
            : Texturable{texture},
                vertices{makeVertices(color)}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture)
            : Texturable{texture}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex)
            : Texturable{texture}, vertices{makeVertices({
                firstVertex,
                secondVertex,
                thirdVertex,
                thirdVertex - secondVertex + firstVertex
            })}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color) requires (IsColorable)
            : Texturable{texture}, vertices{makeVertices(color, {
                firstVertex,
                secondVertex,
                thirdVertex,
                thirdVertex - secondVertex + firstVertex
            })}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimensions)
            : Texturable{texture}, vertices{makeVertices({
                firstVertex,
                firstVertex + Vector2f{0.f, dimensions[1]},
                firstVertex + dimensions,
                firstVertex + Vector2f{dimensions[0], 0.f}
            })}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimensions,
        Color const& color) requires (IsColorable)
            : Texturable{texture}, vertices{makeVertices(color, {
                firstVertex,
                firstVertex + Vector2f{0.f, dimensions[1]},
                firstVertex + dimensions,
                firstVertex + Vector2f{dimensions[0], 0.f}
            })}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(GlyphSprite const& sprite)
        : Texturable{sprite}, vertices{sprite.vertices}
    {
        initializeBuffers();
    }

    template <bool IsColorable>
    void GlyphSprite<IsColorable>::setColor(
        Color const& color) noexcept requires (IsColorable)
    {
        std::ranges::for_each(this->vertices | views::color,
            [&color](auto& vertexColor){ vertexColor = color; });
        this->isModified = true;
    }

    template <bool IsColorable>
    void GlyphSprite<IsColorable>::draw(void) const noexcept {
        this->actualizeBufferBeforeDraw();
        auto const& textureBuffer = this->texture.getTextureBuffer();
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        BindGuard vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

    template <bool IsColorable>
    GlyphSprite<IsColorable>& GlyphSprite<IsColorable>::operator=(
        GlyphSprite const& texturable)
    {
        texture = texturable.texture;
        isModified = true;
        vertices.clear();
        vertices.reserve(texturable.vertices.size());
        std::ranges::copy(texturable.vertices, std::back_inserter(vertices));
        return *this;
    }

    template <bool IsColorable>
    void GlyphSprite<IsColorable>::actualizeBufferBeforeDraw(
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

    template <bool IsColorable>
    void GlyphSprite<IsColorable>::transform(
        Transformation2D const& transformator) noexcept
    {
        any::InputRange<Adapter2D> positions{
            vertices | views::position};
        transformator(positions);
        isModified = true;
    }

}
