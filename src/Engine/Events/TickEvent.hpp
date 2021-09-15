#pragma once

#include <chrono>

namespace ge {

    struct TickEvent {
        explicit TickEvent(void) noexcept = default;

        virtual void onTick(std::chrono::milliseconds const& duration) = 0;

        virtual ~TickEvent(void) noexcept = default;
    };

}
