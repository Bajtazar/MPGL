#pragma once

#include "VectorizedGlyph.hpp"

#include <map>

namespace mpgl {

    struct FontData {
        uint16                              unitsPerEm;
        int16                               xMin;
        int16                               yMin;
        int16                               xMax;
        int16                               yMax;

        explicit FontData(void) noexcept = default;
    };

    struct GlyphData {
        VectorizedGlyph                     glyph;
        uint16                              advanceWidth;
        int16                               leftSideBearing;

        explicit GlyphData(VectorizedGlyph const& glyph,
            uint16 advanceWidth, int16 leftSideBearing) noexcept
                : glyph{glyph}, advanceWidth{advanceWidth},
                leftSideBearing{leftSideBearing} {}
        explicit GlyphData(void) noexcept = default;
    };

    struct KernTable {
        typedef std::map<uint32, int16> DistanceMap;

        DistanceMap                         distance;
        bool                                axis;

        template <ByteInputIterator Iter>
        explicit KernTable(Iter& iter);

        explicit KernTable(void) noexcept = default;

        void setAxis(uint16 const& coverage) noexcept;
    };

    template <ByteInputIterator Iter>
    KernTable::KernTable(Iter& iter) {
        if (readType<uint16, true>(iter)) return;
        std::advance(iter, 2);
        setAxis(readType<uint16, true>(iter));
        auto range = std::views::iota(uint16(0),
            readType<uint16, true>(iter));
        std::advance(iter, 6);
        for (auto _ : range) {
            uint32 key = readType<uint16, true>(iter) << 0x0010;
            key += readType<uint16, true>(iter);
            distance[key] = readType<int16, true>(iter);
        }
    }

    typedef std::vector<KernTable>          Kern;
    typedef std::map<uint16, GlyphData>     GlyphMap;

}
