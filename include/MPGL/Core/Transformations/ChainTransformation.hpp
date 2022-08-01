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

#include <MPGL/Core/Transformations/Transformation.hpp>

#include <vector>

namespace mpgl {

    template <Dimension Dim>
    class ChainTransformation : public Transformation<Dim> {
    public:
        using TransformedType = Transformation<Dim>::TransformedType;

        template <std::derived_from<Transformation<Dim>>... Args>
        explicit ChainTransformation(Args&&... args);

        ChainTransformation(
            ChainTransformation const& transformation);

        ChainTransformation(
            ChainTransformation&&) noexcept = default;

        ChainTransformation& operator=(
            ChainTransformation const& transformation) noexcept;

        ChainTransformation& operator=(
            ChainTransformation&&) noexcept = default;

        void operator() (
           any::InputRange<TransformedType>& coords
           ) const noexcept final;

        void operator() (TransformedType& coord) const noexcept final;

        ~ChainTransformation(void) noexcept = default;
    private:
        class ClonableTransformation : public Transformation<Dim> {
        public:
            explicit ClonableTransformation(void) noexcept = default;

            virtual void operator() (
                any::InputRange<TransformedType>& coords
                ) const noexcept = 0;

            virtual void operator() (
                TransformedType& coord) const noexcept = 0;

            [[nodiscard]] virtual ClonableTransformation* clone(
                void) const = 0;

            virtual ~ClonableTransformation(void) noexcept = default;
        protected:
            ClonableTransformation(
                ClonableTransformation const&) noexcept = default;
            ClonableTransformation(
                ClonableTransformation&&) noexcept = default;

            ClonableTransformation& operator=(
                ClonableTransformation const&) noexcept = default;
            ClonableTransformation& operator=(
                ClonableTransformation&&) noexcept = default;
        };

        template <std::derived_from<Transformation<Dim>>... TArgs>
        class AgregatedTransformation :
            public ClonableTransformation
        {
        public:
            explicit AgregatedTransformation(
                TArgs&&... transformations) noexcept;

            AgregatedTransformation(
                AgregatedTransformation const&) noexcept = default;
            AgregatedTransformation(
                AgregatedTransformation&&) noexcept = default;

            AgregatedTransformation& operator=(
                AgregatedTransformation const&) noexcept = default;
            AgregatedTransformation& operator=(
                AgregatedTransformation&&) noexcept = default;

            void operator() (
                any::InputRange<TransformedType>& coords
                ) const noexcept final;

            void operator() (
                TransformedType& coord) const noexcept final;

            [[nodiscard]] ClonableTransformation* clone(
                void) const final;

            ~AgregatedTransformation(void) noexcept = default;
        private:
            using Agregated = std::tuple<TArgs...>;

            template <size_t Index, size_t... Indexes>
            constexpr static decltype(auto) findFusionIndexes(
                std::index_sequence<Indexes>... indexes);

            constexpr static decltype(auto) findFusionIndexes(void);

            static decltype(auto) constructFusedTuple(
                Agregated&& tuple) noexcept;

            using FusionIndexes = decltype(findFusionIndexes());
            using FusedTuple = decltype(constructFusedTuple(
                std::declval<Agregated>()));

            FusedTuple                              transformations;
        };

        std::unique_ptr<ClonableTransformation>     storage;
    };

}
