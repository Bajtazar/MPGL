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
     * Scales the coordinates by the given vector of factors
     *
     * @tparam Dim the transformed space's dimensions
     */
    template <Dimension Dim>
    class Scaling :
        public Transformation<Dim>,
        public Fusable<Scaling<Dim>>
    {
    public:
        using VectorDf = Vector<float32, Dim::orthogonal_space_degree>;
        using TransformedType = Transformation<Dim>::TransformedType;

        /**
         * Constructs a new Scaling object
         *
         * @param factor the constant reference to the scaling
         * factors vector
         */
        Scaling(VectorDf const& factor) noexcept;

        /**
         * Constructs a new Scaling object
         *
         * @param factor the rvalue reference to the scaling
         * factors vector
         */
        Scaling(VectorDf&& factor) noexcept;

        Scaling(Scaling const&) noexcept = default;
        Scaling(Scaling&&) noexcept = default;

        Scaling& operator=(
            Scaling const&) noexcept = default;
        Scaling& operator=(
            Scaling&&) noexcept = default;

        /**
         * Perfoms scaling with the inner scaling center and factor
         * on the input range of coordinates
         *
         * @param coords the reference to the input range
         * of coordinates
         */
        void operator() (
            any::InputRange<TransformedType>& coords
            ) const noexcept final;

        /**
         * Perfoms scaling with the inner scaling center and factor
         * on the coordinate
         *
         * @param coord the reference to the coordinate object
         */
        void operator() (
            TransformedType& coord) const noexcept final;

        /**
         * Combines two scaling operations into one
         *
         * @param other the constant reference to the other
         * scaling transformation object
         */
        void fuse(Scaling const& other) noexcept final;

        /**
         * Returns a reference to the scaling factor
         *
         * @return the reference to the scaling factor
         */
        [[nodiscard]] float32& getFactor(void) noexcept
            { return factor; }

        /**
         * Returns a constant reference to the scaling factor
         *
         * @return the constant reference to the scaling factor
         */
        [[nodiscard]] float32 const& getFactor(
            void) const noexcept
                { return factor; }

        /**
         * Destroys a Scaling object
         */
        ~Scaling(void) noexcept = default;
    private:
        VectorDf                                    factor;
    };

    typedef Scaling<dim::Dim2>                      Scaling2D;
    typedef Scaling<dim::Dim3>                      Scaling3D;

}

#include <MPGL/Core/Transformations/Scaling.tpp>
