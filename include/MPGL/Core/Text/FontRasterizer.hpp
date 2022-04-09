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

#include <MPGL/Core/Text/FontComponents.hpp>
#include <MPGL/Core/Context/Context.hpp>
#include <MPGL/Collections/Bitmap.hpp>

#include <array>

namespace mpgl {

    /**
     * Rasterizes a vectorized glyph into a bitmap object
     */
    class FontRasterizer : private GraphicalObject {
    public:
        typedef std::size_t                     size_type;

        /**
         * Construct a new Font Rasterizer object with given
         * font data, glyph data and glyph's size
         *
         * @param mainData the font's data
         * @param glyph the glyph's data
         * @param size the size of the glyph
         */
        explicit FontRasterizer(
            FontData const& mainData,
            GlyphData const& glyph,
            size_type size);

        /**
         * Rasterizes the glyph and returns a bitmap with this
         * glyph
         *
         * @return the bitmap containing rasterized glyph
         */
        Bitmap operator() (void) noexcept;
    private:
        /**
         * Represents a point on the vectorized glyph. Provides
         * information whether this point is the control point
         * of the quadratic bézier curve
         */
        struct Point {
            Vector2f                            position;
            bool                                onCurve;

            /**
             * Construct a new Point object with given position
             * and information whether the point is the control
             * point
             *
             * @param position the point's position
             * @param onCurve if point is the control point
             */
            explicit Point(
                Vector2f position,
                bool onCurve) noexcept
                    : position{position}, onCurve{onCurve} {}
        };

        typedef VectorizedGlyph::Point          GlyphPoint;
        typedef std::vector<GlyphPoint>         GlyphContour;
        typedef std::vector<Point>              Contour;
        typedef std::vector<Contour>            Contours;
        typedef std::vector<uint8>              PixelRow;
        typedef std::optional<PixelRow>         CanvaRow;
        typedef Bitmap::iterator::value_type    BitmapRow;
        typedef Vector2<uint16>                 Position;
        typedef Bitmap::pixel                   Pixel;

        typedef void(FontRasterizer::*PixelSetter)(
            Bitmap& canva, Vector2<uint16> const& position);

        Contours                                contours;
        Contour                                 primitiveQueue;
        Position                                lastPosition;
        Position                                firstConoturPixel;
        PixelSetter                             pixelSetter;
        size_type                               size;
        uint8                                   aaCoefficient;
        FontData const&                         mainData;
        GlyphData const&                        glyph;
        bool                                    ascending;

        /**
         * Separates the glyph's contrours
         *
         * @param glyph the constant reference to the glyph's data
         */
        void separateContours(GlyphData const& glyph);

        /**
         * Adds a point to the contour
         *
         * @param contour the reference to the glyph's contour
         * @param point the constant reference to the
         * contour's point
         */
        void addPoint(
            Contour& contour,
            GlyphPoint const& point);

        /**
         * Prepares a canva on which the glyph is going to be
         * drawn
         *
         * @return the glyph's bitmap
         */
        Bitmap prepareCanva(void) const noexcept;

        /**
         * Remaps the points into the local system
         *
         * @param position the constant reference to the point
         * position
         * @return the remapped point
         */
        Vector2f remapPoint(
            Vector2si const& position) const noexcept;

        /**
         * Draws the glyph's contoures and sets flags on the bitmap
         *
         * @param contour the constant reference to the
         * glyph's contoures
         * @param canva the reference to the glyph's bitmap
         */
        void drawContourAndSetFlags(
            Contour const& contour,
            Bitmap& canva) noexcept;

        /**
         * Adds the point to the queue. If there is enough points
         * then draws the primitive of a deduced type
         *
         * @param canva the reference to the glyph's bitmap
         * @param point the constant reference to the outline's
         * point
         */
        void drawPrimitive(
            Bitmap& canva,
            Point const& point) noexcept;

        /**
         * Draws a line on the glyph's bitmap
         *
         * @param canva the reference to the glyph's bitmap
         */
        void drawLine(Bitmap& canva) noexcept;

        /**
         * Draws a quadratic bézier curve on the glyph's bitmap
         *
         * @param canva the reference to the glyph's bitmap
         */
        void drawBezierCurve(Bitmap& canva) noexcept;

        /**
         * Returns the number of samples for a quadratic bézier
         * curve with given vertices
         *
         * @param firstVertex the first vertex position
         * @param secondVertex the control vertex position
         * @param thirdVertex the last vertex position
         * @return the number of samples
         */
        std::size_t getBezierSamples(
            Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Vector2f const& thirdVertex) const noexcept;

        /**
         * Clears the primitives queue
         */
        void clearQueue(void) noexcept;

        /**
         * Draws a line with respect to the given axis
         *
         * @tparam Axis the axis identifier
         * @param canva the reference to the glyph's bitmap
         * @param firstVertex the first vertex position
         * @param secondVertex the last vertex position
         */
        template <bool Axis>
        void drawLineByAxis(
            Bitmap& canva,
            Vector2f const& firstVertex,
            Vector2f const& secondVertex) noexcept;

        /**
         * Returns a line drawing function
         *
         * @tparam Axis the axis identifier
         * @param canva the reference to the glyph's bitmap
         * @param firstVertex the first vertex position
         * @param secondVertex the last vertex position
         * @return the line drawing functor
         */
        template <bool Axis>
        auto getLinearFunction(
            Bitmap& canva,
            Vector2f const& firstVertex,
            Vector2f const& secondVertex) noexcept;

        /**
         * Draws a positive contour with given functor
         *
         * @tparam Function the functor type
         * @param begin the begin of the contour
         * @param end the end of the contour
         * @param function the contour drawing functor
         */
        template <std::invocable<uint16> Function>
        void positiveContour(
            float32 begin,
            float32 end,
            Function function) noexcept;

        /**
         * Draws a negative contour with given functor
         *
         * @tparam Function the functor type
         * @param begin the begin of the contour
         * @param end the end of the contour
         * @param function the contour drawing functor
         */
        template <std::invocable<uint16> Function>
        void negativeContour(
            float32 begin,
            float32 end,
            Function function) noexcept;

        /**
         * Sets the pixel on the glyph's bitmap
         *
         * @tparam Axis the axis identifier
         * @param canva the reference to the glyph's bitmap
         * @param x the x position
         * @param y the y position
         */
        template <bool Axis>
        void setCanvaPixel(
            Bitmap& canva,
            uint16 x,
            float32 y) noexcept;

        /**
         * Returns the rounded coords with respect to the given
         * axis
         *
         * @tparam Axis the axis identifier
         * @param x the x position
         * @param y the y position
         * @return the position vector
         */
        template <bool Axis>
        Vector2<uint16> getCoords(
            uint16 x,
            float32 y) const noexcept;

        /**
         * Corrects when the comparing functor returns true
         *
         * @tparam Compare the comparing functor type
         * @param bitmap the reference to the glyph's bitmap
         * @param lastPosition the last pixel position
         * @param position the current pixel positon
         * @param compare the comparing functor
         */
        template <std::relation<uint16, uint16> Compare>
        void correctWhenTrue(
            Bitmap& bitmap,
            Vector2<uint16> const& lastPosition,
            Vector2<uint16> const& position,
            Compare compare) noexcept;

        /**
         * The default pixel setter
         *
         * @param canva the reference to the glyph's bitmap
         * @param position the pixel's position
         */
        void defaultPixelSetter(
            Bitmap& canva,
            Vector2<uint16> const& position) noexcept;

        /**
         * The deducing pixel setter [used at the begining
         * of the contour]
         *
         * @param canva the reference to the glyph's bitmap
         * @param position the pixel's position
         */
        void deducingPixelSetter(
            Bitmap& canva,
            Vector2<uint16> const& position) noexcept;

        /**
         * Fills the space between flags on the bitmap
         *
         * @param canva the reference to the glyph's bitmap
         */
        void fillContour(Bitmap& canva) const noexcept;

        /**
         * Preforms anti-aliasing on the bitmap
         *
         * @param canva the reference to the glyph's bitmap
         * @return the anti-aliased bitmap
         */
        Bitmap performAntiAliasing(
            Bitmap& canva) const noexcept;

        /**
         * Anti-aliases a pixel on the bitmap
         *
         * @param canva the reference to the glyph's bitmap
         * @param x the x position
         * @param y the y position
         * @return the anti-aliased pixel
         */
        Pixel antiAliasePixels(
            Bitmap const& canva,
            size_type x,
            size_type y) const noexcept;

        /**
         * Rasterizes a bitmap's row
         *
         * @param row the reference to the glyph's bitmap row
         * @return the rasterized line
         */
        CanvaRow rasterizeLine(BitmapRow& row) const noexcept;

        /**
         * Tries to repair broken pixels on the line
         *
         * @param row the reference to the glyph's bitmap row
         * @param index the pixel index
         */
        void tryRepairBrokenPixel(
            BitmapRow& row,
            size_type index) const noexcept;

        static constexpr const uint8            Hit  = 0xFF;
        static constexpr const uint8            Flag = 0x0F;
        static constexpr const uint8            Mask = 0xF0;
    };

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
            x != int32(std::round(end) - 1); --x)
                function(static_cast<uint16>(x));
    }


    template <bool Axis>
    Vector2<uint16> FontRasterizer::getCoords(
        uint16 x, float32 y) const noexcept
    {
        if constexpr (Axis)
            return {std::round(y), x};
        else
            return {x, std::round(y)};
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

    template void FontRasterizer::setCanvaPixel<true>(
        Bitmap&, uint16, float32);
    template void FontRasterizer::setCanvaPixel<false>(
        Bitmap&, uint16, float32);

}
