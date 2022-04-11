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

#include <array>

namespace mpgl {

    /**
     * Calculates the CRC32 checksum of the given range
     */
    class CRC32 {
    public:
        /**
         * Constructs a new CRC32 object
         */
        constexpr explicit CRC32(void) noexcept = default;

        /**
         * Calculates the checksum of the given range
         *
         * @tparam Range the range type
         * @param range the constant reference to the checked range
         * @return the crc32 checksum
         */
        template <std::ranges::input_range Range>
            requires ByteInputIterator<std::ranges::iterator_t<Range>>
        [[nodiscard]] constexpr uint32 operator() (
            Range const& range) const noexcept;
    private:
        typedef std::array<uint32, 256>             LookupTable;

        /**
         * Generates a lookup table for crc 32
         *
         * @return the lookup table for crc 32
         */
        static constexpr LookupTable generateLookupTable(
            void) noexcept;

        static LookupTable const                    lookup;
    };

    inline constexpr CRC32                          crc32{};

}

#include <MPGL/Compression/Checksums/CRC32.tpp>
#include <MPGL/Compression/Checksums/CRC32.ipp>
