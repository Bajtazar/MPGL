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

#include <algorithm>
#include <array>

namespace mpgl {

    /**
     * Allows to pass string as a template parameter
     *
     * @tparam Size the size of the string
     */
    template <std::size_t Size>
    struct TemplateString {
        /**
         * Constructs Template String object from a c-like char
         * array
         *
         * @param cstring reference to c-like char array
         */
        constexpr TemplateString(const char (&cstring)[Size])
            { std::copy_n(cstring, Size, string.begin()); }

        /**
         * Returns whether two template strings are equal
         *
         * @tparam S1 the left string length
         * @tparam S2 the right string length
         * @param leftString the left string object
         * @param rightString the right strig object
         * @return whether template strings are equal
         */
        template <std::size_t S1, std::size_t S2>
        friend constexpr bool operator==(
            TemplateString<S1> const& leftString,
            TemplateString<S2> const& rightString) noexcept;

        // The string array
        std::array<char, Size>                      string;
    };

    template <std::size_t S1, std::size_t S2>
    constexpr bool operator==(
        TemplateString<S1> const& leftString,
        TemplateString<S2> const& rightString) noexcept
    {
        if constexpr (S1 != S2)
            return false;
        for (std::size_t i = 0; i != S1; ++i)
            if (leftString.string[i] != rightString.string[i])
                return false;
        return true;
    }

}
