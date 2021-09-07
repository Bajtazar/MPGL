#pragma once

#include "../../ImageLoading/LoadingHelpers.hpp"
#include "../../Mathematics/Vector.hpp"
#include "../../Traits/Concepts.hpp"

#include <algorithm>
#include <iterator>
#include <variant>
#include <vector>

namespace ge {

    typedef TwoVector<int16_t>                      Vector2si;

    class VectorizedGlyph {
    public:
        typedef std::vector<uint16_t>               UShortVec;
        typedef std::vector<int16_t>                ShortVec;
        typedef std::vector<uint8_t>                UByteVec;
        typedef std::variant<uint8_t, uint16_t>     Argument;
        typedef Vector<float, 6>                    ProjectionMatrix;

        struct Point {
            explicit Point(Vector2si const& position, bool onCurve) noexcept
                : position{position}, onCurve{onCurve} {}
            Vector2si                               position;
            bool                                    onCurve;
        };

        typedef std::vector<Point>                  Points;

        class SimpleGlyph {
        public:
            enum class Flags : uint8_t {
                OnCurvePoint = 0x01,
                XShortVector = 0x02,
                YShortVector = 0x04,
                RepeatFlag = 0x08,
                XIsSameOrPositive = 0x10,
                YIsSameOrPositive = 0x20
            };

            UShortVec                               endPtsOfContours;
            UByteVec                                instructions;
            Points                                  points;

            template <ByteInputIterator Iter>
            explicit SimpleGlyph(Iter& iter, int16_t numberOfContours);
        private:
            template <ByteInputIterator Iter>
            UByteVec readFlags(Iter& iter, uint16_t points);
            void generatePoints(UByteVec const& flags);
            template <ByteInputIterator Iter>
            void readCoords(bool axis, UByteVec const& flags, Iter& iter, Flags vectorFlag, Flags positiveFlag);
        };

        class CompositeGlyph {
        public:
            enum class Flags : uint16_t {
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

            struct Component {
                uint16_t                            glyphIndex;
                int16_t                             destination;
                int16_t                             source;
                ProjectionMatrix                    projection;

                explicit Component(uint16_t glyphIndex) noexcept : glyphIndex{glyphIndex},
                    destination{0}, source{0}, projection{1.f, 0.f, 0.f, 1.f, 0.f, 0.f} {}

                void readArgs(int16_t arg1, int16_t arg2, uint16_t const& flag) noexcept;

                template <ByteInputIterator Iter>
                void readProjection(Iter& iter, uint16_t const& flag);
            };

            typedef std::vector<Component>          Components;

            Components                              components;

            template <ByteInputIterator Iter>
            explicit CompositeGlyph(Iter& iter);
        private:
            typedef std::pair<int16_t, int16_t>     Arguments;

            template <ByteInputIterator Iter>
            static float read2Dot14(Iter& iter) { return readFixed<true, int16_t, float, 14>(iter); }

            template <ByteInputIterator Iter>
            Arguments readArguments(Iter& iter, uint16_t const& flag);
        };

        template <ByteInputIterator Iter>
        explicit VectorizedGlyph(Iter iter);

        explicit VectorizedGlyph(void) noexcept = default;

        bool exist(void) const noexcept { return !std::holds_alternative<std::monostate>(glyph); }
        bool isSimple(void) const noexcept { return std::holds_alternative<SimpleGlyph>(glyph); }

        SimpleGlyph const& getSimpleGlyph(void) const { return std::get<SimpleGlyph>(glyph); }
        CompositeGlyph const& getCompositeGlyph(void) const { return std::get<CompositeGlyph>(glyph); }

        Vector2si const& getMinDimmensions(void) const noexcept { return min; }
        Vector2si const& getMaxDimmensions(void) const noexcept { return max; }

    private:
        typedef std::variant<std::monostate, SimpleGlyph,
            CompositeGlyph>                         Glyph;

        template <ByteInputIterator Iter>
        int16_t readHeader(Iter& iter);

        Vector2si                                   min;
        Vector2si                                   max;
        Glyph                                       glyph;
    };

    constexpr inline uint8_t operator& (VectorizedGlyph::SimpleGlyph::Flags const& left,
        uint8_t right) noexcept
    {
        return static_cast<uint8_t>(left) & right;
    }

    constexpr inline uint8_t operator& (uint8_t left,
        VectorizedGlyph::SimpleGlyph::Flags const& right) noexcept
    {
        return left & static_cast<uint8_t>(right);
    }

    constexpr inline uint16_t operator& (VectorizedGlyph::CompositeGlyph::Flags const& left,
        uint16_t right) noexcept
    {
        return static_cast<uint16_t>(left) & right;
    }

    constexpr inline uint16_t operator& (uint16_t left,
        VectorizedGlyph::CompositeGlyph::Flags const& right) noexcept
    {
        return left & static_cast<uint16_t>(right);
    }

    template <ByteInputIterator Iter>
    int16_t VectorizedGlyph::readHeader(Iter& iter) {
        int16_t numberOfContours = readType<int16_t, true>(iter);
        min[0] = readType<int16_t, true>(iter);
        min[1] = readType<int16_t, true>(iter);
        max[0] = readType<int16_t, true>(iter);
        max[1] = readType<int16_t, true>(iter);
        return numberOfContours;
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::VectorizedGlyph(Iter iter) {
        if (auto numberOfContours = readHeader(iter); numberOfContours >= 0)
            glyph = SimpleGlyph{iter, numberOfContours};
        else
            glyph = CompositeGlyph{iter};
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::SimpleGlyph::SimpleGlyph(Iter& iter, int16_t numberOfContours) {
        for (auto i : std::views::iota(int16_t(0), numberOfContours))
            endPtsOfContours.push_back(readType<uint16_t, true>(iter));
        for (auto i : std::views::iota(uint16_t(0), readType<uint16_t, true>(iter)))
            instructions.push_back(readType<uint8_t, true>(iter));
        if (!endPtsOfContours.size()) return;
        uint16_t points = *std::ranges::max_element(endPtsOfContours) + 1;
        auto flags = readFlags(iter, points);
        generatePoints(flags);
        readCoords(0, flags, iter, Flags::XShortVector, Flags::XIsSameOrPositive);
        readCoords(1, flags, iter, Flags::YShortVector, Flags::YIsSameOrPositive);
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::UByteVec VectorizedGlyph::SimpleGlyph::readFlags(
        Iter& iter, uint16_t points)
    {
        UByteVec flags;
        for (uint16_t i = 0;i < points; ++i) {
            flags.emplace_back(readType<uint8_t>(iter));
            if (flags.back() & Flags::RepeatFlag) {
                uint8_t repeater = readType<uint8_t>(iter);
                i += repeater;
                while (repeater--)
                    flags.push_back(flags.back());
            }
        }
        return flags;
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::SimpleGlyph::readCoords(bool axis, UByteVec const& flags,
        Iter& iter, Flags vectorFlag, Flags positiveFlag)
    {
        int16_t value = 0;
        for (uint16_t i = 0; i < flags.size(); ++i) {
            if (flags[i] & vectorFlag)
                value += (flags[i] & positiveFlag) ? (int16_t)readType<uint8_t>(iter) : -(int16_t)readType<uint8_t>(iter);
            else if (~flags[i] & positiveFlag)
                value += readType<int16_t, true>(iter);
            points[i].position[axis] = value;
        }
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::CompositeGlyph::Arguments
        VectorizedGlyph::CompositeGlyph::readArguments(Iter& iter, uint16_t const& flag)
    {
        if (flag & Flags::Arg1And2AreWords)
            return Arguments{readType<int16_t, true>(iter), readType<int16_t, true>(iter)};
        return Arguments{readType<uint8_t>(iter), readType<uint8_t>(iter)};
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::CompositeGlyph::CompositeGlyph(Iter& iter) {
        uint16_t flag;
        do {
            flag = readType<uint16_t, true>(iter);
            Component component{readType<uint16_t, true>(iter)};
            auto&& [argument2, argument1] = readArguments(iter, flag);
            component.readArgs(argument1, argument2, flag);
            component.readProjection(iter, flag);
            components.push_back(std::move(component));
        } while (flag & Flags::MoreComponents);
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::CompositeGlyph::Component::readProjection(
        Iter& iter, uint16_t const& flag)
    {
        if (flag & Flags::WeHaveAScale) {
            projection[0] = read2Dot14(iter);
            projection[4] = projection[0];
        } else if (flag & Flags::WeHaveAnXAndYScale) {
            projection[0] = read2Dot14(iter);
            projection[4] = read2Dot14(iter);
        } else if (flag & Flags::WeHaveATwoByTwo) {
            projection[0] = read2Dot14(iter);
            projection[1] = read2Dot14(iter);
            projection[3] = read2Dot14(iter);
            projection[4] = read2Dot14(iter);
        }
    }

}
