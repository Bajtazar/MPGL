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
#include "GlyphSprite.hpp"

#include "../Figures/Views.hpp"
#include "../Context/Buffers/BindGuard.hpp"

namespace mpgl {

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Color const& color) requires (IsColorable)
            : Texturable<IsColorable>{texture, color} {}

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture)
            : Texturable<IsColorable>{texture} {}

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex)
            : Texturable<IsColorable>{{
                    firstVertex,
                    secondVertex,
                    thirdVertex + secondVertex - firstVertex,
                    thirdVertex
                }, texture} {}

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color) requires (IsColorable)
            : Texturable<IsColorable>{{
                    firstVertex,
                    secondVertex,
                    thirdVertex + secondVertex - firstVertex,
                    thirdVertex
                }, texture, color} {}

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimensions)
            : Texturable<IsColorable>{{
                firstVertex,
                firstVertex + Vector2f{0.f, dimensions[1]},
                firstVertex + dimensions,
                firstVertex + Vector2f{dimensions[0], 0.f}
            }, texture} {}

    template <bool IsColorable>
    GlyphSprite<IsColorable>::GlyphSprite(
        Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimensions,
        Color const& color) requires (IsColorable)
            : Texturable<IsColorable>{{
                firstVertex,
                firstVertex + Vector2f{0.f, dimensions[1]},
                firstVertex + dimensions,
                firstVertex + Vector2f{dimensions[0], 0.f}
            }, texture, color} {}

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

}
