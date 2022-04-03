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

#include <ranges>
#include <iterator>
#include <functional>

#include "../Traits/Concepts.hpp"

namespace mpgl {

    /**
     * The range version of std::accumulate algorithm
     */
    struct AccumulateFn {

        /**
         * Adds the element of the given range together using
         * the given projection
         *
         * @tparam Iter the iterator type
         * @tparam Sent the sentinel type
         * @tparam Init the sum type
         * @tparam Proj the projection type
         * @param iter the range's iterator
         * @param sent the range's sentinel
         * @param init the initial value
         * @param proj the projection functor
         * @return the result of an operation
         */
        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent,
            Addable Init, typename Proj = std::identity>
                requires std::convertible_to<std::invoke_result_t<
                    Proj, std::iter_value_t<Iter>>, Init>
        [[nodiscard]] constexpr Init operator() (
            Iter iter,
            Sent sent,
            Init init,
            Proj proj = {}) const noexcept;

        /**
         * Performs the given predicate on the element of the
         * given range together using the given projection
         *
         * @tparam Iter the iterator type
         * @tparam Sent the sentinel type
         * @tparam Init the sum type
         * @tparam Proj the projection type
         * @tparam Pred the predicate type
         * @param iter the range's iterator
         * @param sent the range's sentinel
         * @param init the initial value
         * @param pred the predicate functor
         * @param proj the projection functor
         * @return the result of an operation
         */
        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent,
            typename Init, typename Proj = std::identity,
            std::indirect_binary_predicate<Init,
                std::projected<Iter, Proj>> Pred>
            requires (std::convertible_to<std::invoke_result_t<
                    Proj, std::iter_value_t<Iter>>, Init>
                && std::is_nothrow_invocable_v<Pred, Init,
                    std::projected<Iter, Proj>>)
        [[nodiscard]] constexpr Init operator() (
            Iter iter,
            Sent sent,
            Init init,
            Pred pred,
            Proj proj = {}) const noexcept;

        /**
         * Adds the element of the given range together using
         * the given projection
         *
         * @tparam Range the range type
         * @tparam Init the sum type
         * @tparam Proj the projection type
         * @param range the universal reference to the range object
         * @param init the initial value
         * @param proj the projection functor
         * @return the result of an operation
         */
        template <std::ranges::input_range Range,
            Addable Init, typename Proj = std::identity>
                requires std::convertible_to<std::invoke_result_t<
                    Proj, std::ranges::range_value_t<Range>>, Init>
        [[nodiscard]] constexpr Init operator() (
            Range&& range,
            Init init,
            Proj proj = {}) const noexcept;

        /**
         * Performs the given predicate on the element of the
         * given range together using the given projection
         *
         * @tparam Range the range type
         * @tparam Init the sum type
         * @tparam Pred the predicate type
         * @tparam Proj the projection type
         * @param range the universal reference to the range object
         * @param init the initial value
         * @param pred the predicate functor
         * @param proj the projection functor
         * @return the result of an operation
         */
        template <std::ranges::input_range Range,
            typename Init, typename Proj = std::identity,
            std::indirect_binary_predicate<Init,
                std::projected<std::ranges::iterator_t<Range>,
                    Proj>> Pred>
            requires (std::convertible_to<std::invoke_result_t<
                    Proj, std::ranges::range_value_t<Range>>, Init>
                && std::is_nothrow_invocable_v<Pred, Init,
                    std::projected<std::ranges::iterator_t<
                        Range>, Proj>>)
        [[nodiscard]] constexpr Init operator() (
            Range&& range,
            Init init,
            Pred pred,
            Proj proj = {}) const noexcept;

    };

    inline constexpr AccumulateFn                       accumulate{};

    /**
     * The structure representing the result of the find first
     * and last algorithm
     *
     * @tparam Tp the iterator type
     */
    template <typename Tp>
    struct FindFirstAndLastResult {
        [[no_unique_address]] Tp                        first;
        [[no_unique_address]] Tp                        last;

        /**
         * Casts the given structure to the given type
         *
         * @tparam Up the new iterator type
         * @return the casted structure
         */
        template <typename Up>
            requires std::convertible_to<Tp const&, Up>
        [[nodiscard]] constexpr operator
            FindFirstAndLastResult<Up>() const&
                { return {first, last}; }

        /**
         * Casts the given structure to the given type
         *
         * @tparam Up the new iterator type
         * @return the casted structure
         */
        template <typename Up>
            requires std::convertible_to<Tp, Up>
        [[nodiscard]] constexpr operator
            FindFirstAndLastResult<Up>() &&
                { return {std::move(first), std::move(last)}; }
    };

    /**
     * Finds the first and last occurance when the given predicate
     * is satisfied
     */
    struct FindFirstAndLastIfFn {

        /**
         * Finds the first and last occurance when the given predicate
         * is satisfied
         *
         * @tparam Iter the iterator type
         * @tparam Sent the sentinel type
         * @tparam Proj the projection type
         * @tparam Pred the predicate type
         * @param iter the range's iterator
         * @param sent the range's sentinel
         * @param pred the predicate functor
         * @param proj the projection functor
         * @return the first and last occurance
         */
        template <std::forward_iterator Iter,
            std::sentinel_for<Iter> Sent, typename Proj = std::identity,
            std::indirect_unary_predicate<std::projected<
                Iter, Proj>> Pred>
        [[nodiscard]] constexpr FindFirstAndLastResult<Iter>
            operator() (
                Iter iter,
                Sent sent,
                Pred pred,
                Proj proj = {}) const noexcept;

        /**
         * Finds the first and last occurance when the given predicate
         * is satisfied
         *
         * @tparam Range the range type
         * @tparam Pred the predicate type
         * @tparam Proj the projection type
         * @param range the universal reference to the range object
         * @param pred the predicate functor
         * @param proj the projection functor
         * @return the first and last occurance
         */
        template <std::ranges::forward_range Range,
            typename Proj = std::identity,
            std::indirect_unary_predicate<std::projected<
                std::ranges::iterator_t<Range>, Proj>> Pred>
        [[nodiscard]] constexpr FindFirstAndLastResult<
            std::ranges::borrowed_iterator_t<Range>> operator() (
                Range&& range,
                Pred pred,
                Proj proj = {}) const noexcept;

    };

    inline constexpr FindFirstAndLastIfFn findFirstAndLastIf;

    /**
     * Copies the elements to the given range until the range
     * is full
     */
    struct CopyToFn {

        /**
         * Copies the elements to the given range until the range
         * is full
         *
         * @tparam Iter the iterator type
         * @tparam Sent the sentinel type
         * @tparam TargetIter the target iterator type
         * @tparam Proj the projection type
         * @param iter the range's iterator
         * @param sent the range's sentinel
         * @param target the copied range iterator
         * @param proj the projection functor
         */
        template <std::incrementable Iter,
            std::sentinel_for<Iter> Sent,
            std::weakly_incrementable TargetIter,
            typename Proj = std::identity>
        constexpr void operator() (
            Iter iter,
            Sent sent,
            TargetIter target,
            Proj proj = {}) const noexcept;

        /**
         * Copies the elements to the given range until the range
         * is full
         *
         * @tparam Range the range type
         * @tparam TargetIter the target iterator type
         * @tparam Proj the projection type
         * @param range the universal reference to the range object
         * @param iter the copied range iterator
         * @param proj the projection functor
         */
        template <std::ranges::input_range Range,
            std::incrementable TargetIter,
            typename Proj = std::identity>
        constexpr void operator() (
            Range&& range,
            TargetIter iter,
            Proj proj = {}) const noexcept;

    };

    inline constexpr CopyToFn copyTo;

    /**
     * The range version of std::reverse algorithm
     */
    struct ReverseFn {

        /**
         * Reverses the elements order in the given range
         *
         * @tparam Iter the iterator type
         * @tparam Sent the sentinel type
         * @param iter the range's iterator
         * @param sent the range's sentinel
         */
        template <std::bidirectional_iterator Iter,
            std::sentinel_for<Iter> Sent>
        void operator() (Iter iter, Sent sent) const noexcept;

        /**
         * Reverses the elements order in the given range
         *
         * @tparam Range the range type
         * @param range the universal reference to the range object
         */
        template <std::ranges::bidirectional_range Range>
        void operator() (Range&& range) const noexcept;

    };

    inline ReverseFn reverse;

    template <std::input_iterator Iter,
        std::sentinel_for<Iter> Sent,
        Addable Init,
        typename Proj>
            requires std::convertible_to<std::invoke_result_t<
                Proj, std::iter_value_t<Iter>>, Init>
    [[nodiscard]] constexpr Init AccumulateFn::operator() (
        Iter iter,
        Sent sent,
        Init init,
        Proj proj) const noexcept
    {
        for (; iter != sent; ++iter)
            init = std::move(init) + std::invoke(proj, *iter);
        return init;
    }

    template <std::input_iterator Iter,
        std::sentinel_for<Iter> Sent,
        typename Init, typename Proj,
        std::indirect_binary_predicate<Init,
            std::projected<Iter, Proj>> Pred>
        requires (std::convertible_to<std::invoke_result_t<
                Proj, std::iter_value_t<Iter>>, Init>
            && std::is_nothrow_invocable_v<Pred, Init,
                std::projected<Iter, Proj>>)
    [[nodiscard]] constexpr Init AccumulateFn::operator() (
        Iter iter,
        Sent sent,
        Init init,
        Pred pred,
        Proj proj) const noexcept
    {
        for (; iter != sent; ++iter)
            init = std::invoke(proj, std::move(init),
                std::invoke(proj, *iter));
        return init;
    }

    template <std::ranges::input_range Range,
        Addable Init, typename Proj>
            requires std::convertible_to<std::invoke_result_t<
                Proj, std::ranges::range_value_t<Range>>, Init>
    [[nodiscard]] constexpr Init AccumulateFn::operator() (
        Range&& range,
        Init init,
        Proj proj) const noexcept
    {
        return (*this)(std::ranges::begin(range),
            std::ranges::end(range), std::move(init), std::move(proj));
    }

    template <std::ranges::input_range Range,
        typename Init, typename Proj,
        std::indirect_binary_predicate<Init,
            std::projected<std::ranges::iterator_t<Range>,
                Proj>> Pred>
        requires (std::convertible_to<std::invoke_result_t<
                Proj, std::ranges::range_value_t<Range>>, Init>
            && std::is_nothrow_invocable_v<Pred, Init,
                std::projected<std::ranges::iterator_t<
                    Range>, Proj>>)
    [[nodiscard]] constexpr Init AccumulateFn::operator() (
        Range&& range,
        Init init,
        Pred pred,
        Proj proj) const noexcept
    {
        return (*this)(std::ranges::begin(range),
            std::ranges::end(range), std::move(init),
            std::move(pred), std::move(proj));
    }

    template <std::forward_iterator Iter,
        std::sentinel_for<Iter> Sent, typename Proj,
        std::indirect_unary_predicate<std::projected<
            Iter, Proj>> Pred>
    [[nodiscard]] constexpr FindFirstAndLastResult<Iter>
        FindFirstAndLastIfFn::operator() (
            Iter iter,
            Sent sent,
            Pred pred,
            Proj proj) const noexcept
    {
        auto first = iter, last = sent;
        if (iter == sent)
            return {first, last};
        for (; !std::invoke(pred, std::invoke(proj, *iter))
            && iter != sent; ++iter);
        first = iter;
        if (iter == sent)
            return {first, first};
        --sent;
        for (; !std::invoke(pred, std::invoke(proj, *sent))
            && iter != sent; --sent);
        last = iter != sent ? sent : first;
        return {first, last};
    }

    template <std::ranges::forward_range Range,
        typename Proj,
        std::indirect_unary_predicate<std::projected<
            std::ranges::iterator_t<Range>, Proj>> Pred>
    [[nodiscard]] constexpr FindFirstAndLastResult<
        std::ranges::borrowed_iterator_t<Range>>
    FindFirstAndLastIfFn::operator() (
        Range&& range,
        Pred pred,
        Proj proj) const noexcept
    {
        return (*this)(std::ranges::begin(range),
            std::ranges::end(range), std::move(pred),
            std::move(proj));
    }

    template <std::incrementable Iter,
        std::sentinel_for<Iter> Sent,
        std::weakly_incrementable TargetIter, typename Proj>
    constexpr void CopyToFn::operator() (
        Iter iter,
        Sent sent,
        TargetIter target,
        Proj proj) const noexcept
    {
        for (; iter != sent; ++iter, ++target)
            *target = std::invoke(proj, *iter);
    }

    template <std::ranges::input_range Range,
        std::incrementable TargetIter,
        typename Proj>
    constexpr void CopyToFn::operator() (
        Range&& range,
        TargetIter iter,
        Proj proj) const noexcept
    {
        (*this)(std::ranges::begin(range),
            std::ranges::end(range), std::move(iter), std::move(proj));
    }

    template <std::bidirectional_iterator Iter,
        std::sentinel_for<Iter> Sent>
    void ReverseFn::operator() (Iter iter, Sent sent) const noexcept {
        if (iter == sent)
            return;
        --sent;
        for (; iter < sent; ++iter, --sent)
            std::iter_swap(iter, sent);
    }

    template <std::ranges::bidirectional_range Range>
    void ReverseFn::operator() (Range&& range) const noexcept {
        (*this)(std::ranges::begin(range), std::ranges::end(range));
    }

}
