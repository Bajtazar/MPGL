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

namespace mpgl {

    /**
     * Scales the coordinates with the given center of the
     * transformation and the scaling factor
     *
     * @tparam Dim the transformed space's dimensions
     */
    template <Dimension Dim>
    class Scaling : public Transformation<Dim> {
    public:
        using VectorDf = Vector<float32, Dim::orthogonal_space_degree>;
        using TransformedType = Transformation<Dim>::TransformedType;

        /**
         * Constructs a new Scaling object
         *
         * @param center the constant reference to the
         * scaling center
         * @param factor the scaling factor
         */
        constexpr Scaling(
            VectorDf const& center,
            float32 factor) noexcept;

        /**
         * Constructs a new Scaling object
         *
         * @param center the rvalue reference to the
         * scaling center
         * @param factor the scaling factor
         */
        constexpr Scaling(
            VectorDf&& center,
            float32 factor) noexcept;

        constexpr Scaling(Scaling const&) noexcept = default;
        constexpr Scaling(Scaling&&) noexcept = default;

        constexpr Scaling& operator=(
            Scaling const&) noexcept = default;
        constexpr Scaling& operator=(
            Scaling&&) noexcept = default;

        /**
         * Perfoms scaling with the inner scaling center and factor
         * on the input range of coordinates
         *
         * @param coords the reference to the input range
         * of coordinates
         */
        constexpr void operator() (
            any::InputRange<TransformedType>& coords
            ) const noexcept final;

        /**
         * Perfoms scaling with the inner scaling center and factor
         * on the coordinate
         *
         * @param coord the reference to the coordinate object
         */
        constexpr void operator() (
            TransformedType& coord) const noexcept final;

        /**
         * Returns a reference to the scaling center
         *
         * @return the reference to the scaling center
         */
        constexpr [[nodiscard]] VectorDf& getCenter(void) noexcept
            { return center; }

        /**
         * Returns a constant reference to the scaling center
         *
         * @return the constant reference to the scaling center
         */
        constexpr [[nodiscard]] VectorDf const& getCenter(
            void) const noexcept
                { return center; }

        /**
         * Returns a reference to the scaling factor
         *
         * @return the reference to the scaling factor
         */
        constexpr [[nodiscard]] float32& getFactor(void) noexcept
            { return factor; }

        /**
         * Returns a constant reference to the scaling factor
         *
         * @return the constant reference to the scaling factor
         */
        constexpr [[nodiscard]] float32 const& getFactor(
            void) const noexcept
                { return factor; }

        /**
         * Destroys a Scaling object
         */
        constexpr ~Scaling(void) noexcept = default;
    private:
        VectorDf                                    center;
        float32                                     factor;
    };

}

#include <MPGL/Core/Transformations/Scaling.tpp>
