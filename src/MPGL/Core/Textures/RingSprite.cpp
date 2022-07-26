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
#include <MPGL/Utility/Deferred/DeferredExecutionWrapper.hpp>
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
        Layout& layout,
        Vector2u const& oldDimensions) noexcept
    {
        any::InputRange<Adapter2D> positions{vertices};
        layout(positions, oldDimensions);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::InnerEllipse::transform(
        Transformation2D const& transformator) noexcept
    {
        any::InputRange<Adapter2D> positions{vertices};
        transformator(positions);
        actualizeMatrices();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setLocations(void) {
        Shadeable::setLocations(DeferredExecutionWrapper{this->shaderProgram,
            locations}([](auto program, auto locations)
            {
                locations->outerShift
                    = ShaderLocation{*program, "outerShift"};
                locations->innerShift
                    = ShaderLocation{*program, "innerShift"};
                locations->outerTransform
                    = ShaderLocation{*program, "outerTransform"};
                locations->innerTransform
                    = ShaderLocation{*program, "innerTransform"};
            }
        ));
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
                texture, shaderName()}, locations{new Locations},
                innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations();
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
                texture, shaderName(), color}, locations{new Locations},
                    innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations();
    }

    template <bool IsColorable>
    RingSprite<IsColorable>::RingSprite(
        Texture const& texture,
        Vector2f const& center,
        float32 radius,
        InnerEllipse const& innerEllipse)
            : EllipticSprite<IsColorable>{
                this->circlePositions(center, radius),
                texture, shaderName()}, locations{new Locations},
                    innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations();
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
                texture, shaderName(), color}, locations{new Locations},
                    innerEllipse{innerEllipse}
    {
        actualizeMatrices();
        setLocations();
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
        Layout& layout,
        Vector2u const& oldDimensions) noexcept
    {
        EllipticSprite<IsColorable>::onScreenTransformation(
            layout, oldDimensions);
        actualizeMatrices();
        innerEllipse.onScreenTransformation(layout, oldDimensions);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::transform(
        Transformation2D const& transformator) noexcept
    {
        EllipticSprite<IsColorable>::transform(transformator);
        actualizeMatrices();
        innerEllipse.transform(transformator);
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setShader(
        ShaderProgram const& program) noexcept
    {
        EllipticSprite<IsColorable>::setShader(program);
        setLocations();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setShader(
        ShaderProgram&& program) noexcept
    {
        EllipticSprite<IsColorable>::setShader(std::move(program));
        setLocations();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setShader(
        std::string const& name)
    {
        EllipticSprite<IsColorable>::setShader(name);
        setLocations();
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setConvolution(
        Matrix3f const& convolution)
    {
        this->setShader(IsColorable ?
            "MPGL/2D/CTRingConv" : "MPGL/2D/TRingConv");
        Shadeable::setLocations(DeferredExecutionWrapper{this->shaderProgram}(
            [](auto program, auto convolution, auto dimensions) {
                ShaderLocation{*program, "convolution"}(convolution);
                ShaderLocation{*program, "screen"}(dimensions);
            }, convolution, this->texture.getTextureDimensions()
        ));
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::resetConvolution(void) {
        this->setShader(shaderName());
    }

    template <bool IsColorable>
    void RingSprite<IsColorable>::setUniforms(void) const noexcept {
        locations->outerShift(
            Vector2f{get<"position">(this->vertices.front())});
        locations->innerShift(
            Vector2f{innerEllipse.vertices.front()});
        locations->outerTransform(outline);
        locations->innerTransform(innerEllipse.outline);
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