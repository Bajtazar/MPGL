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

#include <MPGL/Core/Textures/RingSprite.hpp>
#include <MPGL/Mathematics/Systems.hpp>

namespace mpgl {

    template <bool IsColorable>
    std::string RingSprite<IsColorable>::shaderName(void) noexcept {
        if constexpr (IsColorable)
            return "MPGL/2D/CTRing";
        else
            return "MPGL/2D/TRing";
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::InnerEllipse::Vertices
        RingSprite<IsColorable>::InnerEllipse::ellipseVertices(
            Vector2f const& center,
            Vector2f const& semiAxis,
            float32 angle)
    {
        Matrix2f rotation = rotationMatrix<float32>(angle);
        Vector2f rot1 = rotation * semiAxis;
        Vector2f rot2 = rotation * Vector2f{semiAxis[0], -semiAxis[1]};
        return {
            center - rot2,
            center + rot1,
            center + rot2,
            center - rot1
        };
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::InnerEllipse::Vertices
        RingSprite<IsColorable>::InnerEllipse::circleVertices(
            Vector2f const& center,
            float32 radius)
    {
        Vector2f semiMajor = Vector2f{radius, 0.f};
        Vector2f semiMinor = Vector2f{0.f, radius};
        return {
            center - semiMajor + semiMinor,
            center + semiMajor + semiMinor,
            center + semiMajor - semiMinor,
            center - semiMajor - semiMinor
        };
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::InnerEllipse::InnerEllipse(
        Vector2f const& center,
        Vector2f const& semiAxis,
        float32 angle)
            : vertices{ellipseVertices(center, semiAxis, angle)}
    {
        actualizeMatrices();
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::InnerEllipse::InnerEllipse(
        Vector2f const& center,
        float radius)
            : vertices{circleVertices(center, radius)}
    {
        actualizeMatrices();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::InnerEllipse::actualizeMatrices(
        void) noexcept
    {
        outline = *invert(transpose(
            Matrix2f{
                Vector2f{vertices[1]} - Vector2f{vertices[0]},
                Vector2f{vertices[3]} - Vector2f{vertices[0]}
            }
        ));
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        RingSprite<IsColorable>::InnerEllipse::getCenter(
            void) const noexcept
    {
        return (Vector2f{vertices[3]} + Vector2f{vertices[1]}) / 2.f;
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        RingSprite<IsColorable>::InnerEllipse::getSemiAxis(
            void) const noexcept
    {
        return {
            (Vector2f{vertices[1]} - Vector2f{vertices[0]}).length(),
            (Vector2f{vertices[3]} - Vector2f{vertices[0]}).length()
        };
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::InnerEllipse::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        for (auto& position : vertices | std::views::transform(
            (Vector2f&(Vertex::*)(void))&Vertex::get))
        {
            position = (position + 1.f) * static_cast<Vector2f>(
                oldDimensions) / static_cast<Vector2f>(
                    GraphicalObject::context.windowDimensions) - 1.f;
        }
        actualizeMatrices();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::InnerEllipse::translate(
        Vector2f const& shift) noexcept
    {
        for (auto& position : vertices)
            position = Vector2f(position) + shift;
        actualizeMatrices();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::InnerEllipse::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        for (auto& position : vertices)
            position = (static_cast<Vector2f>(position)
                - center) * factor + center;
        actualizeMatrices();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::InnerEllipse::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        rotate(center, rotationMatrix<float32>(angle));
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::InnerEllipse::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        for (auto& position : vertices)
            position = rotation * (
                Vector2f(position) - center) + center;
        actualizeMatrices();
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::Locations::Locations(void)
        : outerShift{new ShaderLocation},
        innerShift{new ShaderLocation},
        outerTransform{new ShaderLocation},
        innerTransform{new ShaderLocation} {}

    template <bool IsColorable>
    void RingSprite<IsColorable>::setLocations(
        std::string const& shaderName)
    {
        Shadeable::setLocations(shaderName,
            [locations=locations](ShaderProgram const& program)
        {
            *locations.outerShift
                = ShaderLocation{program, "outerShift"};
            *locations.innerShift
                = ShaderLocation{program, "innerShift"};
            *locations.outerTransform
                = ShaderLocation{program, "outerTransform"};
            *locations.innerTransform
                = ShaderLocation{program, "innerTransform"};
        });
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        Vector2f const& semiAxis,
        InnerEllipse const& innerEllipse,
        float32 angle)
            : EllipticSprite<IsColorable>{
                this->ellipsePositions(center, semiAxis, angle),
                texture, shaderName()}, innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations(shaderName());
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        Vector2f const& semiAxis,
        InnerEllipse const& innerEllipse,
        Color const& color,
        float32 angle) requires (IsColorable)
            : EllipticSprite<IsColorable>{
                this->ellipsePositions(center, semiAxis, angle),
                texture, shaderName(), color},
                    innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations(shaderName());
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        float32 radius,
        InnerEllipse const& innerEllipse)
            : EllipticSprite<IsColorable>{
                this->circlePositions(center, radius),
                texture, shaderName()},
                    innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations(shaderName());
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        float32 radius,
        InnerEllipse const& innerEllipse,
        Color const& color) requires (IsColorable)
            : EllipticSprite<IsColorable>{
                this->circlePositions(center, radius),
                texture, shaderName(), color},
                    innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations(shaderName());
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        Vector2f const& outerSemiAxis,
        Vector2f const& innerSemiAxis,
        float32 angle) : RingSprite{texture, center, outerSemiAxis,
            InnerEllipse{center, innerSemiAxis, angle}, angle} {}

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        Vector2f const& outerSemiAxis,
        Vector2f const& innerSemiAxis,
        Color const& color,
        float32 angle) requires (IsColorable)
            : RingSprite{texture, center, outerSemiAxis,
                InnerEllipse{center, innerSemiAxis, angle}, color,
                    angle} {}

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        float32 outerRadius,
        float32 innerRadius) : RingSprite{texture, center,
            outerRadius, InnerEllipse{center, innerRadius}} {}

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        float32 outerRadius,
        float32 innerRadius,
        Color const& color) requires (IsColorable)
            : RingSprite{texture, center,
                outerRadius, InnerEllipse{center, innerRadius},
                    color} {}

    template <bool IsColorable>
    void RingSprite<IsColorable>::actualizeMatrices(void) noexcept {
        outline = *invert(transpose(
            Matrix2f{Vector2f{get<"position">(this->vertices[1])}
                - Vector2f{get<"position">(this->vertices[0])},
            Vector2f{get<"position">(this->vertices[3])}
                - Vector2f{get<"position">(this->vertices[0])}}));
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        RingSprite<IsColorable>::getCenter(void) const noexcept
    {
        return (Vector2f{get<"position">(this->vertices[3])}
            + Vector2f{get<"position">(this->vertices[1])}) / 2.f;
    }

    template <bool IsColorable>
    [[nodiscard]] Vector2f
        RingSprite<IsColorable>::getOuterSemiAxis(void) const noexcept
    {
        return {
            (Vector2f{get<"position">(this->vertices[1])}
                - Vector2f{get<"position">(this->vertices[0])}).length(),
            (Vector2f{get<"position">(this->vertices[3])}
                - Vector2f{get<"position">(this->vertices[0])}).length()
        };
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        EllipticSprite<IsColorable>::onScreenTransformation(
            oldDimensions);
        actualizeMatrices();
        innerEllipse.onScreenTransformation(oldDimensions);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::translate(
        Vector2f const& shift) noexcept
    {
        EllipticSprite<IsColorable>::translate(shift);
        actualizeMatrices();
        innerEllipse.translate(shift);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::scale(
        Vector2f const& center,
        float32 factor) noexcept
    {
        EllipticSprite<IsColorable>::scale(center, factor);
        actualizeMatrices();
        innerEllipse.scale(center, factor);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::rotate(
        Vector2f const& center,
        float32 angle) noexcept
    {
        auto matrix = rotationMatrix<float32>(angle);
        EllipticSprite<IsColorable>::rotate(center, matrix);
        actualizeMatrices();
        innerEllipse.rotate(center, matrix);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::rotate(
        Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        EllipticSprite<IsColorable>::rotate(center, rotation);
        actualizeMatrices();
        innerEllipse.rotate(center, rotation);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setConvolution(
        Matrix3f const& convolution)
    {
        this->setShader(IsColorable ?
            "MPGL/2D/CTRingConv" : "MPGL/2D/TRingConv");
        setLocations(IsColorable ?
            "MPGL/2D/CTRingConv" : "MPGL/2D/TRingConv");
        ShaderLocation{*this->shaderProgram, "tex"}(0);
        ShaderLocation{*this->shaderProgram, "convolution"}(
            convolution);
        ShaderLocation{*this->shaderProgram, "screen"}(
            this->texture.getTextureDimensions());
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::resetConvolution(void) {
        this->setShader(shaderName());
        setLocations(shaderName());
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setUniforms(void) const noexcept {
        (*locations.outerShift)(
            Vector2f{get<"position">(this->vertices.front())});
        (*locations.innerShift)(
            Vector2f{innerEllipse.vertices.front()});
        (*locations.outerTransform)(outline);
        (*locations.innerTransform)(innerEllipse.outline);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::draw(void) const noexcept {
        auto const& textureBuffer = this->texture.getTextureBuffer();
        this->actualizeBufferBeforeDraw();
        this->shaderProgram->use();
        setUniforms();
        textureBuffer.activate();
        BindGuard textureGuard{textureBuffer};
        BindGuard vaoGuard{this->vertexArray};
        this->vertexArray.drawElements(
            VertexArray::DrawMode::Triangles, 6, DataType::UInt32);
    }

    template <bool IsColorable>
    [[nodiscard]] bool RingSprite<IsColorable>::insideSystem(
            Vector2f const& position,
            Vector2f const& shift,
            Matrix2f const& transform) const noexcept
    {
        Vector2f local = transform * (position - shift);
        return (local - 0.5f).length() <= 0.5;
    }

    template <bool IsColorable>
    [[nodiscard]] bool RingSprite<IsColorable>::contains(
        Vector2f const& position) const noexcept
    {
        bool outring = insideSystem(position,
            get<"position">(this->vertices.front()), this->outline);
        bool inring = insideSystem(position,
            innerEllipse.vertices.front(), innerEllipse.outline);
        return outring && (!inring);
    }

}