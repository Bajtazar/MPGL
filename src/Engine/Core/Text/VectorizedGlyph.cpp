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
#include "VectorizedGlyph.hpp"

namespace mpgl {

    void VectorizedGlyph::generatePoints(
        Glyph& glyph,
        UByteVec const& flags)
    {
        for (uint8 const& flag : flags)
            glyph.points.emplace_back(Vector2si{},
                (flag & SimpleFlags::OnCurvePoint) > 0);
    }

    [[nodiscard]] uint32 LocaTable::operator() (uint16 index) const {
        if (std::holds_alternative<Loca16>(locaTable))
            return 2 * uint32(std::get<Loca16>(locaTable).at(index));
        return std::get<Loca32>(locaTable).at(index);
    }

    void VectorizedGlyph::Component::readArgs(
        int16 arg1, int16 arg2, uint16 const& flag) noexcept
    {
        if (flag & CompositeFlags::ArgsAreXYValues) {
            projection[2] = arg1;
            projection[5] = arg2;
        }
    }

    void VectorizedGlyph::transformSubglyph(
        Component const& component,
        Glyph& target,
        Glyph const& subglyph)
    {
        for (const auto& point : subglyph.points)
            target.points.push_back(
                Point{vectorCast<int16>(Vector2f{
                    point.position[0] * component.projection[0]
                        + point.position[1] * component.projection[1]
                        + component.projection[2],
                    point.position[0] * component.projection[3]
                        + point.position[1] * component.projection[4]
                        + component.projection[5]
                }), point.onCurve});
    }

}
