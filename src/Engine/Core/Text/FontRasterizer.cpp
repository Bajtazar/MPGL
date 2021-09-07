#include "FontRasterizer.hpp"

#include <iostream>
#include <math.h>

namespace ge {

    FontRasterizer::FontRasterizer(FontData const& mainData,
        GlyphData const& glyph) : mainData{mainData}, glyph{glyph}
    {
        primitiveQueue.reserve(3);
        if (glyph.glyph.isSimple())
            separateSimpleContours(glyph);
        else {}
    }

    void FontRasterizer::separateSimpleContours(GlyphData const& glyph) {
        auto const& points = glyph.glyph.getSimpleGlyph().points;
        auto const& ends = glyph.glyph.getSimpleGlyph().endPtsOfContours;
        Contour contour;
        for (uint16_t i = 0, counter = 0; i != points.size(); ++i) {
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
        for (auto const& point : contour)
            drawPrimitive(canva, point, size);
        drawPrimitive(canva, contour.front(), size);
        primitiveQueue.clear();
    }

    void FontRasterizer::drawPrimitive(Image& canva, Point const& point,
        std::size_t size) noexcept
    {
        primitiveQueue.push_back(point);
        if (primitiveQueue.size() == 2) {
            if (!primitiveQueue.back().onCurve)
                return;
            drawLine(canva, size);
        } else if (primitiveQueue.size() == 3)
            drawBezierCurve(canva, size);
    }

    void FontRasterizer::clearQueue(void) noexcept {
        primitiveQueue.erase(primitiveQueue.begin(), primitiveQueue.end() - 1);
    }

    void FontRasterizer::drawLine(Image& canva, std::size_t size) noexcept
    {
        auto firstVertex = remapPoint(primitiveQueue[0].position, size);
        auto secondVertex = remapPoint(primitiveQueue[1].position, size);
        if (std::abs(firstVertex[0] - secondVertex[0]) >
                std::abs(firstVertex[1] - secondVertex[1]))
            drawLineByAxis<0>(canva, firstVertex, secondVertex);
        else
            drawLineByAxis<1>(canva, firstVertex, secondVertex);
        clearQueue();
    }

    void FontRasterizer::drawBezierCurve(Image& canva, std::size_t size) noexcept {
        auto firstVertex = remapPoint(primitiveQueue[0].position, size);
        auto secondVertex = remapPoint(primitiveQueue[1].position, size);
        auto thridVertex = remapPoint(primitiveQueue[2].position, size);
        auto samples = getBezierSamples(firstVertex, secondVertex, thridVertex);
        for (float t = 0, step = 1.f / samples; t < 1.f; t += step) {
            auto result = (1.f - t) * ((1.f - t) * firstVertex + t * secondVertex)
                + t * ((1.f - t) * secondVertex + t * thridVertex);
            setCanvaPixel<0>(canva, std::size_t(std::round(result[0])), result[1]);
        }
        setCanvaPixel<0>(canva, std::size_t(std::round(thridVertex[0])), thridVertex[1]);
        clearQueue();
    }

    std::size_t FontRasterizer::getBezierSamples(Vector2f const& firstVertex,
        Vector2f const& secondVertex, Vector2f const& thirdVertex) const noexcept
    {
        auto line = (secondVertex - firstVertex).length();
        line += (thirdVertex - secondVertex).length();
        return 2 * static_cast<std::size_t>(std::ceil(line));
    }

}
