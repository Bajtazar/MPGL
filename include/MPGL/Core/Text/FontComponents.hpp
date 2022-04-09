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

#include <MPGL/Core/Text/VectorizedGlyph.hpp>

#include <map>

namespace mpgl {

    /**
     * Provides basic informations about font
     */
    struct FontData {
        uint16                              unitsPerEm;
        int16                               xMin;
        int16                               yMin;
        int16                               xMax;
        int16                               yMax;

        /**
         * Constructs a new Font Data object
         */
        explicit FontData(void) noexcept = default;
    };

    /**
     * Provides basic informations about glyph
     */
    struct GlyphData {
        VectorizedGlyph                     glyph;
        uint16                              advanceWidth;
        int16                               leftSideBearing;

        /**
         * Constructs a new Glyph Data object from the given
         * vectorized glyph, advance width and left side
         * bearing
         *
         * @param glyph the vectorized glyph
         * @param advanceWidth the advance width
         * @param leftSideBearing the left side bearing
         */
        explicit GlyphData(
            VectorizedGlyph const& glyph,
            uint16 advanceWidth,
            int16 leftSideBearing) noexcept
                : glyph{glyph}, advanceWidth{advanceWidth},
                leftSideBearing{leftSideBearing} {}

        /**
         * Constructs a new Glyph Data object
         */
        explicit GlyphData(void) noexcept = default;
    };

    /**
     * Provides information about font's kern table
     */
    struct KernTable {
        typedef std::map<uint32, int16> DistanceMap;

        DistanceMap                         distance;
        bool                                axis;

        /**
         * Constructs a new Kern Table object. Loads
         * the kern table from the given iterator
         *
         * @tparam Iter the iterator type
         * @param iter the reference to the iterator object
         */
        template <ByteInputIterator Iter>
        explicit KernTable(Iter& iter);

        /**
         * Construct a new empty Kern Table object
         */
        explicit KernTable(void) noexcept = default;

        /**
         * Sets the kern table axis
         *
         * @param coverage the kern table coverage
         */
        void setAxis(uint16 const& coverage) noexcept;
    };

    template <ByteInputIterator Iter>
    KernTable::KernTable(Iter& iter) {
        if (readType<uint16, true>(iter)) return;
        std::advance(iter, 2);
        setAxis(readType<uint16, true>(iter));
        uint16 end = readType<uint16, true>(iter);
        std::advance(iter, 6);
        for (uint16 i = 0; i < end; ++i) {
            uint32 key = readType<uint16, true>(iter) << 0x0010;
            key += readType<uint16, true>(iter);
            distance[key] = readType<int16, true>(iter);
        }
    }

    typedef std::vector<KernTable>          Kern;
    typedef std::map<uint16, GlyphData>     GlyphMap;

}
