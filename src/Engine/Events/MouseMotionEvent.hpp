#pragma once

#include "../Mathematics/Vector.hpp"
#include "Event.hpp"

namespace ge {

    struct MouseMotionEvent : public EventBase {
        explicit MouseMotionEvent(void) noexcept = default;

        virtual void onMouseMotionEvent(Vector2f const& position) = 0;

        virtual ~MouseMotionEvent(void) noexcept = default;
    };

}
