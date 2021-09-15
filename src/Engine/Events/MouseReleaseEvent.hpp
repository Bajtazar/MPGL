#pragma once

#include "../IO/Devices/Mouse.hpp"

namespace ge {

    struct MouseReleaseEvent {
        explicit MouseReleaseEvent(void) noexcept = default;

        virtual void onMouseReleaseEvent(MouseButton const& button) = 0;

        virtual ~MouseReleaseEvent(void) = default;
    };


}
