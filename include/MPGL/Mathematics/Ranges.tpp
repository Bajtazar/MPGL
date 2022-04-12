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
