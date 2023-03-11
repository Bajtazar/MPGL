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
#pragma once

namespace mpgl {

    template <bool Axis>
    auto FontRasterizer::getLinearFunction(Bitmap& canva,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex) noexcept
    {
        float32 derivative = (secondVertex[!Axis]
            - firstVertex[!Axis])
            / (secondVertex[Axis] - firstVertex[Axis]);
        return [&, derivative] (uint16 x) {
            float32 y = x * derivative + firstVertex[!Axis]
                - firstVertex[Axis] * derivative;
            setCanvaPixel<Axis>(canva, x, y);
        };
    }

    template <bool Axis>
    void FontRasterizer::drawLineByAxis(Bitmap& canva,
        Vector2f const& firstVertex,
        Vector2f const& secondVertex) noexcept
    {
        auto func = getLinearFunction<Axis>(canva,
            firstVertex, secondVertex);
        float32 begin = firstVertex[Axis], end =
            secondVertex[Axis];
        if (begin > end)
            negativeContour(begin, end, func);
        else
            positiveContour(begin, end, func);
    }

    template <std::invocable<uint16> Function>
    void FontRasterizer::positiveContour(float32 begin,
        float32 end, Function function) noexcept
    {
        for (uint16 x = std::round(begin);
            x != std::round(end) + 1; ++x)
                function(x);
    }

    template <std::invocable<uint16> Function>
    void FontRasterizer::negativeContour(float32 begin,
        float32 end, Function function) noexcept
    {
        for (int32 x = std::round(begin);
            x != int32(std::round(end)) - 1; --x)
                function(static_cast<uint16>(x));
    }


    template <bool Axis>
    Vector2<uint16> FontRasterizer::getCoords(
        uint16 x, float32 y) const noexcept
    {
        if constexpr (Axis)
            return {static_cast<uint16>(std::round(y)), x};
        else
            return {x, static_cast<uint16>(std::round(y))};
    }

    template <std::relation<uint16, uint16> Compare>
    void FontRasterizer::correctWhenTrue(Bitmap& canva,
        Vector2<uint16> const& lastPosition,
        Vector2<uint16> const& position,
        Compare compare) noexcept
    {
        if (compare(position, lastPosition)) {
            canva[lastPosition] ^= Mask;
            ascending = !ascending;
        }
    }

    template <bool Axis>
    void FontRasterizer::setCanvaPixel(Bitmap& canva,
        uint16 x, float32 y) noexcept
    {
        auto position = getCoords<Axis>(x, y);
        (this->*pixelSetter)(canva, position);
        lastPosition = position;
    }

}
