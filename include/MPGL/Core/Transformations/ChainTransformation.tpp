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

#include <functional>

namespace mpgl {

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... Args>
    ChainTransformation<Dim>::ChainTransformation(
        Args&&... args) noexcept
            : storage{new AgregatedTransformation{
                std::forward<Args>(args)...}} {}

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
                Agregated{std::forward<TArgs>(transformations)...})} {}

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
    template <size_t Index, size_t... Indices>
    constexpr decltype(auto)
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            findFusionIndices(std::index_sequence<Indices...> indices)
    {
        if constexpr (Index == sizeof...(TArgs)) {
            return indices;
        } else {
            using CurrentTp = std::remove_cvref_t<decltype(
                std::get<Index>(std::declval<Agregated>()))>;
            using LastTp = std::remove_cvref_t<decltype(
                std::get<Index - 1>(std::declval<Agregated>()))>;

            return findFusionIndicesHelper<Index, CurrentTp, LastTp>(
                std::move(indices));
        }
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    template <
        size_t Index,
        class CurrentTp,
        class LastTp,
        size_t... Indices>
    constexpr decltype(auto)
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            findFusionIndicesHelper(
                std::index_sequence<Indices...> indices)
    {
        if constexpr (!InstanceOf<CurrentTp, Fusable>) {
            return findFusionIndices<Index + 1>(
                pushBack<Index>(indices));
        } else if constexpr (!IsFusable<LastTp,
            typename CurrentTp::FusionBase>::Value)
        {
            return findFusionIndices<Index + 1>(
                pushBack<Index>(indices));
        } else
            return findFusionIndices<Index + 1>(std::move(indices));
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    constexpr decltype(auto)
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            findFusionIndices(void)
    {
        return findFusionIndices<1>(std::index_sequence<0>{});
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
        }(FusionIndices{});
    }

    template <Dimension Dim>
    template <std::derived_from<Transformation<Dim>>... TArgs>
    void
        ChainTransformation<Dim>::AgregatedTransformation<TArgs...>::
            fuseInstructions(Agregated& tuple) noexcept
    {
        using FusionRange = decltype(pushBack<sizeof...(TArgs)>(
            std::declval<FusionIndices>()));

        std::apply([&tuple](auto&&... sequences){
            ([&tuple]<size_t F, size_t... I>(
                std::index_sequence<F, I...>)
            {
                (std::get<F>(tuple).fuse(std::get<I>(tuple)), ...);
            }(sequences), ...);
        }, SplitedIndexSequence<FusionRange>{});
    }

}
