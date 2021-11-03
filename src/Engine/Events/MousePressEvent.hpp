#pragma once

#include "../IO/Devices/Mouse.hpp"
#include "Event.hpp"

namespace ge {

    struct MousePressEvent : public EventBase {
        explicit MousePressEvent(void) noexcept = default;

        virtual void onMousePressEvent(MouseButton const& button) = 0;

        virtual ~MousePressEvent(void) noexcept = default;
    };

}
