#pragma once

#include "../../Mathematics/Vector.hpp"
#include "../../Traits/Concepts.hpp"
#include "../../IO/Helper.hpp"

#include <algorithm>
#include <iterator>
#include <variant>

namespace ge {

    class LocaTable {
    public:
        typedef std::vector<uint16>                 Loca16;
        typedef std::vector<uint32>                 Loca32;

        explicit LocaTable(void) noexcept = default;

        template <ByteInputIterator Iter>
        explicit LocaTable(Iter iter, int16 indexFormat,
            uint16 numGlyphs);

        uint32 operator() (uint16 index) const;
    private:
        std::variant<Loca16, Loca32>                locaTable;
    };

    typedef Vector2<int16>                          Vector2si;

    class VectorizedGlyph {
    public:
        typedef std::vector<uint16>                 UShortVec;
        typedef std::vector<int16>                  ShortVec;
        typedef std::vector<uint8>                  UByteVec;

        struct Point {
            explicit Point(Vector2si const& position, bool onCurve) noexcept
                : position{position}, onCurve{onCurve} {}
            explicit Point(void) noexcept = default;
            Vector2si                               position;
            bool                                    onCurve;
        };

        typedef std::vector<Point>                  Points;

        struct Glyph {
            UShortVec                               endPtsOfContours;
            Points                                  points;

            explicit Glyph(void) noexcept = default;
        };

        enum class SimpleFlags : uint8 {
            OnCurvePoint = 0x01,
            XShortVector = 0x02,
            YShortVector = 0x04,
            RepeatFlag = 0x08,
            XIsSameOrPositive = 0x10,
            YIsSameOrPositive = 0x20
        };

        enum class CompositeFlags : uint16 {
            Arg1And2AreWords = 0x0001,
            ArgsAreXYValues = 0x0002,
            RoundXYToGrid = 0x0004,
            WeHaveAScale = 0x0008,
            MoreComponents = 0x0020,
            WeHaveAnXAndYScale = 0x0040,
            WeHaveATwoByTwo = 0x0080,
            WeHaveInstructions = 0x0100,
            UseMyMetrics = 0x0200,
            OverlapCompound = 0x0400,
            ScaledComponentOffset = 0x0800,
            UnscaledComponentOffset = 0x1000
        };

        template <ByteInputIterator Iter>
        explicit VectorizedGlyph(Iter const begin, std::ptrdiff_t offset,
            LocaTable const& locaTable);

        explicit VectorizedGlyph(void) noexcept = default;

        bool exist(void) const noexcept { return !std::holds_alternative<std::monostate>(glyph); }
        Glyph const& getGlyph(void) const { return std::get<Glyph>(glyph); }

        Vector2si const& getMinDimmensions(void) const noexcept { return min; }
        Vector2si const& getMaxDimmensions(void) const noexcept { return max; }

    private:
        typedef std::variant<std::monostate, Glyph> GlyphVariant;
        typedef Vector<float32, 6>                    ProjectionMatrix;

        struct Component {
            uint16                                  glyphIndex;
            ProjectionMatrix                        projection;

            explicit Component(uint16 glyphIndex) noexcept :
                glyphIndex{glyphIndex}, projection{1.f, 0.f, 0.f, 0.f, 1.f, 0.f} {}

            template <ByteInputIterator Iter>
            void parseProjection(Iter& iter, uint16 const& flag);

            void readArgs(int16 arg1, int16 arg2, uint16 const& flag) noexcept;
        };

        typedef std::vector<Component>              Components;
        typedef std::pair<int16, int16>         Arguments;

        template <ByteInputIterator Iter>
        int16 parseHeader(Iter& iter);

        template <ByteInputIterator Iter>
        void parseSimpleGlyph(Iter& iter, int16 numberOfContours, Glyph& glyph);

        template <ByteInputIterator Iter>
        VectorizedGlyph::UByteVec readSimpleFlags(Iter& iter, uint16 points);

        template <bool Axis, ByteInputIterator Iter>
        void readCoords(Glyph& glyph, UByteVec const& flags, Iter& iter,
            SimpleFlags vectorFlag, SimpleFlags positiveFlag);

        template <ByteInputIterator Iter>
        void parseCompoundGlyph(Iter const begin, Iter& iter,
            LocaTable const& table, Glyph& glyph);

        template <ByteInputIterator Iter>
        Arguments parseArguments(Iter& iter, uint16 const& flag) const;

        void generatePoints(Glyph& glyph, UByteVec const& flags);

        template <ByteInputIterator Iter>
        void addComponentsToGlyph(Iter const begin, LocaTable const& table,
            Glyph& glyph, Components const& components);

        template <ByteInputIterator Iter>
        Glyph parseSubglyph(Iter const begin, uint16 offset, LocaTable const& locaTable);

        void transformSubglyph(Component const& component, Glyph& target, Glyph const& subglyph);

        Vector2si                                   min;
        Vector2si                                   max;
        GlyphVariant                                glyph;

        template <ByteInputIterator Iter>
        static float32 read2Dot14(Iter& iter) { return readFixed<true, int16, float32, 14>(iter); }
    };

    constexpr inline uint8 operator& (VectorizedGlyph::SimpleFlags const& left,
        uint8 right) noexcept
    {
        return static_cast<uint8>(left) & right;
    }

    constexpr inline uint8 operator& (uint8 left,
        VectorizedGlyph::SimpleFlags const& right) noexcept
    {
        return left & static_cast<uint8>(right);
    }

    constexpr inline uint16 operator& (VectorizedGlyph::CompositeFlags const& left,
        uint16 right) noexcept
    {
        return static_cast<uint16>(left) & right;
    }

    constexpr inline uint16 operator& (uint16 left,
        VectorizedGlyph::CompositeFlags const& right) noexcept
    {
        return left & static_cast<uint16>(right);
    }

    template <ByteInputIterator Iter>
    int16 VectorizedGlyph::parseHeader(Iter& iter) {
        int16 numberOfContours = readType<int16, true>(iter);
        min[0] = readType<int16, true>(iter);
        min[1] = readType<int16, true>(iter);
        max[0] = readType<int16, true>(iter);
        max[1] = readType<int16, true>(iter);
        return numberOfContours;
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::VectorizedGlyph(Iter const begin, std::ptrdiff_t offset,
        LocaTable const& locaTable)
    {
        auto iter = begin + offset;
        Glyph glyph;
        if (auto numberOfContours = parseHeader(iter); numberOfContours >= 0) {
            parseSimpleGlyph(iter, numberOfContours, glyph);
            this->glyph = std::move(glyph);
        } else {
            parseCompoundGlyph(begin, iter, locaTable, glyph);
            this->glyph = std::move(glyph);
        }
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::parseSimpleGlyph(Iter& iter, int16 numberOfContours, Glyph& glyph) {
        for (auto i : std::views::iota(int16(0), numberOfContours))
            glyph.endPtsOfContours.push_back(readType<uint16, true>(iter));
        std::advance(iter, readType<uint16, true>(iter));
        if (!glyph.endPtsOfContours.size()) return;
        uint16 points = *std::ranges::max_element(glyph.endPtsOfContours) + 1;
        auto flags = readSimpleFlags(iter, points);
        generatePoints(glyph, flags);
        readCoords<0>(glyph, flags, iter, SimpleFlags::XShortVector, SimpleFlags::XIsSameOrPositive);
        readCoords<1>(glyph, flags, iter, SimpleFlags::YShortVector, SimpleFlags::YIsSameOrPositive);
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::UByteVec VectorizedGlyph::readSimpleFlags(Iter& iter, uint16 points) {
        UByteVec flags;
        for (uint16 i = 0; i < points; ++i) {
            flags.emplace_back(readType<uint8>(iter));
            if (flags.back() & SimpleFlags::RepeatFlag) {
                uint8 repeater = readType<uint8>(iter);
                i += repeater;
                while (repeater--)
                    flags.push_back(flags.back());
            }
        }
        return flags;
    }

    template <bool Axis, ByteInputIterator Iter>
    void VectorizedGlyph::readCoords(Glyph& glyph, UByteVec const& flags, Iter& iter,
        SimpleFlags vectorFlag, SimpleFlags positiveFlag)
    {
        int16 value = 0;
        for (uint16 i = 0; i != flags.size(); ++i) {
            if (flags[i] & vectorFlag)
                value += (flags[i] & positiveFlag) ? (int16)readType<uint8>(iter) : -(int16)readType<uint8>(iter);
            else if (~flags[i] & positiveFlag)
                value += readType<int16, true>(iter);
            glyph.points[i].position[Axis] = value;
        }
    }

    template <ByteInputIterator Iter>
    LocaTable::LocaTable(Iter iter, int16 indexFormat, uint16 numGlyphs) {
        if (indexFormat) {
            Loca32 loca;
            for (uint16 i = 0; i < numGlyphs + 1; ++i)
                loca.push_back(readType<uint32, true>(iter));
            locaTable = std::move(loca);
        } else {
            Loca16 loca;
            for (uint16 i = 0; i < numGlyphs + 1; ++i)
                loca.push_back(readType<uint16, true>(iter));
            locaTable = std::move(loca);
        }
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::Arguments VectorizedGlyph::parseArguments(Iter& iter,
        uint16 const& flag) const
    {
        if (flag & CompositeFlags::Arg1And2AreWords)
            return Arguments{readType<int16, true>(iter), readType<int16, true>(iter)};
        return Arguments{readType<uint8>(iter), readType<uint8>(iter)};
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::parseCompoundGlyph(Iter const begin, Iter& iter,
        LocaTable const& table, Glyph& glyph)
    {
        Components components;
        uint16 flag;
        do {
            flag = readType<uint16, true>(iter);
            Component component{readType<uint16, true>(iter)};
            auto&& [argument2, argument1] = parseArguments(iter, flag);
            component.readArgs(argument1, argument2, flag);
            component.parseProjection(iter, flag);
        } while (flag & CompositeFlags::MoreComponents);
        addComponentsToGlyph(begin, table, glyph, components);
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::Component::parseProjection(Iter& iter,
        uint16 const& flag)
    {
        if (flag & CompositeFlags::WeHaveAScale) {
            projection[0] = read2Dot14(iter);
            projection[4] = projection[0];
        } else if (flag & CompositeFlags::WeHaveAnXAndYScale) {
            projection[0] = read2Dot14(iter);
            projection[4] = read2Dot14(iter);
        } else if (flag & CompositeFlags::WeHaveATwoByTwo) {
            projection[0] = read2Dot14(iter);
            projection[1] = read2Dot14(iter);
            projection[3] = read2Dot14(iter);
            projection[4] = read2Dot14(iter);
        }
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::addComponentsToGlyph(Iter const begin,
        LocaTable const& table, Glyph& glyph, Components const& components)
    {
        for (auto const& component : components) {
            uint16 currentSize = static_cast<uint16>(glyph.points.size());
            auto subglyph = parseSubglyph(begin, table(component.glyphIndex), table);
            transformSubglyph(component, glyph, subglyph);
            for (auto const& contourEnds : subglyph.endPtsOfContours)
                glyph.endPtsOfContours.push_back(contourEnds + currentSize);
        }
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::Glyph VectorizedGlyph::parseSubglyph(Iter const begin,
        uint16 offset, LocaTable const& locaTable)
    {
        auto iter = begin + offset;
        int16 numberOfContours = readType<int16, true>(iter);
        std::advance(iter, 8);
        Glyph glyph;
        if (numberOfContours >= 0)
            parseSimpleGlyph(iter, numberOfContours, glyph);
        else
            parseCompoundGlyph(begin, iter, locaTable, glyph);
        return glyph;
    }

}
