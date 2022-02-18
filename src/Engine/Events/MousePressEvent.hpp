#pragma once

#include "../IO/Devices/Mouse.hpp"
#include "Event.hpp"

namespace mpgl {

    struct MousePressEvent : public EventBase {
        explicit MousePressEvent(void) noexcept = default;

        virtual void onMousePress(
            MouseButton const& button) noexcept = 0;

        virtual ~MousePressEvent(void) noexcept = default;
    };

}
