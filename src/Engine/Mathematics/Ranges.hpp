#pragma once

#include "../Core/Figures/Views.hpp"

#include <math.h>

namespace ge {

    template <typename T>
    concept Mathematical = std::is_arithmetic_v<T> && std::convertible_to<T, double>;

    struct NormFn {

        struct powerInvocable {

            template <Mathematical T, Mathematical U>
            constexpr T operator() (T const& value, U const& power) const noexcept {
                return std::pow(value, power);
            }

        };

        template <std::incrementable Iter, std::sentinel_for<Iter> Sent,
            std::invocable<std::iter_value_t<Iter>, double> Pred = powerInvocable>
                requires requires (std::iter_value_t<Iter> a) { a += a; }
        constexpr std::iter_value_t<Iter>
            operator() (Iter first, Sent last, double norm = 2., Pred pred = {}) const noexcept
        {
            std::iter_value_t<Iter> sum {};
            for (;first != last; ++first)
                sum += std::move(std::invoke(pred, std::move(*first), norm));
            return std::invoke(pred, sum, 1. / norm);
        }

        template <std::ranges::input_range Range,
            std::invocable<std::ranges::range_value_t<Range>, double> Pred = powerInvocable>
                requires requires (std::ranges::range_value_t<Range> a) { a += a; }
        constexpr std::ranges::range_value_t<Range>
            operator() (Range&& range, double norm = 2., Pred pred = {}) const noexcept
        {
            return (*this)(std::ranges::begin(range), std::ranges::end(range), norm, std::move(pred));
        }

    };

    inline constexpr NormFn norm{};

    template <typename Iter, typename Proj>
    constexpr decltype(auto) convert(Iter iter, Proj proj = {}) noexcept {
        return std::invoke(proj, *iter);
    }

    template <typename Iter, typename Proj, typename Value>
    concept Convertible = std::convertible_to<decltype(convert<Iter, Proj>(std::declval<Iter>())), Value>;

    struct InnerProductFn {

        template <std::incrementable LeftIter, std::sentinel_for<LeftIter> LeftSent,
            std::weakly_incrementable RightIter, typename Init, typename LeftProj = std::identity,
                typename RightProj = std::identity>
            requires Convertible<LeftIter, LeftProj, Init> && Convertible<RightIter, RightProj, Init>
                && requires (Init a) { a + a; a * a; }
        constexpr Init operator()(LeftIter leftIter, LeftSent leftSent, RightIter rightIter,
            Init init, LeftProj leftProj = {}, RightProj rightProj = {}) const noexcept
        {
            for (; leftIter != leftSent; ++leftIter, ++rightIter)
                init = std::move(init) + std::invoke(leftProj, *leftIter) * std::invoke(rightProj, *rightIter);
            return init;
        }

        template <std::incrementable LeftIter, std::sentinel_for<LeftIter> LeftSent,
                std::weakly_incrementable RightIter, typename Init, typename LeftProj = std::identity,
                typename RightProj = std::identity,
                std::indirect_binary_predicate<std::projected<LeftIter, LeftProj>,
                std::projected<RightIter, RightProj>> RightPred,
                std::indirect_binary_predicate<Init, decltype(RightPred{}())> LeftPred>
            requires Convertible<LeftIter, LeftProj, Init> && Convertible<RightIter, RightProj, Init>
                && requires (Init a) { a + a; a * a; }
        constexpr Init operator()(LeftIter leftIter, LeftSent leftSent, RightIter rightIter, Init init,
            LeftPred leftPred, RightPred rightPred, LeftProj leftProj = {}, RightProj rightProj = {}) const noexcept
        {
            for (; leftIter != leftSent; ++leftIter, ++rightIter)
                init = std::invoke(leftPred, std::move(init), std::invoke(rightPred, std::invoke(leftProj, *leftIter),
                    std::invoke(rightProj, *rightIter)));
            return init;
        }

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Init,
            typename RightProj = std::identity, typename LeftProj = std::identity>
                requires Convertible<std::ranges::iterator_t<LeftRange>, LeftProj, Init>
                    && Convertible<std::ranges::iterator_t<RightRange>, RightProj, Init>
                    && requires (Init a) { a + a; a * a; }
        constexpr Init operator()(LeftRange&& leftRange, RightRange&& rightRange, Init init,
            LeftProj leftProj = {}, RightProj rightProj = {}) const noexcept
        {
            return (*this)(std::ranges::begin(leftRange), std::ranges::end(leftRange),
                std::ranges::begin(rightRange), std::move(init), std::move(leftProj), std::move(rightProj));
        }

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Init,
            typename LeftProj = std::identity, typename RightProj = std::identity,
                std::indirect_binary_predicate<std::projected<std::ranges::iterator_t<LeftRange>, LeftProj>,
                std::projected<std::ranges::iterator_t<RightRange>, RightProj>> RightPred,
                std::indirect_binary_predicate<Init, decltype(RightPred{}())> LeftPred>
            requires Convertible<std::ranges::iterator_t<LeftRange>, LeftProj, Init>
                && Convertible<std::ranges::iterator_t<RightRange>, RightProj, Init>
                && requires (Init a) { a + a; a * a; }
        constexpr Init operator()(LeftRange&& leftRange, RightRange&& rightRange, Init init,
            LeftPred leftPred, RightPred rightPred, LeftProj leftProj = {}, RightProj rightProj = {}) const noexcept
        {
            return (*this)(std::ranges::begin(leftRange), std::ranges::end(leftRange),
                std::ranges::begin(rightRange), std::move(init), std::move(leftPred),
                std::move(rightPred), std::move(leftProj), std::move(rightProj));
        }

    };

    inline constexpr InnerProductFn innerProduct;

}
