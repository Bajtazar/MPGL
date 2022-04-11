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

    template <typename Tp, std::predicate<Tp, Tp> Compare,
        std::ranges::forward_range Sequence>
    void MinHeap<Tp, Compare, Sequence>::push(
        const_reference element) requires std::copyable<value_type>
    {
        sequence.push_back(element);
        std::ranges::push_heap(sequence, compare);
    }

    template <typename Tp, std::predicate<Tp, Tp> Compare,
        std::ranges::forward_range Sequence>
    void MinHeap<Tp, Compare, Sequence>::push(
        value_type&& element) requires std::movable<value_type>
    {
        sequence.push_back(std::move(element));
        std::ranges::push_heap(sequence, compare);
    }

    template <typename Tp, std::predicate<Tp, Tp> Compare,
        std::ranges::forward_range Sequence>
    template <typename... Args>
    void MinHeap<Tp, Compare, Sequence>::emplace(
        Args&&... args) requires (
            std::constructible_from<value_type, Args...>)
    {
        sequence.emplace_back(std::forward<Args>(args)...);
        std::ranges::push_heap(sequence, compare);
    }

    template <typename Tp, std::predicate<Tp, Tp> Compare,
        std::ranges::forward_range Sequence>
    [[nodiscard]] MinHeap<Tp, Compare, Sequence>::value_type
        MinHeap<Tp, Compare, Sequence>::popBack(
            void) requires std::movable<value_type>
    {
        std::ranges::pop_heap(sequence, compare);
        auto result = std::move(sequence.back());
        sequence.pop_back();
        return result;
    }

}
