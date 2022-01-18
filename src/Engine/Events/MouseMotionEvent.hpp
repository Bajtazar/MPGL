#pragma once

#include "../Mathematics/Vector.hpp"
#include "Event.hpp"

namespace ge {

    struct MouseMotionEvent : public EventBase {
        explicit MouseMotionEvent(void) noexcept = default;

        virtual void onMouseMotion(
            Vector2f const& position) noexcept = 0;

        virtual ~MouseMotionEvent(void) noexcept = default;
    };

}
