#pragma once

#include "../Mathematics/Vector.hpp"

namespace ge {

    struct ScreenTransformationEvent {
        explicit ScreenTransformationEvent(void) noexcept = default;

        virtual void onScreenTransformation(Vector2ui const& oldDimmensions) noexcept = 0;

        virtual ~ScreenTransformationEvent(void) = default;
    };

}
