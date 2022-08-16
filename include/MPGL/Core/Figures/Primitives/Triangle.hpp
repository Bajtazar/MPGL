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

#include <MPGL/Core/Figures/Primitives/Helpers/TriangleHelpers.hpp>
#include <MPGL/Traits/DeriveIf.hpp>

namespace mpgl {

    /**
     * Represents a triangle figure
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    class Triangle :
        public virtual
            DeriveIfT<Dim::orthogonal_space_degree == 3, Clickable>,
        public Angular<Dim, Spec>
    {
    public:
        using VertexTraits = Angular<Dim, Spec>::VertexTraits;
        using Vector = VertexTraits::Vector;

        /**
         * Constructs a new Triangle with given
         * vertices
         *
         * @param firstVertex the first vertex coord
         * @param secondVertex the second vertex coord
         * @param thirdVertex the third vertex coord
         * @param color the color of the triangle
         */
        Triangle(
            Vector const& firstVertex,
            Vector const& secondVertex,
            Vector const& thirdVertex,
            Color const& color = {});

        /**
         * Constructs a new Triangle with given
         * color
         *
         * @param color the color of the triangle
         */
        Triangle(Color const& color = {});

        Triangle(Triangle const& triangle) = default;
        Triangle(Triangle&& triangle) noexcept = default;

        Triangle& operator= (Triangle const& triangle) = default;
        Triangle& operator= (Triangle&& triangle) noexcept = default;

        /**
         * Draws the triangle on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Checks whether the given pixel is located inside
         * the triangle
         *
         * @param position the pixel's position
         * @return if the given point is inside the triangle
         */
        [[nodiscard]] virtual bool contains(
            Vector2u const& position) const noexcept;

        /**
         * Virtual destructor. Destroys the Triangle object
         */
        virtual ~Triangle(void) noexcept = default;

        friend class TriangleDrawer<Dim, Spec>;
        friend class TriangleClickChecker<Dim, Spec>;
    private:
        using Drawer = TriangleDrawer<Dim, Spec>;
        using Clicker = TriangleClickChecker<Dim, Spec>;

        static Drawer const                         drawer;
        static Clicker const                        clicker;
    };

    template class Triangle<dim::Dim2>;
    template class Triangle<dim::Dim3>;
    template class Triangle<dim::Dim2, uint8>;
    template class Triangle<dim::Dim3, uint8>;

    typedef Triangle<dim::Dim2>                     Triangle2D;
    typedef Triangle<dim::Dim3>                     Triangle3D;

}
