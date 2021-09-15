#pragma once

#include "../Mathematics/Vector.hpp"

namespace ge {

    struct MouseMotionEvent {
        explicit MouseMotionEvent(void) noexcept = default;

        virtual void onMouseMotionEvent(Vector2f const& position) = 0;

        virtual ~MouseMotionEvent(void) noexcept = default;
    };

}
