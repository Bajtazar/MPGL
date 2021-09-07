#include "FontRasterizer.hpp"

#include <iostream>
#include <math.h>

namespace ge {

    FontRasterizer::FontRasterizer(FontData const& mainData,
        GlyphData const& glyph)
    : mainData{mainData}, glyph{glyph}
    {
        if (glyph.glyph.isSimple())
            separateSimpleContours();
        else {} // later
    }

    void FontRasterizer::separateSimpleContours(void) {
        auto const& points = glyph.glyph.getSimpleGlyph().points;
        auto const& ends = glyph.glyph.getSimpleGlyph().endPtsOfContours;
        Contour contour;
        uint8_t counter = 0;
        for (uint16_t i = 0; i != points.size(); ++i) {
            contour.push_back(points[i]);
            if (ends[counter] == i) {
                contours.emplace_back(std::move(contour));
                contour = Contour{};
                ++counter;
            }
        }
    }

    Image FontRasterizer::rasterize(std::size_t size) noexcept {
        auto canva = prepareCanva(size);
        for (auto const& contour : contours)
            drawContourAndSetFlags(contour, canva, size);
        return canva;
    }

    Image FontRasterizer::prepareCanva(std::size_t size) const noexcept {
        auto dimmensions = remapPoint(glyph.glyph.getMaxDimmensions(), size);
        return Image{vectorCast<std::size_t>(ceil(dimmensions + 1.f))};
    }

    Vector2f FontRasterizer::remapPoint(Vector2si const& position
        , std::size_t size) const noexcept
    {
        auto translated = position - glyph.glyph.getMinDimmensions();
        return float(size) * vectorCast<float>(translated)
            / float(mainData.unitsPerEm);
    }

    void FontRasterizer::drawContourAndSetFlags(Contour const& contour,
            Image& canva, std::size_t size) noexcept
    {
        auto firstIter = contour.begin(), secondIter = firstIter + 1;
        for (; secondIter != contour.end(); ++firstIter, ++secondIter) {
            drawLine(canva, remapPoint(firstIter->position, size),
                remapPoint(secondIter->position, size));
        }
        drawLine(canva, remapPoint(firstIter->position, size),
            remapPoint(contour.front().position, size));
    }

    void FontRasterizer::drawLine(Image& canva, Vector2f firstVertex,
        Vector2f secondVertex) noexcept
    {
        if (std::abs(firstVertex[0] - secondVertex[0]) >
                std::abs(firstVertex[1] - secondVertex[1]))
            drawLineByAxis<0>(canva, firstVertex, secondVertex);
        else
            drawLineByAxis<1>(canva, firstVertex, secondVertex);
    }

}
