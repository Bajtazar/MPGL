#pragma once

#include "../IO/Devices/Keyboard.hpp"

namespace ge {

    struct KeyReleaseEvent {
        explicit KeyReleaseEvent(void) noexcept = default;

        virtual void onKeyReleaseEvent(void) noexcept = 0;

        virtual ~KeyReleaseEvent(void) noexcept = default;
    };

}
