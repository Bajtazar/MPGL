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

#include <MPGL/Collections/Erasers/InputRange.hpp>
#include <MPGL/Utility/Adapter.hpp>
#include <MPGL/Core/Dimensions.hpp>

namespace mpgl {

    /**
     * An interface for all transformations. Transformation is a
     * class that performs any operation over the given coordinates.
     * Transformations can be performed only on the vectors with the
     * same dimensions wrapped with adapter class
     *
     * @tparam Dim the dimension's tag of the transformed vector
     */
    template <Dimension Dim>
    class Transformation {
    public:
        using TransformedType = Adapter<Dim::orthogonal_space_degree>;

        /**
         * Constructs a new Transformation object
         */
        explicit Transformation(void) noexcept = default;

        /**
         * Pure virtual method. Has to be overloaded. Perfoms
         * transformation on the input range of coordinates
         *
         * @param coords the reference to the input range
         * of coordinates
         */
        virtual void operator() (
           any::InputRange<TransformedType>& coords) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded. Perfoms
         * transformation on the coodinate
         *
         * @param coord the reference to the coordinate object
         */
        virtual void operator() (
            TransformedType& coord) const noexcept = 0;

        /**
         * Destroys the Transformation object
         */
        virtual ~Transformation(void) noexcept = default;
    protected:
        Transformation(
            Transformation const&) noexcept = default;
        Transformation(Transformation&&) noexcept = default;

        Transformation& operator=(
            Transformation const&) noexcept = default;
        Transformation& operator=(
            Transformation&&) noexcept = default;
    };

    typedef Transformation<dim::Dim2>           Transformation2D;
    typedef Transformation<dim::Dim3>           Transformation3D;

    template class any::InputRange<Adapter2D>;

}
