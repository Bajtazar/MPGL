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

#include <MPGL/Traits/IndexSequence.hpp>

namespace mpgl {

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... Args>
    ChainTransformation<Dim>::ChainTransformation(
        Args&&... args) noexcept

    {}

    template <Dimension Dim>
    ChainTransformation<Dim>::ChainTransformation(
        ChainTransformation const& transformation)
            : storage{transformation.storage->clone()} {}

    template <Dimension Dim>
    ChainTransformation<Dim>& ChainTransformation<Dim>::operator=(
        ChainTransformation const& transformation)
    {
        storage = Storage{transformation.storage->clone()};
        return *this;
    }

    template <Dimension Dim>
    void ChainTransformation<Dim>::operator() (
        any::InputRange<TransformedType>& coords) const noexcept
    {
        (*storage)(coords);
    }

    template <Dimension Dim>
    void ChainTransformation<Dim>::operator() (
        TransformedType& coord) const noexcept
    {
        (*storage)(coord);
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
        AgregatedTransformation(TArgs&&... transformations) noexcept
            : transformations{constructFusedTuple(
                Agregated{std::forward(transformations)...})} {}

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    void ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
        operator() (any::InputRange<TransformedType>& coords
            ) const noexcept
    {
        std::apply([&coords](auto&... trans) { (trans(coords), ...); },
            transformations);
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    void ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
        operator() (TransformedType& coord) const noexcept
    {
        std::apply([&coord](auto&... trans) { (trans(coord), ...); },
            transformations);
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    [[nodiscard]] ChainTransformation<Dim>::ClonableTransformation*
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            clone(void) const
    {
        return new AgregatedTransformation{*this};
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    template <size_t Index, size_t... Indexes>
    constexpr decltype(auto)
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            findFusionIndexes(std::index_sequence<Indexes...> indexes)
    {
        if constexpr (Index == sizeof...(TArgs)) {
            return indexes;
        } else {
            using CurrentTp = std::remove_cvref_t<decltype(
                std::get<Index>(std::declval<Agregated>()))>;
            using LastTp = std::remove_cvref_t<decltype(
                std::get<Index - 1>(std::declval<Agregated>()))>;

            return findFusionIndexesHelper<Index, CurrentTp, LastTp>(
                std::move(indexes));
        }
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    template <
        size_t Index,
        class CurrentTp,
        class LastTp,
        size_t... Indexes>
    constexpr decltype(auto)
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            findFusionIndexesHelper(
                std::index_sequence<Indexes...> indexes)
    {
        if constexpr (!InstanceOf<CurrentTp, Fusable>) {
            return fuzeIndexes<Index + 1>(
                addToIndexSequence<Index>(indexes));
        } else if constexpr (!IsFusable<LastTp,
            typename CurrentTp::FusionBase>::Value)
        {
            return fuzeIndexes<Index + 1>(
                addToIndexSequence<Index>(indexes));
        } else
            return fuzeIndexes<Index + 1>(std::move(indexes));
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    constexpr decltype(auto)
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            findFusionIndexes(void)
    {
        return findFusionIndexes<1>(std::make_index_sequence<1>{});
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    decltype(auto)
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            constructFusedTuple(Agregated&& tuple) noexcept
    {
        fuseInstructions(tuple);
        return [&tuple]<size_t... I>(std::index_sequence<I...>) {
            return std::tuple{ std::get<I>(tuple)... };
        }(FusionIndexes{});
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    void
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            fuseInstructions(Agregated& tuple) noexcept
    {
        std::apply([&tuple]<size_t... I>(std::index_sequence<I...> seq)
        {
            if constexpr (sizeof...(I)) {
                [&tuple]<size_t First, size_t... Index>(
                    std::index_sequence<First, Index...>)
                {
                    (std::get<First>(tuple).fuse(
                        std::get<Index>(tuple)), ...);
                }(std::move(seq));
            }
        }, SplitedIndexSequence<FusionIndexes>{});
    }

}
