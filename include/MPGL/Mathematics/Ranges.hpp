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

#include <math.h>

#include <MPGL/Traits/Concepts.hpp>

namespace mpgl {

    /**
     * Calculates the norm of the given range
     */
    struct NormFn {

        struct PowerInvocable {

            /**
             * Predicate that raises the given value to the
             * given power
             *
             * @tparam Tp the value's type
             * @tparam Up the power's type
             * @param value the value
             * @param power the power
             * @return the value raised to the power
             */
            template <FloatConvertible Tp, FloatConvertible Up>
            [[nodiscard]] constexpr Tp operator() (
                Tp const& value,
                Up const& power) const noexcept;

        };

        /**
         * Calculates the given norm of the given range
         *
         * @tparam Iter the iterator type
         * @tparam Sent the sentinel type
         * @tparam Pred the predicate type
         * @param first the iterator to the begin of the range
         * @param last the sentinel to the end of the range
         * @param norm the norm degree
         * @param pred the predicate
         * @return the value of the given norm
         */
        template <std::incrementable Iter,
            std::sentinel_for<Iter> Sent,
            std::invocable<std::iter_value_t<Iter>,
                float64> Pred = PowerInvocable>
                    requires Arithmetic<std::iter_value_t<Iter>>
        [[nodiscard]] constexpr std::iter_value_t<Iter>
            operator() (
                Iter first,
                Sent last,
                float64 norm = 2.,
                Pred pred = {}) const noexcept;

        /**
         * Calculates the given norm of the given range
         *
         * @tparam Range the range type
         * @tparam Pred the predicate type
         * @param range the universal reference to the range object
         * @param norm the norm degree
         * @param pred the predicate
         * @return the value of the given norm
         */
        template <std::ranges::input_range Range,
            std::invocable<std::ranges::range_value_t<Range>,
                float64> Pred = PowerInvocable>
                requires Arithmetic<std::ranges::range_value_t<Range>>
        [[nodiscard]] constexpr std::ranges::range_value_t<Range>
            operator() (
                Range&& range,
                float64 norm = 2.,
                Pred pred = {}) const noexcept;

    };

    inline constexpr NormFn norm{};

    /**
     * Returns the result of the type projection
     *
     * @tparam Iter the iterator type
     * @tparam Proj the projection type
     * @param iter the iterator
     * @param proj the projection
     * @return the result of projecting the iterator
     */
    template <typename Iter, typename Proj>
    [[nodiscard]] constexpr decltype(auto) project(
        Iter iter,
        Proj proj = {}) noexcept;

    /**
     * Checks whether the result of projection can be casted to
     * the value
     *
     * @tparam Proj the projection type
     * @tparam Iter the iterator type
     * @tparam Value the value type
     */
    template <typename Proj, typename Iter, typename Value>
    concept Convertible = std::convertible_to<
        decltype(project<Iter, Proj>(std::declval<Iter>())), Value>;

    /**
     * Calculates the inner product of two ranges
     */
    struct InnerProductFn {

        /**
         * Calculates the inner product of the given ranges
         *
         * @tparam LeftIter the left iterator type
         * @tparam LeftSent the left sentinel type
         * @tparam RightIter the right iterator type
         * @tparam Init the initial value type
         * @tparam LeftProj the left projection type
         * @tparam RightProj the right projection type
         * @param leftIter the left range iterator
         * @param leftSent the left range sentinel
         * @param rightIter the right range iterator
         * @param init the initial value
         * @param leftProj the left projection
         * @param rightProj the right projection
         * @return the value of the inner product
         */
        template <std::incrementable LeftIter,
            std::sentinel_for<LeftIter> LeftSent,
            std::weakly_incrementable RightIter,
            Arithmetic Init,
            Convertible<LeftIter, Init> LeftProj = std::identity,
            Convertible<RightIter, Init> RightProj = std::identity>
        [[nodiscard]] constexpr Init operator()(
            LeftIter leftIter,
            LeftSent leftSent,
            RightIter rightIter,
            Init init,
            LeftProj leftProj = {},
            RightProj rightProj = {}) const noexcept;

        /**
         * Calculates the inner product of the given ranges.
         * Allows to use predicates instead of default operations
         *
         * @tparam LeftIter the left iterator type
         * @tparam LeftSent the left sentinel type
         * @tparam RightIter the right iterator type
         * @tparam Init the initial value type
         * @tparam LeftProj the left projection type
         * @tparam RightProj the right projection type
         * @tparam RightPred the right predicate type
         * @tparam LeftPred the left predicate type
         * @param leftIter the left range iterator
         * @param leftSent the left range sentinel
         * @param rightIter the right range iterator
         * @param init the initial value
         * @param leftPred the predicate used as the replacement
         * of the multiplication
         * @param rightPred the predicate used as the replacement
         * of the sum
         * @param leftProj the left projection
         * @param rightProj the right projection
         * @return the value of the inner product
         */
        template <std::incrementable                    LeftIter,
            std::sentinel_for<LeftIter>                 LeftSent,
            std::weakly_incrementable                   RightIter,
            Arithmetic Init,
            Convertible<LeftIter, Init>                 LeftProj
                = std::identity,
            Convertible<RightIter, Init>                RightProj
                = std::identity,
            std::indirect_binary_predicate<
                std::projected<LeftIter, LeftProj>,
                std::projected<RightIter, RightProj>>   RightPred,
            std::indirect_binary_predicate<Init,
                decltype(RightPred{}())>                LeftPred>
        [[nodiscard]] constexpr Init operator()(
            LeftIter leftIter,
            LeftSent leftSent,
            RightIter rightIter,
            Init init,
            LeftPred leftPred,
            RightPred rightPred,
            LeftProj leftProj = {},
            RightProj rightProj = {}) const noexcept;

        /**
         * Calculates the inner product of the given ranges
         *
         * @tparam LeftRange the left range type
         * @tparam RightRange the right range type
         * @tparam Init the initial value type
         * @tparam LeftProj the left projection type
         * @tparam RightProj the right projection type
         * @param leftRange the universal reference to the left range
         * @param rightRange the universal reference to the right range
         * @param init the initial value
         * @param leftProj the left projection
         * @param rightProj the right projection
         * @return the value of the inner product
         */
        template <std::ranges::input_range              LeftRange,
            std::ranges::input_range                    RightRange,
            Arithmetic Init,
            Convertible<std::ranges::iterator_t<
                RightRange>, Init>                      RightProj
            = std::identity,
            Convertible<std::ranges::iterator_t<
                LeftRange>, Init>                       LeftProj
            = std::identity>
        [[nodiscard]] constexpr Init operator()(
            LeftRange&& leftRange,
            RightRange&& rightRange,
            Init init,
            LeftProj leftProj = {},
            RightProj rightProj = {}) const noexcept;

        /**
         * Calculates the inner product of the given ranges.
         * Allows to use predicates instead of default operations
         *
         * @tparam LeftRange the left range type
         * @tparam RightRange the right range type
         * @tparam Init the initial value type
         * @tparam LeftProj the left projection type
         * @tparam RightProj the right projection type
         * @tparam RightPred the right predicate type
         * @tparam LeftPred the left predicate type
         * @param leftRange the universal reference to the left range
         * @param rightRange the universal reference to the right range
         * @param init the initial value
         * @param leftPred the predicate used as the replacement
         * of the multiplication
         * @param rightPred the predicate used as the replacement
         * of the sum
         * @param leftProj the left projection
         * @param rightProj the right projection
         * @return the value of the inner product
         */
        template <std::ranges::input_range LeftRange,
            std::ranges::input_range RightRange,
            Arithmetic Init,
            Convertible<std::ranges::iterator_t<
                RightRange>, Init>                      RightProj
            = std::identity,
            Convertible<std::ranges::iterator_t<
                LeftRange>, Init>                       LeftProj
            = std::identity,
            std::indirect_binary_predicate<
                std::projected<std::ranges::iterator_t<
                LeftRange>, LeftProj>, std::projected<
                std::ranges::iterator_t<RightRange>,
                RightProj>>                             RightPred,
                std::indirect_binary_predicate<Init,
                decltype(RightPred{}())>                LeftPred>
        [[nodiscard]] constexpr Init operator()(
            LeftRange&& leftRange,
            RightRange&& rightRange,
            Init init,
            LeftPred leftPred,
            RightPred rightPred,
            LeftProj leftProj = {},
            RightProj rightProj = {}) const noexcept;

    };

    inline constexpr InnerProductFn innerProduct;

    template <FloatConvertible Tp, FloatConvertible Up>
    [[nodiscard]] constexpr Tp
        NormFn::PowerInvocable::operator() (
            Tp const& value,
            Up const& power) const noexcept
    {
        return std::pow(value, power);
    }

    template <std::incrementable Iter,
        std::sentinel_for<Iter> Sent,
        std::invocable<std::iter_value_t<Iter>,
            float64> Pred>
                requires Arithmetic<std::iter_value_t<Iter>>
    [[nodiscard]] constexpr std::iter_value_t<Iter>
        NormFn::operator() (
            Iter first,
            Sent last,
            float64 norm,
            Pred pred) const noexcept
    {
        std::iter_value_t<Iter> sum {};
        for (;first != last; ++first)
            sum += std::move(std::invoke(pred,
                std::move(*first), norm));
        return std::invoke(pred, sum, 1. / norm);
    }

    template <std::ranges::input_range Range,
        std::invocable<std::ranges::range_value_t<Range>,
            float64> Pred>
            requires Arithmetic<std::ranges::range_value_t<Range>>
    [[nodiscard]] constexpr std::ranges::range_value_t<Range>
        NormFn::operator() (
            Range&& range,
            float64 norm,
            Pred pred) const noexcept
    {
        return (*this)(std::ranges::begin(range),
            std::ranges::end(range), norm, std::move(pred));
    }

    template <typename Iter, typename Proj>
    [[nodiscard]] constexpr decltype(auto) project(
        Iter iter,
        Proj proj) noexcept
    {
        return std::invoke(proj, *iter);
    }

    template <std::incrementable LeftIter,
        std::sentinel_for<LeftIter> LeftSent,
        std::weakly_incrementable RightIter,
        Arithmetic Init,
        Convertible<LeftIter, Init> LeftProj,
        Convertible<RightIter, Init> RightProj>
    [[nodiscard]] constexpr Init InnerProductFn::operator()(
        LeftIter leftIter,
        LeftSent leftSent,
        RightIter rightIter,
        Init init,
        LeftProj leftProj,
        RightProj rightProj) const noexcept
    {
        for (; leftIter != leftSent; ++leftIter, ++rightIter)
            init = std::move(init) + std::invoke(leftProj,
                *leftIter) * std::invoke(rightProj, *rightIter);
        return init;
    }

    template <std::incrementable                    LeftIter,
        std::sentinel_for<LeftIter>                 LeftSent,
        std::weakly_incrementable                   RightIter,
        Arithmetic Init,
        Convertible<LeftIter, Init>                 LeftProj,
        Convertible<RightIter, Init>                RightProj,
        std::indirect_binary_predicate<
            std::projected<LeftIter, LeftProj>,
            std::projected<RightIter, RightProj>>   RightPred,
        std::indirect_binary_predicate<Init,
            decltype(RightPred{}())>                LeftPred>
    [[nodiscard]] constexpr Init InnerProductFn::operator()(
        LeftIter leftIter,
        LeftSent leftSent,
        RightIter rightIter,
        Init init,
        LeftPred leftPred,
        RightPred rightPred,
        LeftProj leftProj,
        RightProj rightProj) const noexcept
    {
        for (; leftIter != leftSent; ++leftIter, ++rightIter)
            init = std::invoke(leftPred, std::move(init),
                std::invoke(rightPred, std::invoke(leftProj, *leftIter),
                    std::invoke(rightProj, *rightIter)));
        return init;
    }

    template <std::ranges::input_range              LeftRange,
        std::ranges::input_range                    RightRange,
        Arithmetic Init,
        Convertible<std::ranges::iterator_t<
            RightRange>, Init>                      RightProj,
        Convertible<std::ranges::iterator_t<
            LeftRange>, Init>                       LeftProj>
    [[nodiscard]] constexpr Init InnerProductFn::operator()(
        LeftRange&& leftRange,
        RightRange&& rightRange,
        Init init,
        LeftProj leftProj,
        RightProj rightProj) const noexcept
    {
        return (*this)(std::ranges::begin(leftRange),
            std::ranges::end(leftRange),
            std::ranges::begin(rightRange),
            std::move(init), std::move(leftProj),
            std::move(rightProj));
    }

        template <std::ranges::input_range LeftRange,
            std::ranges::input_range RightRange,
            Arithmetic Init,
            Convertible<std::ranges::iterator_t<
                RightRange>, Init>                      RightProj,
            Convertible<std::ranges::iterator_t<
                LeftRange>, Init>                       LeftProj,
            std::indirect_binary_predicate<
                std::projected<std::ranges::iterator_t<
                LeftRange>, LeftProj>, std::projected<
                std::ranges::iterator_t<RightRange>,
                RightProj>>                             RightPred,
                std::indirect_binary_predicate<Init,
                decltype(RightPred{}())>                LeftPred>
        [[nodiscard]] constexpr Init InnerProductFn::operator()(
            LeftRange&& leftRange,
            RightRange&& rightRange,
            Init init,
            LeftPred leftPred,
            RightPred rightPred,
            LeftProj leftProj,
            RightProj rightProj) const noexcept
        {
            return (*this)(std::ranges::begin(leftRange),
                std::ranges::end(leftRange),
                std::ranges::begin(rightRange), std::move(init),
                std::move(leftPred), std::move(rightPred),
                std::move(leftProj), std::move(rightProj));
        }

}
