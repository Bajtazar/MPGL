#pragma once

#include "../../ImageLoading/LoadingHelpers.hpp"
#include "../../Traits/Concepts.hpp"

#include <algorithm>
#include <iterator>
#include <variant>
#include <vector>

namespace ge {

    class VectorizedGlyph {
    public:
        typedef std::vector<uint16_t>               UShortVec;
        typedef std::vector<int16_t>                ShortVec;
        typedef std::vector<uint8_t>                UByteVec;
        typedef std::variant<uint8_t, uint16_t>     Argument;

        class SimpleGlyph {
        public:
            enum class Flags : uint8_t {
                OnCurvePoint = 0x01,
                XShortVector = 0x02,
                YShortVector = 0x04,
                RepeatFlag = 0x08,
                XIsSameOrPoisitive = 0x10,
                YIsSameOrPoisitive = 0x20
            };

            UShortVec                               endPtsOfContours;
            UByteVec                                instructions;
            UByteVec                                flags;
            ShortVec                                xCoordinates;
            ShortVec                                yCoordinates;

            template <ByteInputIterator Iter>
            explicit SimpleGlyph(Iter& iter, int16_t numberOfContours);
        private:
            template <ByteInputIterator Iter>
            void readFlags(Iter& iter, uint16_t points);
            template <ByteInputIterator Iter>
            void readCoords(ShortVec& vector, Iter& iter, uint8_t vectorFlag, uint8_t positiveFlag);
        };

        class CompositeGlyph {
        public:
            enum class Flags : uint8_t {

            };

            uint16_t                                flags;
            uint16_t                                glyphIndex;
            Argument                                argument1;
            Argument                                argument2;

            template <ByteInputIterator Iter>
            explicit CompositeGlyph(Iter& iter, int16_t numberOfContours);
        private:

        };

        template <bool isSimple, ByteInputIterator Iter>
        explicit VectorizedGlyph(Iter& iter, int16_t numberOfContours);

        bool isSimple(void) const noexcept { return std::holds_alternative<SimpleGlyph>(glyph); }
        SimpleGlyph& getSimpleGlyph(void) { return std::get<SimpleGlyph>(glyph); }
        CompositeGlyph& getCompositeGlyph(void) { return std::get<CompositeGlyph>(glyph); }
        SimpleGlyph const& getSimpleGlyph(void) const { return std::get<SimpleGlyph>(glyph); }
        CompositeGlyph const& getCompositeGlyph(void) const { return std::get<CompositeGlyph>(glyph); }

    private:
        std::variant<SimpleGlyph, CompositeGlyph>   glyph;
    };

    template <bool isSimple, ByteInputIterator Iter>
    VectorizedGlyph::VectorizedGlyph(Iter& iter, int16_t numberOfContours) {
        if constexpr (isSimple)
            glyph = SimpleGlyph{iter, numberOfContours};
        else
            glyph = CompositeGlyph{iter, numberOfContours};
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::SimpleGlyph::SimpleGlyph(Iter& iter, int16_t numberOfContours) {
        for (auto i : std::views::iota(int16_t(0), numberOfContours))
            endPtsOfContours.push_back(readType<uint16_t, true>(iter));
        for (auto i : std::views::iota(uint16_t(0), readType<uint16_t, true>(iter)))
            instructions.push_back(readType<uint16_t, true>(iter));
        if (!numberOfContours)
            return;
        uint16_t points = *std::ranges::max_element(endPtsOfContours) + 1;
        readFlags(iter, points);
        readCoords(xCoordinates, iter, Flags::XShortVector, Flags::XIsSameOrPoisitive);
        readCoords(yCoordinates, iter, Flags::YShortVector, Flags::YIsSameOrPoisitive);
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::SimpleGlyph::readFlags(Iter& iter, uint16_t points) {
        for (uint16_t i = 0;i < points; ++i) {
            flags.emplace_back(readType<uint8_t>(iter));
            if (flags.back() & Flags::RepeatFlag) {
                uint8_t repeater = readType<uint8_t>(iter);
                i += repeater;
                while (repeater--)
                    flags.push_back(flags.back());
            }
        }
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::SimpleGlyph::readCoords(ShortVec& vector, Iter& iter,
        uint8_t vectorFlag, uint8_t positiveFlag)
    {
        int16_t value = 0;
        for (uint8_t const& flag : flags) {
            if (flag & vectorFlag)
                value += (flag & positiveFlag) ? (int16_t)readType<uint8_t>(iter) : -(int16_t)readType<uint8_t>(iter);
            else if (~flag & positiveFlag)
                value += readType<int16_t, true>(iter);
            vector.emplace_back(value);
        }
    }

}
