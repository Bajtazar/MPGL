#pragma once

#include <inttypes.h>

namespace ge {

    struct OnClickEvent {
        explicit OnClickEvent(void) noexcept = default;

        virtual void onClickEvent(uint8_t key) = 0;

        virtual ~OnClickEvent(void) noexcept = default;
    };

}
