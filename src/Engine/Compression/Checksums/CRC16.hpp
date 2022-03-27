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

#include "../../Traits/Concepts.hpp"

namespace mpgl {

    /**
     * Calculates the CRC16 checksum of the given range
     */
    class CRC16 {
    public:
        /**
         * Constructs a new CRC16 object
         */
        constexpr explicit CRC16(void) noexcept = default;

        /**
         * Calculates the checksum of the given range
         *
         * @tparam Range the range type
         * @param range the constant reference to the checked range
         * @return the crc16 checksum
         */
        template <std::ranges::input_range Range>
            requires ByteInputIterator<typename Range::iterator>
        [[nodiscard]] constexpr uint32 operator() (
            Range const& range) const noexcept;
    private:
        typedef std::array<uint16, 256>             LookupTable;

        /**
         * Generates a lookup table for crc 16
         *
         * @return the lookup table for crc 16
         */
        static constexpr LookupTable generateLookupTable(
            void) noexcept;

        static LookupTable const                    lookup;
    };

    inline constexpr CRC16                          crc16{};

    CRC16::LookupTable const CRC16::lookup = generateLookupTable();

    template <std::ranges::input_range Range>
        requires ByteInputIterator<typename Range::iterator>
    [[nodiscard]] constexpr uint32 CRC16::operator() (
        Range const& range) const noexcept
    {
        uint16 crc = 0xFFFF;
        for (uint8 value : range)
            crc = (crc << 8) ^ lookup[value ^ (crc >> 8)];
        return crc;
    }

    constexpr CRC16::LookupTable
        CRC16::generateLookupTable(void) noexcept
    {
        LookupTable table{};
        for (uint16 i = 1, crc = 0x8000; i < 256; i <<= 1) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
            for (uint8 j = 0; j < i; ++j)
                table[i + j] = crc ^ table[j];
        }
        return table;
    }

}
