#pragma once

#include "../Mathematics/Vector.hpp"

namespace ge {

    struct ScrollEvent {
        explicit ScrollEvent(void) noexcept = default;

        virtual void onScrollEvent(Vector2f const& scroll) = 0;

        virtual ~ScrollEvent(void) noexcept = default;
    };

}
