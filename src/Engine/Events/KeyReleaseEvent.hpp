#pragma once

#include "../IO/Devices/Keyboard.hpp"
#include "Event.hpp"

namespace ge {

    struct KeyReleaseEvent : public EventBase {
        explicit KeyReleaseEvent(void) noexcept = default;

        virtual void onKeyRelease(
            Key const& key) noexcept = 0;

        virtual ~KeyReleaseEvent(void) noexcept = default;
    };

}
