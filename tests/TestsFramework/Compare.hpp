#pragma once

#include <algorithm>
#include <concepts>

namespace ge::tests {

    template <typename Tp>
    constexpr bool compare(Tp&& t1, Tp&& t2) noexcept
        requires requires (Tp t1) { { t1 == t2 } -> std::same_as<bool>; }
    {
        return t1 == t2;
    }

    template <std::ranges::input_range Tp, std::ranges::input_range Up>
    constexpr bool compare(Tp&& t1, Up&& t2) noexcept {
        return std::ranges::equal(std::forward<Tp>(t1),
            std::forward<Up>(t2));
    }

}
