#pragma once

#include "Event.hpp"

namespace mpgl {

    struct WindowCloseEvent : public EventBase {
        explicit WindowCloseEvent(void) noexcept = default;

        virtual void onWindowClose(void) noexcept = 0;

        virtual ~WindowCloseEvent(void) noexcept = default;
    };

}
