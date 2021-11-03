#pragma once

#include "../Mathematics/Vector.hpp"
#include "Event.hpp"

namespace ge {

    struct ScreenTransformationEvent : public EventBase {
        explicit ScreenTransformationEvent(void) noexcept = default;

        virtual void onScreenTransformation(Vector2ui const& oldDimmensions) noexcept = 0;

        virtual ~ScreenTransformationEvent(void) = default;
    };

}
