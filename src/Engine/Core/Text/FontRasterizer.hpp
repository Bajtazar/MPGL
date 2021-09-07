#pragma once

#include "../../ImageLoading/Image.hpp"
#include "FontComponents.hpp"

namespace ge {

    class FontRasterizer {
    public:
        explicit FontRasterizer(
            FontData const& mainData,
            GlyphData const& glyph);

        Image rasterize(std::size_t size) noexcept;
    private:
        typedef std::vector<VectorizedGlyph::Point>     Contour;
        typedef std::vector<Contour>                    Contours;

        Contours                    contours;
        FontData const&             mainData;
        GlyphData const&            glyph;

        void separateSimpleContours(void);

        Image prepareCanva(std::size_t size) const noexcept;

        Vector2f remapPoint(Vector2si const& position,
            std::size_t size) const noexcept;

        void drawContourAndSetFlags(Contour const& contour,
            Image& canva, std::size_t size) noexcept;

        void drawLine(Image& canva, Vector2f firstVertex,
            Vector2f secondVertex) noexcept;

        template <bool Axis>
        void drawLineByAxis(Image& canva, Vector2f firstVertex,
            Vector2f secondVertex) noexcept;

        template <bool Axis>
        void setCanvaPixel(Image& canva, std::size_t x, float y) const noexcept;
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
            x != std::round(secondVertex[Axis]); ++x) {

            float y = x * derivative + firstVertex[!Axis] - firstVertex[Axis] * derivative;
            setCanvaPixel<Axis>(canva, x, y);
        }
    }

    template <bool Axis>
    void FontRasterizer::setCanvaPixel(Image& canva, std::size_t x, float y) const noexcept {
        if constexpr (Axis)
            canva[x][std::round(y)].red = 0xFF;
        else
            canva[std::round(y)][x].red = 0xFF;
    }

}
