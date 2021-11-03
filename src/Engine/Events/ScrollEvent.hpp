#pragma once

#include "../Mathematics/Vector.hpp"
#include "Event.hpp"

namespace ge {

    struct ScrollEvent : public EventBase {
        explicit ScrollEvent(void) noexcept = default;

        virtual void onScrollEvent(Vector2f const& scroll) = 0;

        virtual ~ScrollEvent(void) noexcept = default;
    };

}
