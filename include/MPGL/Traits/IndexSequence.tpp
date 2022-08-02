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

namespace mpgl {

    template <size_t I, size_t... Indexes>
    [[nodiscard]] consteval std::index_sequence<Indexes..., I>
        pushBack(std::index_sequence<Indexes...>) noexcept
            { return std::index_sequence<Indexes..., I>{}; }

    template <size_t I, size_t... Indexes>
    [[nodiscard]] consteval std::index_sequence<I, Indexes...>
        pushFront(std::index_sequence<Indexes...>) noexcept
            { return std::index_sequence<I, Indexes...>{}; }

    template <size_t Index, size_t End, size_t... Indexes>
    [[nodiscard]] consteval decltype(auto) indexSequenceRange(
        std::index_sequence<Indexes...> indexes) noexcept
    {
        if constexpr (Index == End)
            return indexes;
        else
            return indexSequenceRange<Index + 1, End>(
                pushBack<Index>(std::move(indexes)));
    }

    template <size_t... Indexes>
        requires (sizeof...(Indexes) > 0)
    [[nodiscard]] consteval decltype(auto) splitIndexSequence(
        std::index_sequence<Indexes...> indexes) noexcept
    {
        if constexpr (sizeof...(Indexes) == 1)
            return std::tuple{indexes};
        else
            return []<size_t... I>(std::index_sequence<I...>) {
                constexpr std::index_sequence<Indexes...> indexes;
                return std::tuple{ makeIndexSequenceRange<
                    get<I>(indexes), get<I+1>(indexes)>{}... };
            }(std::make_index_sequence<sizeof...(Indexes) - 1>{});
    }

    template <size_t I, size_t N, size_t C, size_t... Index>
    [[nodiscard]] consteval size_t getFn(
        std::index_sequence<C, Index...>)
    {
        if constexpr (I == N)
            return C;
        else
            return getFn<I + 1, N>(std::index_sequence<Index...>{});
    }

    template <size_t I, size_t... Index>
        requires (sizeof...(Index) > I && sizeof...(Index) > 0)
    [[nodiscard]] consteval size_t get(
        std::index_sequence<Index...> sequence)
    {
        return getFn<0, I>(sequence);
    }

}
