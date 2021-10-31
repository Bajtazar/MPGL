#pragma once

#include <ranges>

namespace ge {

    template <std::size_t Size, std::ranges::random_access_range Range>
    struct FixedRange : public Range {
        constexpr explicit FixedRange(Range&& range) noexcept
            : Range{std::forward<Range>(range)} {}

        static constexpr std::size_t size(void) noexcept { return Size; }
    };

}
