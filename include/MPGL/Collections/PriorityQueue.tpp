/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

namespace mpgl {

    template <typename Tp,
        std::predicate<Tp, Tp> Comparator,
        std::ranges::forward_range Sequence>
    void PriorityQueue<Tp, Comparator, Sequence>::push(
        const_reference element) requires std::copyable<Tp>
    {
        sequence.push_back(element);
        std::ranges::push_heap(sequence, comparator);
    }

    template <typename Tp,
        std::predicate<Tp, Tp> Comparator,
        std::ranges::forward_range Sequence>
    void PriorityQueue<Tp, Comparator, Sequence>::push(
        value_type&& element) requires std::movable<Tp>
    {
        sequence.push_back(std::move(element));
        std::ranges::push_heap(sequence, comparator);
    }

    template <typename Tp,
        std::predicate<Tp, Tp> Comparator,
        std::ranges::forward_range Sequence>
    template <typename... Args>
    void PriorityQueue<Tp, Comparator, Sequence>::emplace(
        Args&&... args) requires
            std::constructible_from<value_type, Args...>
    {
        sequence.emplace_back(std::forward<Args>(args)...);
        std::ranges::push_heap(sequence, comparator);
    }

    template <typename Tp,
        std::predicate<Tp, Tp> Comparator,
        std::ranges::forward_range Sequence>
    [[nodiscard]] PriorityQueue<Tp, Comparator, Sequence>::value_type
        PriorityQueue<Tp, Comparator, Sequence>::pop(
            void) requires std::movable<value_type>
    {
        std::ranges::pop_heap(sequence, comparator);
        auto&& result = std::move(sequence.back());
        sequence.pop_back();
        return result;
    }

}
