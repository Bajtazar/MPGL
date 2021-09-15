#pragma once

#include "../IO/Devices/Keyboard.hpp"

namespace ge {

    struct KeyPressEvent {
        explicit KeyPressEvent(void) noexcept = default;

        virtual void onKeyPressEvent(Key const& key) = 0;

        virtual ~KeyPressEvent(void) noexcept = default;
    };

}
