#pragma once

#include "../../Collections/Bitmap.hpp"
#include "FontComponents.hpp"

#include <array>

namespace ge {

    class FontRasterizer {
    public:
        explicit FontRasterizer(
            FontData const& mainData,
            GlyphData const& glyph,
            std::size_t size);

        Bitmap operator() (void) noexcept;
    private:
        struct Point {
            Vector2f                position;
            bool                    onCurve;
            explicit Point(Vector2f position, bool onCurve) noexcept
                : position{position}, onCurve{onCurve} {}
        };

        typedef VectorizedGlyph::Point                  GlyphPoint;
        typedef std::vector<GlyphPoint>                 GlyphContour;
        typedef std::vector<Point>                      Contour;
        typedef std::vector<Contour>                    Contours;
        typedef std::vector<uint8>                      PixelRow;
        typedef std::optional<PixelRow>                 CanvaRow;
        typedef Bitmap::iterator::value_type            BitmapRow;

        using PixelSetter = void (FontRasterizer::*)
            (Bitmap& canva, Vector2<uint16> const& position);

        Contours                    contours;
        Contour                     primitiveQueue;
        Vector2<uint16>             lastPosition;
        Vector2<uint16>             firstConoturPixel;
        PixelSetter                 pixelSetter;
        std::size_t                 size;
        FontData const&             mainData;
        GlyphData const&            glyph;
        bool                        ascending : 1;

        void separateContours(GlyphData const& glyph);

        void addPoint(Contour& contour, GlyphPoint const& point);

        Bitmap prepareCanva(std::size_t size) const noexcept;

        Vector2f remapPoint(Vector2si const& position,
            std::size_t size) const noexcept;

        void drawContourAndSetFlags(Contour const& contour,
            Bitmap& canva, std::size_t size) noexcept;

        void drawPrimitive(Bitmap& canva, Point const& point,
            std::size_t size) noexcept;

        void drawLine(Bitmap& canva, std::size_t size) noexcept;

        void drawBezierCurve(Bitmap& canva, std::size_t size) noexcept;
        /// Quadratic Bezier Curve

        std::size_t getBezierSamples(Vector2f const& firstVertex,
            Vector2f const& secondVertex, Vector2f const& thirdVertex) const noexcept;

        void clearQueue(void) noexcept;

        template <bool Axis>
        void drawLineByAxis(Bitmap& canva, Vector2f const& firstVertex,
            Vector2f const& secondVertex) noexcept;

        template <bool Axis>
        auto getLinearFunction(Bitmap& canva, Vector2f const& firstVertex,
            Vector2f const& secondVertex) noexcept;

        template <std::invocable<uint16> Function>
        void positiveContour(float32 begin, float32 end, Function function) noexcept;

        template <std::invocable<uint16> Function>
        void negativeContour(float32 begin, float32 end, Function function) noexcept;

        template <bool Axis>
        void setCanvaPixel(Bitmap& canva, uint16 x, float32 y) noexcept;

        template <bool Axis>
        Vector2<uint16> getCoords(uint16 x, float32 y) const noexcept;

        template <std::relation<uint16, uint16> Compare>
        void correctWhenTrue(Bitmap& bitmap, Vector2<uint16> const& lastPosition,
            Vector2<uint16> const& position, Compare compare) noexcept;

        void defaultPixelSetter(Bitmap& canva,
            Vector2<uint16> const& position) noexcept;

        void deducingPixelSetter(Bitmap& canva,
            Vector2<uint16> const& position) noexcept;

        void fillContour(Bitmap& canva) const noexcept;

        CanvaRow rasterizeLine(BitmapRow& row) const noexcept;

        void tryRepairBrokenPixel(BitmapRow& row, std::size_t index) const noexcept;

        static constexpr const uint8                    Hit  = 0xFF;
        static constexpr const uint8                    Flag = 0x0F;
        static constexpr const uint8                    Mask = 0xF0;
    };

    template <bool Axis>
    auto FontRasterizer::getLinearFunction(Bitmap& canva, Vector2f const& firstVertex,
        Vector2f const& secondVertex) noexcept
    {
        float32 derivative = (secondVertex[!Axis] - firstVertex[!Axis])
            / (secondVertex[Axis] - firstVertex[Axis]);
        return [&, derivative] (uint16 x) {
            float32 y = x * derivative + firstVertex[!Axis]
                - firstVertex[Axis] * derivative;
            setCanvaPixel<Axis>(canva, x, y);
        };
    }

    template <bool Axis>
    void FontRasterizer::drawLineByAxis(Bitmap& canva, Vector2f const& firstVertex,
        Vector2f const& secondVertex) noexcept
    {
        auto func = getLinearFunction<Axis>(canva, firstVertex, secondVertex);
        float32 begin = firstVertex[Axis], end = secondVertex[Axis];
        if (begin > end)
            negativeContour(begin, end, func);
        else
            positiveContour(begin, end, func);
    }

    template <std::invocable<uint16> Function>
    void FontRasterizer::positiveContour(float32 begin, float32 end
        , Function function) noexcept
    {
        for (uint16 x = std::round(begin); x != std::round(end) + 1; ++x)
            function(x);
    }

    template <std::invocable<uint16> Function>
    void FontRasterizer::negativeContour(float32 begin, float32 end,
        Function function) noexcept
    {
        for (int32 x = std::round(begin); x != int32(std::round(end) - 1); --x)
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
    void FontRasterizer::setCanvaPixel(Bitmap& canva, uint16 x, float32 y) noexcept {
        auto position = getCoords<Axis>(x, y);
        (this->*pixelSetter)(canva, position);
        lastPosition = position;
    }

    template void FontRasterizer::setCanvaPixel<true>(Bitmap&, uint16, float32);
    template void FontRasterizer::setCanvaPixel<false>(Bitmap&, uint16, float32);

}
