#pragma once

#include "../IO/Devices/Keyboard.hpp"
#include "Event.hpp"

namespace ge {

    struct KeyPressEvent : public EventBase {
        explicit KeyPressEvent(void) noexcept = default;

        virtual void onKeyPress(
            Key const& key) noexcept = 0;

        virtual ~KeyPressEvent(void) noexcept = default;
    };

}
