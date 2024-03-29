/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Traits/Concepts.hpp>
#include <MPGL/IO/Readers.hpp>

#include <algorithm>
#include <iterator>
#include <variant>

namespace mpgl {

    /**
     * Provides information about font's loca table
     */
    class LocaTable {
    public:
        typedef std::vector<uint16>                 Loca16;
        typedef std::vector<uint32>                 Loca32;

        /**
         * Constructs a new Loca Table object
         */
        explicit LocaTable(void) noexcept = default;

        /**
         * Constructs a new Loca Table object from a given
         * iterator, index format and number of glyphs
         *
         * @tparam Iter the type of the iterator
         * @param iter the iterator used to load the loca table
         * @param indexFormat the index format
         * @param numGlyphs the number of glyphs
         */
        template <ByteInputIterator Iter>
        explicit LocaTable(
            Iter iter,
            int16 indexFormat,
            uint16 numGlyphs);

        /**
         * Returns the loca with the given index
         *
         * @throw std::out_of_range when loca is not available
         * @param index the index of the loca
         * @return the loca
         */
        [[nodiscard]] uint32 operator() (uint16 index) const;
    private:
        std::variant<Loca16, Loca32>                locaTable;
    };

    /**
     * Vectorized and stores information about glyph's outline
     */
    class VectorizedGlyph {
    public:
        typedef std::vector<uint16>                 UShortVec;
        typedef std::vector<int16>                  ShortVec;
        typedef std::vector<uint8>                  UByteVec;

        /**
         * Represents a point on the vectorized glyph. Provides
         * information whether this point is the control point
         * of the quadratic bézier curve
         */
        struct Point {
            /**
             * Constructs a new Point object with given position
             * and information whether the point is the control
             * point
             *
             * @param position the point's position
             * @param onCurve if point is the control point
             */
            explicit Point(
                Vector2si const& position,
                bool onCurve) noexcept
                    : position{position}, onCurve{onCurve} {}

            /**
             * Constructs a new Point object
             */
            explicit Point(void) noexcept = default;

            Vector2si                               position;
            bool                                    onCurve;
        };

        typedef std::vector<Point>                  Points;

        /**
         * Represents the glyph outline
         */
        struct Glyph {
            UShortVec                               endPtsOfContours;
            Points                                  points;

            /**
             * Constructs a new Glyph object
             */
            explicit Glyph(void) noexcept = default;
        };

        /**
         * Represents the flags of the simple outline
         */
        enum class SimpleFlags : uint8 {
            OnCurvePoint                          = 0x01,
            XShortVector                          = 0x02,
            YShortVector                          = 0x04,
            RepeatFlag                            = 0x08,
            XIsSameOrPositive                     = 0x10,
            YIsSameOrPositive                     = 0x20
        };

        /**
         * Represents the flags of the composite outline
         */
        enum class CompositeFlags : uint16 {
            Arg1And2AreWords                      = 0x0001,
            ArgsAreXYValues                       = 0x0002,
            RoundXYToGrid                         = 0x0004,
            WeHaveAScale                          = 0x0008,
            MoreComponents                        = 0x0020,
            WeHaveAnXAndYScale                    = 0x0040,
            WeHaveATwoByTwo                       = 0x0080,
            WeHaveInstructions                    = 0x0100,
            UseMyMetrics                          = 0x0200,
            OverlapCompound                       = 0x0400,
            ScaledComponentOffset                 = 0x0800,
            UnscaledComponentOffset               = 0x1000
        };

        /**
         * Constructs a new vectorized glyph object. Reads
         * the glyph from given pointer using given offset
         * and loca table
         *
         * @tparam Iter the iterator type
         * @param begin the iterator to the begining
         * @param offset the offset to the glyph
         * @param locaTable the constant reference to the loca table
         */
        template <ByteInputIterator Iter>
        explicit VectorizedGlyph(
            Iter const begin,
            std::ptrdiff_t offset,
            LocaTable const& locaTable);

        /**
         * Constructs a new Vectorized Glyph object
         */
        explicit VectorizedGlyph(void) noexcept = default;

        /**
         * Returns whether the glyph has an outline
         *
         * @return if the glyph has an outline
         */
        [[nodiscard]] bool exist(void) const noexcept
            { return !std::holds_alternative<std::monostate>(glyph); }

        /**
         * Returns a constant reference to glyph object
         *
         * @return the constant reference to glyph object
         */
        [[nodiscard]] Glyph const& getGlyph(void) const
            { return std::get<Glyph>(glyph); }

        /**
         * Returns an outline's minimum dimensions
         *
         * @return the outline's minimum dimensions
         */
        [[nodiscard]] Vector2si const& getMinDimensions(
            void) const noexcept
                { return min; }

        /**
         * Returns an outline's maximum dimensions
         *
         * @return the outline's maximum dimensions
         */
        [[nodiscard]] Vector2si const& getMaxDimensions(
            void) const noexcept
                { return max; }
    private:
        typedef std::variant<std::monostate, Glyph> GlyphVariant;
        typedef Vector<float32, 6>                  ProjectionMatrix;

        /**
         * Represents an outline's component
         */
        struct Component {
            uint16                                  glyphIndex;
            ProjectionMatrix                        projection;

            /**
             * Constructs a new Component object from the given
             * glyph's index
             *
             * @param glyphIndex the glyph's index
             */
            explicit Component(uint16 glyphIndex) noexcept
                : glyphIndex{glyphIndex},
                projection{1.f, 0.f, 0.f, 0.f, 1.f, 0.f} {}

            /**
             * Parser the projection matrix from iterator
             *
             * @tparam Iter the iterator type
             * @param iter the reference to byte input iterator
             * @param flag the component's flags
             */
            template <ByteInputIterator Iter>
            void parseProjection(Iter& iter, uint16 const& flag);

            /**
             * Reads component arguments
             *
             * @param arg1 the first argument
             * @param arg2 the second argument
             * @param flag the component's flag
             */
            void readArgs(
                int16 arg1,
                int16 arg2,
                uint16 const& flag) noexcept;
        };

        typedef std::vector<Component>              Components;
        typedef std::pair<int16, int16>             Arguments;

        /**
         * Parses glyph's outline header
         *
         * @tparam Iter the iterator type
         * @param iter the reference to byte input iterator
         * @return the number of glyph's contours
         */
        template <ByteInputIterator Iter>
        int16 parseHeader(Iter& iter);

        /**
         * Parses simple glyph outline
         *
         * @tparam Iter the iterator type
         * @param iter the reference to byte input iterator
         * @param numberOfContours the number of glyph's contours
         * @param glyph the reference to the glyph object
         */
        template <ByteInputIterator Iter>
        void parseSimpleGlyph(
            Iter& iter,
            int16 numberOfContours,
            Glyph& glyph);

        /**
         * Reads the simple glyph's coordinates
         *
         * @tparam Iter the iterator type
         * @param iter the reference to byte input iterator
         * @param numberOfContours the number of glyph's contours
         * @param glyph the reference to the glyph object
         */
        template <ByteInputIterator Iter>
        void readSimpleGlyphCoords(
            Iter& iter,
            Glyph& glyph,
            uint16 points);

        /**
         * Reads the simple glyph's flags
         *
         * @tparam Iter the iterator type
         * @param iter the reference to byte input iterator
         * @param points the number of glyph's contour points
         * @return VectorizedGlyph::UByteVec
         */
        template <ByteInputIterator Iter>
        VectorizedGlyph::UByteVec readSimpleFlags(
            Iter& iter,
            uint16 points);

        /**
         * Reads coords with respect to the axis
         *
         * @tparam Axis the axis id
         * @tparam Iter the iterator type
         * @param glyph the reference to the glyph object
         * @param flags the glyph's flags
         * @param iter the reference to byte input iterator
         * @param vectorFlag the vector flag
         * @param positiveFlag the positive flag
         */
        template <bool Axis, ByteInputIterator Iter>
        void readCoords(
            Glyph& glyph,
            UByteVec const&
            flags,
            Iter& iter,
            SimpleFlags vectorFlag,
            SimpleFlags positiveFlag);

        /**
         * Parsed compound glyph outline
         *
         * @tparam Iter the iterator type
         * @param begin the iterator to the begining of the
         * compound glyph
         * @param iter the reference to byte input iterator
         * @param table the constant reference to the loca table
         * @param glyph the reference to the glyph object
         */
        template <ByteInputIterator Iter>
        void parseCompoundGlyph(
            Iter const begin,
            Iter& iter,
            LocaTable const& table,
            Glyph& glyph);

        /**
         * Parses the arguments from the iterator
         *
         * @tparam Iter the iterator type
         * @param iter the reference to byte input iterator
         * @param flag the arguments flag
         * @return the glyph's arguments
         */
        template <ByteInputIterator Iter>
        Arguments parseArguments(
            Iter& iter,
            uint16 const& flag) const;

        /**
         * Generates glyph outline's points
         *
         * @param glyph the reference to the glyph object
         * @param flags the glyph flags
         */
        void generatePoints(Glyph& glyph, UByteVec const& flags);

        /**
         * Adds the components to the glyph's outline
         *
         * @tparam Iter the iterator type
         * @param begin the iterator to the begining of the glyph
         * @param table the constant reference to the loca table
         * @param glyph the reference to the glyph object
         * @param components the constant reference to the
         * components vector
         */
        template <ByteInputIterator Iter>
        void addComponentsToGlyph(
            Iter const begin,
            LocaTable const& table,
            Glyph& glyph,
            Components const& components);

        /**
         * Parses the outline's subglyph
         *
         * @tparam Iter the iterator type
         * @param begin the iterator to the begining of the glyph
         * @param offset the offset to the subglyph
         * @param locaTable the constant reference to the loca table
         * @return the outline's subglyph
         */
        template <ByteInputIterator Iter>
        Glyph parseSubglyph(
            Iter const begin,
            uint16 offset,
            LocaTable const& locaTable);

        /**
         * Transforms the given subglyph
         *
         * @param component the constant reference to the subglyph's
         * component
         * @param target the reference to the main glyph object
         * @param subglyph the constant reference to the
         * subglyph object
         */
        void transformSubglyph(
            Component const& component,
            Glyph& target,
            Glyph const& subglyph);

        Vector2si                                   min;
        Vector2si                                   max;
        GlyphVariant                                glyph;

        /**
         * Reads the fixed type in the 2.14 format
         *
         * @tparam Iter the iterator type
         * @param iter the reference to byte input iterator
         * @return the fixed point
         */
        template <ByteInputIterator Iter>
        static float32 read2Dot14(Iter& iter)
            { return readFixed<true, int16, float32, 14>(iter); }
    };

    [[nodiscard]] constexpr inline uint8 operator& (
        VectorizedGlyph::SimpleFlags const& left,
        uint8 right) noexcept;

    [[nodiscard]] constexpr inline uint8 operator& (
        uint8 left,
        VectorizedGlyph::SimpleFlags const& right) noexcept;

    [[nodiscard]] constexpr inline uint16 operator& (
        VectorizedGlyph::CompositeFlags const& left,
        uint16 right) noexcept;

    [[nodiscard]] constexpr inline uint16 operator& (
        uint16 left,
        VectorizedGlyph::CompositeFlags const& right) noexcept;

}

#include <MPGL/Core/Text/VectorizedGlyph.tpp>
#include <MPGL/Core/Text/VectorizedGlyph.ipp>
