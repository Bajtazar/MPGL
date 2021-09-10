#include "FontRasterizer.hpp"

#include <math.h>

namespace ge {

    FontRasterizer::FontRasterizer(FontData const& mainData,
        GlyphData const& glyph, std::size_t size) : mainData{mainData}, glyph{glyph},
        lastPosition{0, 0}, pixelSetter{&FontRasterizer::deducingPixelSetter},
        size{size}, ascending{false}
    {
        primitiveQueue.reserve(3);
        separateContours(glyph);
    }

    void FontRasterizer::separateContours(GlyphData const& glyph) {
        auto const& points = glyph.glyph.getGlyph().points;
        auto const& ends = glyph.glyph.getGlyph().endPtsOfContours;
        Contour contour;
        for (uint16_t i = 0, counter = 0; i != points.size(); ++i) {
            addPoint(contour, points[i]);
            if (ends[counter] == i) {
                contours.emplace_back(std::move(contour));
                contour = Contour{};
                ++counter;
            }
        }
    }

    void FontRasterizer::addPoint(Contour& contour,
       GlyphPoint const& point)
    {
        if (!point.onCurve)
            if (!contour.back().onCurve)
                contour.emplace_back(
                        (contour.back().position + remapPoint(point.position, size)) / 2.f, true);
        contour.emplace_back(remapPoint(point.position, size), point.onCurve);
    }

    Bitmap FontRasterizer::operator() (void) noexcept {
        auto canva = prepareCanva(size);
        for (auto const& contour : contours) {
            lastPosition = vectorCast<uint16_t>(round(contour.front().position));
            drawContourAndSetFlags(contour, canva, size);
        }
        fillContour(canva);
        return canva;
    }

    Bitmap FontRasterizer::prepareCanva(std::size_t size) const noexcept {
        auto dimmensions = remapPoint(glyph.glyph.getMaxDimmensions(), size);
        return Bitmap{vectorCast<std::size_t>(ceil(dimmensions + 1.f))};
    }

    Vector2f FontRasterizer::remapPoint(Vector2si const& position
        , std::size_t size) const noexcept
    {
        auto translated = position - glyph.glyph.getMinDimmensions();
        return float(size) * vectorCast<float>(translated)
            / float(mainData.unitsPerEm);
    }

    void FontRasterizer::drawContourAndSetFlags(Contour const& contour,
            Bitmap& canva, std::size_t size) noexcept
    {
        pixelSetter = &FontRasterizer::deducingPixelSetter; //deducingPixelSetter;
        for (auto const& point : contour)
            drawPrimitive(canva, point, size);
        drawPrimitive(canva, contour.front(), size);
        primitiveQueue.clear();
    }

    void FontRasterizer::drawPrimitive(Bitmap& canva, Point const& point,
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

    void FontRasterizer::drawLine(Bitmap& canva, std::size_t size) noexcept
    {
        auto const& firstVertex = primitiveQueue[0].position;
        auto const& secondVertex = primitiveQueue[1].position;
        if (std::abs(firstVertex[0] - secondVertex[0]) >
                std::abs(firstVertex[1] - secondVertex[1]))
            drawLineByAxis<0>(canva, firstVertex, secondVertex);
        else
            drawLineByAxis<1>(canva, firstVertex, secondVertex);
        clearQueue();
    }

    void FontRasterizer::drawBezierCurve(Bitmap& canva, std::size_t size) noexcept {
        auto const& firstVertex = primitiveQueue[0].position;
        auto const& secondVertex = primitiveQueue[1].position;
        auto const& thridVertex = primitiveQueue[2].position;
        auto samples = getBezierSamples(firstVertex, secondVertex, thridVertex);
        for (float t = 0, step = 1.f / samples; t < 1.f; t += step) {
            auto result = (1.f - t) * ((1.f - t) * firstVertex + t * secondVertex)
                + t * ((1.f - t) * secondVertex + t * thridVertex);
            setCanvaPixel<0>(canva, std::round(result[0]), result[1]);
        }
        setCanvaPixel<0>(canva, std::round(thridVertex[0]), thridVertex[1]);
        clearQueue();
    }

    std::size_t FontRasterizer::getBezierSamples(Vector2f const& firstVertex,
        Vector2f const& secondVertex, Vector2f const& thirdVertex) const noexcept
    {
        auto line = (secondVertex - firstVertex).length();
        line += (thirdVertex - secondVertex).length();
        return static_cast<std::size_t>(1.41f * std::ceil(line));
    }

    void FontRasterizer::defaultPixelSetter(Bitmap& canva,
        TwoVector<uint16_t> const& position) noexcept
    {
        if (position[1] == lastPosition[1]) {
            if (!canva[position])
                canva[position] = Hit;
        } else {
            if (ascending)
                correctWhenTrue(canva, lastPosition, position, std::less<>{});
            else
                correctWhenTrue(canva, lastPosition, position, std::greater<>{});
            canva[position] = canva[position] == Flag ? Hit : Flag;
        }
    }

    void FontRasterizer::deducingPixelSetter(Bitmap& canva,
        TwoVector<uint16_t> const& position) noexcept
    {
        if (position[1] == lastPosition[1]) {
            if (!canva[position])
                canva[position] = Hit;
        } else {
            ascending = lastPosition[1] < position[1];
            canva[position] = canva[position] == Flag ? Hit : Flag;
            pixelSetter = &FontRasterizer::defaultPixelSetter;
        }
    }

    void FontRasterizer::fillContour(Bitmap& canva) const noexcept {
        for (std::size_t i = 0; i != canva.getHeight(); ++i) {
            auto row = canva[i];
            if (auto pixels = rasterizeLine(row))
                std::copy(pixels->begin(), pixels->end(), row.begin());
            else {  // slow but i have no time to fix this
                tryRepairBrokenPixel(row, i);
                if (auto pixels = rasterizeLine(row))
                    std::copy(pixels->begin(), pixels->end(), row.begin());
            }
        }
    }

    void FontRasterizer::tryRepairBrokenPixel(BitmapRow& row,
        std::size_t index) const noexcept
    {
        TwoVector<uint16_t> brokenPixel;
        for (auto const& contour : contours) {
            if (std::round(contour.front().position[1]) == index) {
                uint16_t brokenPixel = std::round(contour.front().position[0]);
                row[brokenPixel] ^= Mask;
                break;
            }
        }
    }

    FontRasterizer::CanvaRow FontRasterizer::rasterizeLine(
        BitmapRow& row) const noexcept
    {
        PixelRow pixelRow;
        pixelRow.resize(row.size(), 0);
        bool flag = false;
        for (std::size_t i = 0; i != row.size(); ++i) {
            if (row[i] == Flag) {
                flag = !flag;
                pixelRow[i] = Hit;
            } else if (flag)
                pixelRow[i] = Hit;
        }
        if (flag) return {};
        return {pixelRow};
    }

}
