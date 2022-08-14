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

#include <MPGL/Core/Figures/Primitives/Helpers/LineHelpers.hpp>
#include <MPGL/Traits/DeriveIf.hpp>

namespace mpgl {

    /**
     * Represents a line figure
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    class Line :
        public virtual
            DeriveIfT<Dim::orthogonal_space_degree == 3, Clickable>,
        public Angular<Dim, Spec>
    {
    public:
        using VertexTraits = Angular<Dim, Spec>::VertexTraits;
        using Vector = VertexTraits::Vector;

        /**
         * Construct a new line with the begin
         * in the first vertex and the end in the
         * second one
         *
         * @param firstVertex the begin of the line
         * @param secondVertex the end of the line
         * @param color the color of the line (optional)
         */
        Line(
            Vector const& firstVertex,
            Vector const& secondVertex,
            Color const& color = {});

        /**
         * Construct a new Line object with the
         * given color
         *
         * @param color the color of the line (optional)
         */
        Line(Color const& color = {});

        Line(Line const& line) = default;
        Line(Line&& line) noexcept = default;

        Line& operator=(Line const& line) = default;
        Line& operator=(Line&& line) noexcept = default;

        /**
         * Returns the center of the line
         *
         * @return the center of the line
         */
        [[nodiscard]] Vector getLineCenter(void) const noexcept;

        /**
         * Draws the line on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Checks whether the given pixel is located on the line
         *
         * @param position the pixel's position
         * @return if the given point is located on the line
         */
        [[nodiscard]] bool contains(
            Vector2u const& position) const noexcept final;

        /**
         *  Destroy the Line object
         */
        ~Line(void) noexcept = default;

        friend class LineDrawer<Dim, Spec>;
        friend class LineClickChecker<Dim, Spec>;
    private:
        using Drawer = LineDrawer<Dim, Spec>;
        using Clicker = LineClickChecker<Dim, Spec>;

        static Drawer const                         drawer;
        static Clicker const                        clicker;
    };

    template class Line<dim::Dim2>;
    template class Line<dim::Dim3>;

    typedef Line<dim::Dim2>                         Line2D;
    typedef Line<dim::Dim3>                         Line3D;

}
