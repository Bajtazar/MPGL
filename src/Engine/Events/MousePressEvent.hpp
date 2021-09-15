#pragma once

#include "../IO/Devices/Mouse.hpp"

namespace ge {

    struct MousePressEvent {
        explicit MousePressEvent(void) noexcept = default;

        virtual void onMousePressEvent(MouseButton const& button) = 0;

        virtual ~MousePressEvent(void) noexcept = default;
    };

}
