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
#include <MPGL/Core/Transformations/Fusable.hpp>

namespace mpgl {

    /**
     * Performs multiple transformations at once allowing compiler to
     * better optimize code and reduce number of internal operations
     * performed on the transformed object that are indepedent from
     * the transformator. Performs the instruction fusion on the
     * fusable transformations in order to reduce number of calls
     *
     * @tparam Dim the transformed space's dimensions
     */
    template <Dimension Dim>
    class ChainTransformation : public Transformation<Dim> {
    public:
        using TransformedType = Transformation<Dim>::TransformedType;

        /**
         * Constructs a new chain transformation object. Fuses
         * transformations that can be fused together
         *
         * @tparam Args the transformation types
         * @param args the universal reference to the transformation
         * objects
         */
        template <std::derived_from<Transformation<Dim>>... Args>
        explicit ChainTransformation(Args&&... args) noexcept;

        /**
         * Construct a new chain transformation object from
         * the other object
         *
         * @param transformation the constant reference to the other
         * object
         */
        ChainTransformation(
            ChainTransformation const& transformation);

        ChainTransformation(
            ChainTransformation&&) noexcept = default;

        /**
         * Assigns an other chain transformation object to this
         * object
         *
         * @param transformation the constant reference to the
         * other object
         * @return the reference to this object
         */
        ChainTransformation& operator=(
            ChainTransformation const& transformation);

        ChainTransformation& operator=(
            ChainTransformation&&) noexcept = default;

        /**
         * Performs the transformations stored inside the chain
         * transformation object on the input range of coordinates
         *
         * @param coords the reference to the input range
         * of coordinates
         */
        void operator() (
           any::InputRange<TransformedType>& coords
           ) const noexcept final;

        /**
         * Performs the transformations stored inside the chain
         * transformation object on the coordinate
         *
         * @param coord the reference to the coordinate object
         */
        void operator() (TransformedType& coord) const noexcept final;

        /**
         * Swaps two chain transfromation objects together
         *
         * @param other the reference to the other object
         */
        void swap(ChainTransformation& other) noexcept
            { storage.swap(other.storage); }

        /**
         * Destroys the chain transformation object
         */
        ~ChainTransformation(void) noexcept = default;
    private:
        /**
         * An interface for the agregated transformations class.
         * Allows to clone  agregated transformation objects thus
         * allows the chain transformation objects to be
         * copy-assignable and copy-constructible
         */
        class ClonableTransformation : public Transformation<Dim> {
        public:
            /**
             * Constructs a new clonable transformation object
             */
            explicit ClonableTransformation(void) noexcept = default;

            /**
             * Pure virtual method. Has to be overloaded. Perfoms
             * transformation on the input range of coordinates
             *
             * @param coords the reference to the input range
             * of coordinates
             */
            virtual void operator() (
                any::InputRange<TransformedType>& coords
                ) const noexcept = 0;

            /**
             * Pure virtual method. Has to be overloaded. Perfoms
             * transformation on the coodinate
             *
             * @param coord the reference to the coordinate object
             */
            virtual void operator() (
                TransformedType& coord) const noexcept = 0;

            /**
             * Pure virtual method. Has to be overloaded. Clones
             * the object and returns a pointer to the cloned object
             * casted to the clonable transformation pointer
             *
             * @return the pointer to the cloned object
             */
            [[nodiscard]] virtual ClonableTransformation* clone(
                void) const = 0;

            /**
             * Virtual destructor. Destroys the clonable
             * transformation object
             */
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

        /**
         * Agregates transformations, fuses them when it is possible
         * and stores them
         *
         * @tparam TArgs the transformation types
         */
        template <std::derived_from<Transformation<Dim>>... TArgs>
        class AgregatedTransformation :
            public ClonableTransformation
        {
        public:
            /**
             * Constructs a new agregated transformation object.
             * Fuses the instructions when it is possible
             *
             * @param transformations the univeral reference to
             * the transformation objects
             */
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

            /**
             * Performs the transformations stored inside the chain
             * transformation object on the input range of coordinates
             *
             * @param coords the reference to the input range
             * of coordinates
             */
            void operator() (
                any::InputRange<TransformedType>& coords
                ) const noexcept final;

            /**
             * Performs the transformations stored inside the chain
             * transformation object on the coordinate
             *
             * @param coord the reference to the coordinate object
             */
            void operator() (
                TransformedType& coord) const noexcept final;

            /**
             * Clones the object and returns a pointer to the cloned
             * object casted to the clonable transformation pointer
             *
             * @return the pointer to the cloned object
             */
            [[nodiscard]] ClonableTransformation* clone(
                void) const final;

            /**
             * Destroys the agregated transformation object
             */
            ~AgregatedTransformation(void) noexcept = default;
        private:
            using Agregated = std::tuple<TArgs...>;

            /**
             * A helper function used by the find fusion indices
             * method. Determines which function should be call
             * next based if it can be fused with the previous
             * one or not
             *
             * @tparam Index the current transformation index
             * @tparam CurrentTp the current transformation type
             * @tparam LastTp the last transformation type
             * @tparam Indices the allready gathered indices
             * @param indices the allready gathered indices
             * @return the index sequence with the indices of
             * transformations which will be present in the final
             * tuple. The rest will be fused with them
             */
            template <
                size_t Index,
                class CurrentTp,
                class LastTp,
                size_t... Indices>
            constexpr static decltype(auto) findFusionIndicesHelper(
                std::index_sequence<Indices...> indices);

            /**
             * Recursively searches for transformations which can
             * be fused with the previous ones
             *
             * @tparam Index the current transformation index
             * @tparam Indices the allready gathered indices
             * @param indices the allready gathered indices
             * @return the index sequence with the indices of
             * transformations which will be present in the final
             * tuple. The rest will be fused with them
             */
            template <size_t Index, size_t... Indices>
            constexpr static decltype(auto) findFusionIndices(
                std::index_sequence<Indices...> indices);

            /**
             * Searches for transformations that cannot be fused
             * together and saves their indices in the index
             * sequence
             *
             * @return the index sequence with the indices of
             * transformations which will be present in the final
             * tuple. The rest will be fused with them
             */
            constexpr static decltype(auto) findFusionIndices(void);

            /**
             * Checks whether the first type is fusable with the
             * send one
             *
             * @tparam Tp the first type
             * @tparam Up the second type
             */
            template <class Tp, class Up>
            struct IsFusable;

            /**
             * Checks whether the first type is fusable with the
             * send one based on the fusable base arguments
             *
             * @tparam Tp the first type
             * @tparam Args the fusable base argument types
             */
            template <class Tp, class... Args>
            struct IsFusable<Tp, Fusable<Args...>> {
                constexpr static bool Value = (std::same_as<
                    std::remove_cvref_t<Tp>, Args> || ...);
            };

            /**
             * Constructs a final tuple with fused transformations.
             * Looks for transformations which can be fused with
             * previous ones and fuses them together
             *
             * @param tuple the rvalue reference to the input
             * tuple of transformations
             * @return the final tuple with fused functions
             */
            static decltype(auto) constructFusedTuple(
                Agregated&& tuple) noexcept;

            /**
             * Looks for transformations which can be fused with
             * previous ones and fuses them in-place with the first
             * transformation from the fusable subset
             *
             * @param tuple the reference to the original
             * transformations tuple
             */
            static void fuseInstructions(Agregated& tuple) noexcept;

            using FusionIndices = decltype(findFusionIndices());
            using FusedTuple = decltype(constructFusedTuple(
                std::declval<Agregated>()));

            FusedTuple                              transformations;
        };

        using Storage = std::unique_ptr<ClonableTransformation>;

        Storage                                     storage;
    };

    template class ChainTransformation<dim::Dim2>;
    template class ChainTransformation<dim::Dim3>;

    using ChainTransformation2D = ChainTransformation<dim::Dim2>;
    using ChainTransformation3D = ChainTransformation<dim::Dim3>;

}

#include <MPGL/Core/Transformations/ChainTransformation.tpp>

namespace std {

    /**
     * Swaps two mpgl::ChainTransformation objects
     *
     * @tparam Dim the transformed space's dimensions
     * @param left the reference to the left object
     * @param right the reference to the right object
     */
    template <mpgl::Dimension Dim>
    void swap(
        mpgl::ChainTransformation<Dim>& left,
        mpgl::ChainTransformation<Dim>& right) noexcept
    {
        left.swap(right);
    }

}
