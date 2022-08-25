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
     * Adds the given number to the back of index sequence
     *
     * @tparam I the inserted number
     * @tparam Indices the index sequence's indices
     * @return the index sequence with inserted number
     */
    template <size_t I, size_t... Indices>
    [[nodiscard]] consteval std::index_sequence<Indices..., I>
        pushBack(std::index_sequence<Indices...>) noexcept;

    /**
     * Adds the given number to the front of index sequence
     *
     * @tparam I the inserted number
     * @tparam Indices the index sequence's indices
     * @return the index sequence with inserted number
     */
    template <size_t I, size_t... Indices>
    [[nodiscard]] consteval std::index_sequence<I, Indices...>
        pushFront(std::index_sequence<Indices...>) noexcept;

    /**
     * Produces index sequence that holds only indices between
     * the Begin index and the End index (excluding End)
     *
     * @tparam Index the current index [begin or continuation of
     * recursive call]
     * @tparam End the end index
     * @tparam Indices the current index sequence's indices
     * @param indices the current index sequence
     * @return the range index sequence
     */
    template <size_t Index, size_t End, size_t... Indices>
    [[nodiscard]] consteval decltype(auto) indexSequenceRange(
        std::index_sequence<Indices...> indices) noexcept;

    /**
     * Produces index sequence that holds only indices between
     * the Begin index and the End index (excluding End)
     *
     * @tparam Begin first index
     * @tparam End first index after last
     */
    template <size_t Begin, size_t End>
    using makeIndexSequenceRange = decltype(
        indexSequenceRange<Begin + 1, End>(
            std::index_sequence<Begin>{}));

    /**
     * Splits sequence between the ranges and returns a tuple
     * containing them
     *
     * @tparam Indices the index sequence's indices
     * @param indices the index sequence
     * @return a tuple containing subranges
     */
    template <size_t... Indices>
        requires (sizeof...(Indices) > 0)
    [[nodiscard]] consteval decltype(auto) splitIndexSequence(
        std::index_sequence<Indices...> indices) noexcept;

    /**
     * Splits sequence between the ranges and returns a tuple
     * containing them
     */
    template <class IndexSequence>
        requires requires(IndexSequence seq) {
            splitIndexSequence(seq); }
    using SplitedIndexSequence = decltype(
        splitIndexSequence(std::declval<IndexSequence>()));

    /**
     * Helper function. Acquires the given index from the index
     * sequence
     *
     * @tparam I the current recursive call number
     * @tparam N the index of searched index
     * @tparam C currenlty checked index
     * @tparam Index the rest of indiecies
     * @return the N-th index value
     */
    template <size_t I, size_t N, size_t C, size_t... Index>
    [[nodiscard]] consteval size_t getFn(
        std::index_sequence<C, Index...>);

    /**
     * Acquires the N-th index value from the index sequence
     *
     * @tparam I the looked index's index
     * @tparam Index the rest of indiecies
     * @return the N-th index value
     */
    template <size_t I, size_t... Index>
        requires (sizeof...(Index) > I && sizeof...(Index) > 0)
    [[nodiscard]] consteval size_t get(
        std::index_sequence<Index...> sequence);

}

#include <MPGL/Traits/IndexSequence.tpp>
