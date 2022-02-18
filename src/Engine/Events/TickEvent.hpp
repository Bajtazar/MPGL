#pragma once

#include <chrono>

#include "Event.hpp"

namespace mpgl {

    struct TickEvent : public EventBase {
        explicit TickEvent(void) noexcept = default;

        virtual void onTick(
            std::chrono::milliseconds const& duration) noexcept = 0;

        virtual ~TickEvent(void) noexcept = default;
    };

}
