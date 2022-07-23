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
#include <MPGL/Mathematics/FastFunctions.hpp>

#include <MPGL/Core/Text/FontRasterizer.hpp>

#include <math.h>

namespace mpgl {

    FontRasterizer::FontRasterizer(
        FontData const& mainData,
        GlyphData const& glyph,
        size_type size)
            : lastPosition{0, 0}, pixelSetter{
                &FontRasterizer::deducingPixelSetter}, size{size},
                aaCoefficient{fast2Sqrt(
                context.windowOptions.antiAliasingSamples)},
            mainData{mainData}, glyph{glyph},
            ascending{false}
    {
        size *= aaCoefficient;
        primitiveQueue.reserve(3);
        separateContours(glyph);
    }

    void FontRasterizer::separateContours(GlyphData const& glyph) {
        auto const& points = glyph.glyph.getGlyph().points;
        auto const& ends = glyph.glyph.getGlyph().endPtsOfContours;
        Contour contour;
        for (uint16 i = 0, counter = 0; i != points.size(); ++i) {
            addPoint(contour, points[i]);
            if (ends[counter] == i) {
                contours.emplace_back(std::move(contour));
                contour = Contour{};
                ++counter;
            }
        }
    }

    void FontRasterizer::addPoint(
        Contour& contour,
       GlyphPoint const& point)
    {
        if (!point.onCurve)
            if (!contour.back().onCurve)
                contour.emplace_back(
                        (contour.back().position + remapPoint(
                            point.position)) / 2.f, true);
        contour.emplace_back(remapPoint(point.position),
            point.onCurve);
    }

    Bitmap FontRasterizer::operator() (void) noexcept {
        auto canva = prepareCanva();
        for (auto const& contour : contours) {
            lastPosition = vectorCast<uint16>(
                round(contour.front().position));
            drawContourAndSetFlags(contour, canva);
        }
        fillContour(canva);
        return performAntiAliasing(canva);
    }

    FontRasterizer::Pixel
        FontRasterizer::antiAliasePixels(
            Bitmap const& canva,
            size_type x,
            size_type y) const noexcept
    {
        x *= aaCoefficient;
        y *= aaCoefficient;
        size_type sum = 0;
        for (uint8 i = 0; i != aaCoefficient; ++i)
            for (uint j = 0; j != aaCoefficient; ++j)
                sum += canva[y + i][x + j];
        return sum / (aaCoefficient * aaCoefficient);
    }

    Bitmap FontRasterizer::performAntiAliasing(
        Bitmap& canva) const noexcept
    {
        if (aaCoefficient == 1)
            return canva;
        Bitmap antiAliased{canva.size() / size_type(aaCoefficient)};
        for (size_type y = 0; y < antiAliased.getHeight(); ++y)
            for (size_type x = 0; x < antiAliased.getWidth(); ++x)
                antiAliased[y][x] = antiAliasePixels(canva, x, y);
        return antiAliased;
    }

    Bitmap FontRasterizer::prepareCanva(void) const noexcept {
        auto dimensions = remapPoint(glyph.glyph.getMaxDimensions());
        return Bitmap{vectorCast<size_type>(ceil(dimensions + 1.f))};
    }

    Vector2f FontRasterizer::remapPoint(
        Vector2si const& position) const noexcept
    {
        auto translated = position - glyph.glyph.getMinDimensions();
        return float32(size) * vectorCast<float32>(translated)
            / float32(mainData.unitsPerEm);
    }

    void FontRasterizer::drawContourAndSetFlags(
        Contour const& contour,
        Bitmap& canva) noexcept
    {
        pixelSetter = &FontRasterizer::deducingPixelSetter;
        for (auto const& point : contour)
            drawPrimitive(canva, point);
        drawPrimitive(canva, contour.front());
        primitiveQueue.clear();
    }

    void FontRasterizer::drawPrimitive(
        Bitmap& canva,
        Point const& point) noexcept
    {
        primitiveQueue.push_back(point);
        if (primitiveQueue.size() == 2) {
            if (!primitiveQueue.back().onCurve)
                return;
            drawLine(canva);
        } else if (primitiveQueue.size() == 3)
            drawBezierCurve(canva);
    }

    void FontRasterizer::clearQueue(void) noexcept {
        primitiveQueue.erase(primitiveQueue.begin(),
            primitiveQueue.end() - 1);
    }

    void FontRasterizer::drawLine(
        Bitmap& canva) noexcept
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

    void FontRasterizer::drawBezierCurve(
        Bitmap& canva) noexcept
    {
        auto const& firstVertex = primitiveQueue[0].position;
        auto const& secondVertex = primitiveQueue[1].position;
        auto const& thridVertex = primitiveQueue[2].position;
        auto samples = getBezierSamples(firstVertex, secondVertex, thridVertex);
        for (float32 t = 0, step = 1.f / samples; t < 1.f; t += step) {
            auto result = (1.f - t) * ((1.f - t) * firstVertex + t * secondVertex)
                + t * ((1.f - t) * secondVertex + t * thridVertex);
            setCanvaPixel<0>(canva, std::round(result[0]), result[1]);
        }
        setCanvaPixel<0>(canva, std::round(thridVertex[0]), thridVertex[1]);
        clearQueue();
    }

    FontRasterizer::size_type
        FontRasterizer::getBezierSamples(
            Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Vector2f const& thirdVertex) const noexcept
    {
        auto line = (secondVertex - firstVertex).length();
        line += (thirdVertex - secondVertex).length();
        return static_cast<size_type>(1.41f * std::ceil(line));
    }

    void FontRasterizer::defaultPixelSetter(Bitmap& canva,
        Vector2<uint16> const& position) noexcept
    {
        if (position[1] == lastPosition[1]) {
            if (!canva[position])
                canva[position] = Hit;
        } else {
            if (ascending)
                correctWhenTrue(canva, lastPosition, position,
                    std::less<>{});
            else
                correctWhenTrue(canva, lastPosition, position,
                    std::greater<>{});
            canva[position] = canva[position] == Flag ? Hit : Flag;
        }
    }

    void FontRasterizer::deducingPixelSetter(
        Bitmap& canva,
        Vector2<uint16> const& position) noexcept
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
        for (size_type i = 0; i != canva.getHeight(); ++i) {
            auto& row = canva[i];
            if (auto pixels = rasterizeLine(row))
                std::copy(pixels->begin(), pixels->end(), row.begin());
            else {  // slow but i have no time to fix this
                tryRepairBrokenPixel(row, i);
                if (auto pixels = rasterizeLine(row))
                    std::copy(pixels->begin(), pixels->end(),
                        row.begin());
            }
        }
    }

    void FontRasterizer::tryRepairBrokenPixel(
        BitmapRow& row,
        size_type index) const noexcept
    {
        Vector2<uint16> brokenPixel;
        for (auto const& contour : contours) {
            if (std::round(contour.front().position[1]) == index) {
                uint16 brokenPixel = std::round(
                    contour.front().position[0]);
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
        for (size_type i = 0; i != row.size(); ++i) {
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
