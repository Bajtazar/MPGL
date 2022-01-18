#pragma once

#include "Event.hpp"

namespace ge {

    struct WindowCloseEvent : public EventBase {
        explicit WindowCloseEvent(void) noexcept = default;

        virtual void onWindowClose(void) noexcept = 0;

        virtual ~WindowCloseEvent(void) noexcept = default;
    };

}
