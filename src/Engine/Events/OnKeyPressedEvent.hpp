#pragma once'

#include "../IO/Devices/Keyboard.hpp"

namespace ge {

    struct OnKeyPressedEvent {
        explicit OnKeyPressedEvent(void) noexcept = default;

        virtual void onKeyPressedEvent(Key const& key) = 0;

        virtual ~OnKeyPressedEvent(void) noexcept = default;
    };

}
