#pragma once

#include <ranges>

namespace ge {

    template <std::size_t Size,
        std::ranges::random_access_range Range>
    struct FixedRange : public Range {
        using Range::Range;

        static constexpr std::size_t size(void) noexcept
            { return Size; }
    };

}
