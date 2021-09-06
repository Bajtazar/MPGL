#include "VectorizedGlyph.hpp"

namespace ge {

    void VectorizedGlyph::SimpleGlyph::generatePoints(UByteVec const& flags) {
        for (uint8_t const& flag : flags)
            points.emplace_back(Vector2si{}, (flag & Flags::OnCurvePoint) > 0);
    }

}
