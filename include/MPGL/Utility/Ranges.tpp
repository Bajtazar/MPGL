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

namespace mpgl {

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

    template <Clonable Tp>
    [[nodiscard]] constexpr auto CloneFn::operator() (
        Tp const& object) const
            -> decltype(std::declval<Tp>().clone())
    {
        return object.clone();
    }

    template <ClonablePointer Ptr>
    [[nodiscard]] constexpr auto CloneFn::operator() (
        Ptr const& pointer) const
            -> decltype(std::declval<Ptr>()->clone())
    {
        return bool(pointer) ? pointer->clone() : nullptr;
    }

}
