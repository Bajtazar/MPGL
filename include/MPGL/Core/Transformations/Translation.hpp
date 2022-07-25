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
     * Performs translation of the coordinates by the given
     * translation vector
     *
     * @tparam Dim the transformed space's dimensions
     */
    template <Dimension Dim>
    class Translation : public Transformation<Dim> {
    public:
        using VectorDf = Vector<float32, Dim::orthogonal_space_degree>;
        using TransformedType = Transformation<Dim>::TransformedType;

        /**
         * Constructs a new Translation object
         *
         * @param translation the constant reference to the
         * translation vector
         */
        constexpr Translation(VectorDf const& translation) noexcept;

        /**
         * Constructs a new Translation object
         *
         * @param translation the rvalue reference to the
         * translation vector
         */
        constexpr Translation(VectorDf&& translation) noexcept;

        constexpr Translation(Translation const&) noexcept = default;
        constexpr Translation(Translation&&) noexcept = default;

        constexpr Translation& operator=(
            Translation const&) noexcept = default;
        constexpr Translation& operator=(
            Translation&&) noexcept = default;

        /**
         * Perfoms translation by the inner translation vector on
         * the input range of coordinates
         *
         * @param coords the reference to the input range
         * of coordinates
         */
        constexpr void operator() (
            any::InputRange<TransformedType>& coords
            ) const noexcept final;

        /**
         * Perfoms translation by the inner translation vector on
         * the coordinate
         *
         * @param coord the reference to the coordinate object
         */
        constexpr void operator() (
            TransformedType& coord) const noexcept final;

        /**
         * Returns a reference to the translation vector
         *
         * @return the reference to the translation vector
         */
        constexpr [[nodiscard]] VectorDf& getTranslation(void) noexcept
            { return translation; }

        /**
         * Returns a constant reference to the translation vector
         *
         * @return the constant reference to the translation vector
         */
        constexpr [[nodiscard]] VectorDf const& getTranslation(
            void) const noexcept
                { return translation; }

        /**
         * Destroys a Translation object
         */
        constexpr ~Translation(void) noexcept = default;
    private:
        VectorDf                                translation;
    };

    typedef Translation<dim::Dim2>              Translation2D;
    typedef Translation<dim::Dim3>              Translation3D;

}

#include <MPGL/Core/Transformations/Translation.tpp>
