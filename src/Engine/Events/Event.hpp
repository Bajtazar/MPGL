#pragma once

#include <concepts>

namespace ge {

    struct EventBase {
        virtual ~EventBase(void) noexcept = default;
    };

    template <typename T>
    concept Event = std::derived_from<T, EventBase>;

}
