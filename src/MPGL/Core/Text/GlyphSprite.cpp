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

    template <Dimension Dim>
    GlyphSprite<Dim>::Vertices
        GlyphSprite<Dim>::makeVertices(
            Color const& color,
            Positions const& positions)
    {
        return {
            Vertex{positions[0], Vector2f{0.f, 0.f}, color},
            Vertex{positions[1], Vector2f{0.f, 1.f}, color},
            Vertex{positions[2], Vector2f{1.f, 1.f}, color},
            Vertex{positions[3], Vector2f{1.f, 0.f}, color}
        };
    }

    template <Dimension Dim>
    void GlyphSprite<Dim>::initializeBuffers(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{this->vertexArray};
        BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
        this->vertexBuffer.setBufferData(vertices);
        this->vertexArray.setArrayData(VertexArray::VertexTag<Vertex>{});
    }

    template <Dimension Dim>
    GlyphSprite<Dim>::GlyphSprite(
        Texture const& texture,
        Color const& color)
            : Texturable<Dim>{texture},
                vertices{makeVertices(color,
                    {Vector{}, Vector{}, Vector{}, Vector{}})}
    {
        initializeBuffers();
    }

    template <Dimension Dim>
    GlyphSprite<Dim>::GlyphSprite(
        Texture const& texture,
        Vector const& firstVertex,
        Vector const& secondVertex,
        Vector const& thirdVertex,
        Color const& color)
            : Texturable<Dim>{texture}, vertices{makeVertices(color, {
                firstVertex,
                secondVertex,
                thirdVertex,
                thirdVertex - secondVertex + firstVertex
            })}
    {
        initializeBuffers();
    }

    template <Dimension Dim>
    GlyphSprite<Dim>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimensions,
        Color const& color) requires TwoDimensional<Dim>
            : Texturable<Dim>{texture}, vertices{makeVertices(color, {
                firstVertex,
                firstVertex + Vector2f{0.f, dimensions[1]},
                firstVertex + dimensions,
                firstVertex + Vector2f{dimensions[0], 0.f}
            })}
    {
        initializeBuffers();
    }

    template <Dimension Dim>
    GlyphSprite<Dim>::GlyphSprite(GlyphSprite const& sprite)
        : Texturable<Dim>{sprite}, vertices{sprite.vertices}
    {
        initializeBuffers();
    }

    template <Dimension Dim>
    GlyphSprite<Dim>& GlyphSprite<Dim>::operator=(
        GlyphSprite const& texturable)
    {
        this->texture = texturable.texture;
        this->isModified = true;
        vertices.clear();
        vertices.reserve(texturable.vertices.size());
        std::ranges::copy(texturable.vertices,
            std::back_inserter(vertices));
        return *this;
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::iterator
        GlyphSprite<Dim>::begin(void) noexcept
    {
        return iterator{vertices.begin(), this->isModified};
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::iterator
        GlyphSprite<Dim>::end(void) noexcept
    {
        return iterator{vertices.end(), this->isModified};
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_iterator
        GlyphSprite<Dim>::begin(void) const noexcept
    {
        return vertices.begin();
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_iterator
        GlyphSprite<Dim>::end(void) const noexcept
    {
        return vertices.end();
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_iterator
        GlyphSprite<Dim>::cbegin(void) const noexcept
    {
        return vertices.cbegin();
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_iterator
        GlyphSprite<Dim>::cend(void) const noexcept
    {
        return vertices.cend();
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::reverse_iterator
        GlyphSprite<Dim>::rbegin(void) noexcept
    {
        return reverse_iterator{vertices.rbegin(), this->isModified};
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::reverse_iterator
        GlyphSprite<Dim>::rend(void) noexcept
    {
        return reverse_iterator{vertices.rend(), this->isModified};
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_reverse_iterator
        GlyphSprite<Dim>::rbegin(void) const noexcept
    {
        return vertices.rbegin();
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_reverse_iterator
        GlyphSprite<Dim>::rend(void) const noexcept
    {
        return vertices.rend();
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_reverse_iterator
        GlyphSprite<Dim>::crbegin(void) const noexcept
    {
        return vertices.crbegin();
    }

    template <Dimension Dim>
    [[nodiscard]] GlyphSprite<Dim>::const_reverse_iterator
        GlyphSprite<Dim>::crend(void) const noexcept
    {
        return vertices.crend();
    }

    template <Dimension Dim>
    void GlyphSprite<Dim>::setColor(Color const& color) noexcept {
        std::ranges::for_each(this->vertices | views::color,
            [&color](auto& vertexColor){ vertexColor = color; });
        this->isModified = true;
    }

    template <Dimension Dim>
    void GlyphSprite<Dim>::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        auto const& textureBuffer = this->texture.getTextureBuffer();
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        BindGuard vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

    template <Dimension Dim>
    void GlyphSprite<Dim>::actualizeBufferBeforeDraw(
        void) const noexcept
    {
        if (this->isModified) {
            {
                BindGuard<VertexBuffer> vboGuard{this->vertexBuffer};
                this->vertexBuffer.changeBufferData(vertices);
            }
            this->isModified = false;
        }
    }

    template <Dimension Dim>
    void GlyphSprite<Dim>::transform(
        Transformation<Dim> const& transformator) noexcept
    {
        any::InputRange<Adapter> positions{vertices | views::position};
        transformator(positions);
        this->isModified = true;
    }

}
