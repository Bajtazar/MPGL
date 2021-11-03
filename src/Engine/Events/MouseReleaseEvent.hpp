#pragma once

#include "../IO/Devices/Mouse.hpp"
#include "Event.hpp"

namespace ge {

    struct MouseReleaseEvent : public EventBase {
        explicit MouseReleaseEvent(void) noexcept = default;

        virtual void onMouseReleaseEvent(MouseButton const& button) = 0;

        virtual ~MouseReleaseEvent(void) = default;
    };


}
