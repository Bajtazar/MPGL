#pragma once

#include "../IO/Devices/Mouse.hpp"
#include "Event.hpp"

namespace ge {

    struct MouseReleaseEvent : public EventBase {
        explicit MouseReleaseEvent(void) noexcept = default;

        virtual void onMouseRelease(
            MouseButton const& button) noexcept = 0;

        virtual ~MouseReleaseEvent(void) = default;
    };


}
