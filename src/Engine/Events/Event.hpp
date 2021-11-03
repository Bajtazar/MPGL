#pragma once

#include <concepts>

namespace ge {

    struct EventBase {};

    template <typename T>
    concept Event = std::derived_from<T, EventBase>;

}
