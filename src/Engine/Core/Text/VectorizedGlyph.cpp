#include "VectorizedGlyph.hpp"

namespace ge {

    void VectorizedGlyph::SimpleGlyph::generatePoints(UByteVec const& flags) {
        for (uint8_t const& flag : flags)
            points.emplace_back(Vector2si{}, (flag & Flags::OnCurvePoint) > 0);
    }

    void VectorizedGlyph::CompositeGlyph::Component::readArgs(
        int16_t arg1, int16_t arg2, uint16_t const& flag) noexcept
    {
        if (flag & Flags::ArgsAreXYValues) {
            projection[2] = arg1;
            projection[5] = arg2;
        } else {
            destination = arg1;
            source = arg2;
        }
    }

}
