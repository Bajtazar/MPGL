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

#include <MPGL/Core/Figures/Primitives/Helpers/PointsHelpers.hpp>
#include <MPGL/Core/Figures/ResizableAngular.hpp>
#include <MPGL/Traits/DeriveIf.hpp>

namespace mpgl {

    /**
     * Represents a points group on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    class Points :
        public virtual
            DeriveIfT<Dim::orthogonal_space_degree == 3, Clickable>,
        public ResizableAngular<Dim, Spec>
    {
    public:
        using VertexTraits = Angular<Dim, Spec>::VertexTraits;
        using Vector = VertexTraits::Vector;

        /**
         * Constructs a new Points object with given number of points
         * and theirs color
         *
         * @param vertices the number of points
         * @param color the color of points
         */
        Points(
            std::size_t vertices = 0,
            Color const& color = colors::white);

        /**
         * Constructs a new Points object and initializes the
         * vertices with the given positions and color
         *
         * @tparam ColorTp the color vector type
         * @tparam Vectors the parameter pack of vectors
         * @param color the color of the vertices
         * @param positions the vertices positions
         */
        template <class ColorTp, AllConvertible<Vector>... Vectors>
            requires std::constructible_from<Color, ColorTp>
        Points(ColorTp&& color, Vectors&&... vertices);

        /**
         * Construct a new Points object and initializes the
         * vertices with the given positions
         *
         * @tparam Vectors the parameter pack of vectors
         * @param positions the vertices positions
         */
        template <AllConvertible<Vector>... Vectors>
        Points(Vectors&&... vertices);

        Points(Points const& points) = default;
        Points(Points&& points) noexcept = default;

        Points& operator= (Points const& points) = default;
        Points& operator= (Points&& points) noexcept = default;

        /**
         * Checks whether the given pixel is one of the points
         *
         * @param position the pixel's position
         * @return if the given point is one of the points
         */
        [[nodiscard]] virtual bool contains(
            Vector2u const& position) const noexcept;

        /**
         * Draws the polygon on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Virtual destructor. Destroys the Points object
         */
        virtual ~Points(void) noexcept = default;

        friend class PointsDrawer<Dim, Spec>;
        friend class PointsClickChecker<Dim, Spec>;
        friend class PointsClickCheckerNormalizer<Dim, Spec>;
    private:
        using Drawer = PointsDrawer<Dim, Spec>;
        using Clicker = PointsClickChecker<Dim, Spec>;

        static Drawer const                         drawer;
        static Clicker const                        clicker;
    };

    template class Points<dim::Dim2>;
    template class Points<dim::Dim3>;
    template class Points<dim::Dim2, uint8>;
    template class Points<dim::Dim3, uint8>;

    typedef Points<dim::Dim2>                       Points2D;
    typedef Points<dim::Dim3>                       Points3D;

}

#include <MPGL/Core/Figures/Primitives/Points.tpp>
