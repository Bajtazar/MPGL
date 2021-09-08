#include "VectorizedGlyph.hpp"

namespace ge {

    void VectorizedGlyph::generatePoints(Glyph& glyph, UByteVec const& flags) {
        for (uint8_t const& flag : flags)
            glyph.points.emplace_back(Vector2si{}, (flag & SimpleFlags::OnCurvePoint) > 0);
    }

    uint32_t LocaTable::operator() (uint16_t index) const {
        if (std::holds_alternative<Loca16>(locaTable))
            return 2 * uint32_t(std::get<Loca16>(locaTable).at(index));
        return std::get<Loca32>(locaTable).at(index);
    }

    void VectorizedGlyph::Component::readArgs(
        int16_t arg1, int16_t arg2, uint16_t const& flag) noexcept
    {
        if (flag & CompositeFlags::ArgsAreXYValues) {
            projection[2] = arg1;
            projection[5] = arg2;
        }
    }

    void VectorizedGlyph::transformSubglyph(Component const& component,
        Glyph& target, Glyph const& subglyph)
    {
        for (const auto& point : subglyph.points)
            target.points.push_back(
                Point{
                    vectorCast<int16_t>(Vector2f{
                        point.position[0] * component.projection[0] + point.position[1]
                            * component.projection[1] + component.projection[2],
                        point.position[0] * component.projection[3] + point.position[1]
                            * component.projection[4] + component.projection[5]
                    }), point.onCurve
                });
    }

}
