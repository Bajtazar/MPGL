#pragma once

#include "VectorizedGlyph.hpp"

#include <map>

namespace ge {

    struct FontData {
        uint16_t                            unitsPerEm;
        int16_t                             xMin;
        int16_t                             yMin;
        int16_t                             xMax;
        int16_t                             yMax;

        explicit FontData(void) noexcept = default;
    };

    struct GlyphData {
        VectorizedGlyph                     glyph;
        uint16_t                            advanceWidth;
        int16_t                             leftSideBearing;

        explicit GlyphData(VectorizedGlyph const& glyph,
            uint16_t advanceWidth, int16_t leftSideBearing) noexcept
                : glyph{glyph}, advanceWidth{advanceWidth},
                leftSideBearing{leftSideBearing} {}
        explicit GlyphData(void) noexcept = default;
    };

    struct KernTable {
        typedef std::map<uint32_t, int16_t> DistanceMap;

        DistanceMap                         distance;
        bool                                axis;

        template <ByteInputIterator Iter>
        explicit KernTable(Iter& iter);

        void setAxis(uint16_t const& coverage) noexcept;
    };

    template <ByteInputIterator Iter>
    KernTable::KernTable(Iter& iter) {
        if (readType<uint16_t, true>(iter)) return;
        std::advance(iter, 2);
        setAxis(readType<uint16_t, true>(iter));
        auto range = std::views::iota(uint16_t(0), readType<uint16_t, true>(iter));
        std::advance(iter, 6);
        for (auto _ : range) {
            uint32_t key = readType<uint16_t, true>(iter) << 0x0010;
            key += readType<uint16_t, true>(iter);
            distance[key] = readType<int16_t, true>(iter);
        }
    }

    typedef std::vector<KernTable>          Kern;

}
