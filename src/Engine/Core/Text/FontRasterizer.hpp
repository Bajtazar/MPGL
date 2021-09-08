#pragma once

#include "../../Collections/Image.hpp"
#include "FontComponents.hpp"

#include <array>

namespace ge {

    class FontRasterizer {
    public:
        explicit FontRasterizer(
            FontData const& mainData,
            GlyphData const& glyph);

        Image rasterize(std::size_t size) noexcept;
    private:
        typedef VectorizedGlyph::Point                  Point;
        typedef std::vector<Point>                      Contour;
        typedef std::vector<Contour>                    Contours;

        Contours                    contours;
        Contour                     primitiveQueue;
        FontData const&             mainData;
        GlyphData const&            glyph;

        void separateContours(GlyphData const& glyph);

        Image prepareCanva(std::size_t size) const noexcept;

        Vector2f remapPoint(Vector2si const& position,
            std::size_t size) const noexcept;

        void drawContourAndSetFlags(Contour const& contour,
            Image& canva, std::size_t size) noexcept;

        void drawPrimitive(Image& canva, Point const& point,
            std::size_t size) noexcept;

        void drawLine(Image& canva, std::size_t size) noexcept;

        void drawBezierCurve(Image& canva, std::size_t size) noexcept;
        /// Quadratic Bezier Curve

        std::size_t getBezierSamples(Vector2f const& firstVertex,
            Vector2f const& secondVertex, Vector2f const& thirdVertex) const noexcept;

        void clearQueue(void) noexcept;

        template <bool Axis>
        void drawLineByAxis(Image& canva, Vector2f firstVertex,
            Vector2f secondVertex) noexcept;

        template <bool Axis>
        void setCanvaPixel(Image& canva, std::size_t x, float y) noexcept;
    };

    template <bool Axis>
    void FontRasterizer::drawLineByAxis(Image& canva, Vector2f firstVertex,
            Vector2f secondVertex) noexcept
    {
        if (firstVertex[Axis] > secondVertex[Axis])
            std::swap(firstVertex, secondVertex);
        float derivative = (secondVertex[!Axis] - firstVertex[!Axis])
            / (secondVertex[Axis] - firstVertex[Axis]);
        for (std::size_t x = std::round(firstVertex[Axis]);
            x != std::round(secondVertex[Axis] + 1); ++x) {

            float y = x * derivative + firstVertex[!Axis] - firstVertex[Axis] * derivative;
            setCanvaPixel<Axis>(canva, x, y);
        }
    }

    template <bool Axis>
    void FontRasterizer::setCanvaPixel(Image& canva, std::size_t x, float y) noexcept {
        if constexpr (Axis)
            canva[x][std::round(y)].red = 0xFF;
        else
            canva[std::round(y)][x].red = 0xFF;
    }

    template void FontRasterizer::setCanvaPixel<true>(Image&, std::size_t, float);
    template void FontRasterizer::setCanvaPixel<false>(Image&, std::size_t, float);

}
