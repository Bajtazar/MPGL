/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include <MPGL/Traits/Concepts.hpp>

namespace mpgl {

    /**
     * Calculates the adler32 checksum of the given range
     */
    class Adler32 {
    public:
        /**
         * Constructs a new Adler32 object
         */
        constexpr explicit Adler32(void) noexcept = default;

        /**
         * Calculates the checksum of the given range
         *
         * @tparam Range the range type
         * @param range the constant reference to the checked range
         * @return the adler32 checksum
         */
        template <std::ranges::input_range Range>
            requires ByteInputIterator<std::ranges::iterator_t<Range>>
        [[nodiscard]] constexpr uint32 operator() (
            Range const& range) const noexcept;
    private:
        static constexpr const uint32 AdlerBase   = 65521;
    };

    inline constexpr Adler32                        adler32{};

    template <std::ranges::input_range Range>
        requires ByteInputIterator<std::ranges::iterator_t<Range>>
    [[nodiscard]] constexpr uint32 Adler32::operator() (
       Range const& range) const noexcept
    {
        uint32 low = 1, high = 0;
        for (uint8 value : range) {
            low = (low + value) % AdlerBase;
            high = (high + low) % AdlerBase;
        }
        return (high << 16) | low;
    }

}
