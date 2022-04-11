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
    VectorizedGlyph::VectorizedGlyph(
        Iter const begin,
        std::ptrdiff_t offset,
        LocaTable const& locaTable)
    {
        auto iter = begin + offset;
        Glyph glyph;
        if (auto numberOfContours = parseHeader(iter);
            numberOfContours >= 0)
        {
            parseSimpleGlyph(iter, numberOfContours, glyph);
            this->glyph = std::move(glyph);
        } else {
            parseCompoundGlyph(begin, iter, locaTable, glyph);
            this->glyph = std::move(glyph);
        }
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::readSimpleGlyphCoords(
        Iter& iter,
        Glyph& glyph,
        uint16 points)
    {
        auto flags = readSimpleFlags(iter, points);
        generatePoints(glyph, flags);
        readCoords<0>(glyph, flags, iter, SimpleFlags::XShortVector,
            SimpleFlags::XIsSameOrPositive);
        readCoords<1>(glyph, flags, iter, SimpleFlags::YShortVector,
            SimpleFlags::YIsSameOrPositive);
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::parseSimpleGlyph(
        Iter& iter,
        int16 numberOfContours,
        Glyph& glyph)
    {
        for (uint16 i = 0; i < numberOfContours; ++i)
            glyph.endPtsOfContours.push_back(
                readType<uint16, true>(iter));
        std::advance(iter, readType<uint16, true>(iter));
        if (!glyph.endPtsOfContours.size())
            return;
        readSimpleGlyphCoords(iter, glyph, 1 +
            *std::ranges::max_element(glyph.endPtsOfContours));
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::UByteVec VectorizedGlyph::readSimpleFlags(
        Iter& iter,
        uint16 points)
    {
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
    void VectorizedGlyph::readCoords(
        Glyph& glyph,
        UByteVec const& flags,
        Iter& iter,
        SimpleFlags vectorFlag,
        SimpleFlags positiveFlag)
    {
        int16 value = 0;
        for (uint16 i = 0; i != flags.size(); ++i) {
            if (flags[i] & vectorFlag) {
                int16 subval = readType<uint8>(iter);
                value += (flags[i] & positiveFlag) ? subval : -subval;
            } else if (~flags[i] & positiveFlag)
                value += readType<int16, true>(iter);
            glyph.points[i].position[Axis] = value;
        }
    }

    template <ByteInputIterator Iter>
    LocaTable::LocaTable(
        Iter iter,
        int16 indexFormat,
        uint16 numGlyphs)
    {
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
    VectorizedGlyph::Arguments VectorizedGlyph::parseArguments(
        Iter& iter,
        uint16 const& flag) const
    {
        if (flag & CompositeFlags::Arg1And2AreWords)
            return Arguments{readType<int16, true>(iter),
                readType<int16, true>(iter)};
        return Arguments{readType<uint8>(iter),
            readType<uint8>(iter)};
    }

    template <ByteInputIterator Iter>
    void VectorizedGlyph::parseCompoundGlyph(
        Iter const begin,
        Iter& iter,
        LocaTable const& table,
        Glyph& glyph)
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
    void VectorizedGlyph::addComponentsToGlyph(
        Iter const begin,
        LocaTable const& table,
        Glyph& glyph,
        Components const& components)
    {
        for (auto const& component : components) {
            uint16 currentSize = static_cast<uint16>(
                glyph.points.size());
            auto subglyph = parseSubglyph(begin, table(
                component.glyphIndex), table);
            transformSubglyph(component, glyph, subglyph);
            for (auto const& contourEnds : subglyph.endPtsOfContours)
                glyph.endPtsOfContours.push_back(
                    contourEnds + currentSize);
        }
    }

    template <ByteInputIterator Iter>
    VectorizedGlyph::Glyph VectorizedGlyph::parseSubglyph(
        Iter const begin,
        uint16 offset,
        LocaTable const& locaTable)
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
