/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Core/Figures/Primitives/Helpers/LineStripHelpers.hpp>
#include <MPGL/Core/Figures/ResizableAngular.hpp>
#include <MPGL/Traits/DeriveIf.hpp>

namespace mpgl {

    /**
     * Represents a line strip on the screen
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, AngularTraitSpecifier<Dim> Spec>
    class LineStrip :
        public virtual
            DeriveIfT<Dim::orthogonal_space_degree == 3, Clickable>,
        public ResizableAngular<Dim, Spec>
    {
    public:
        using VertexTraits = Angular<Dim, Spec>::VertexTraits;
        using Vector = VertexTraits::Vector;

        /**
         * Constructs a new Line Strip object and initializes the
         * vertices with the given positions and color
         *
         * @tparam ColorTp the color vector type
         * @tparam Vectors the parameter pack of vectors
         * @param color the color of the vertices
         * @param positions the vertices positions
         */
        template <class ColorTp, AllConvertible<Vector>... Vectors>
            requires std::constructible_from<Color, ColorTp>
        LineStrip(ColorTp&& color, Vectors&&... vertices);

        /**
         * Constructs a new Line Strip object and initializes the
         * vertices with the given positions
         *
         * @tparam Vectors the parameter pack of vectors
         * @param positions the vertices positions
         */
        template <AllConvertible<Vector>... Vectors>
        LineStrip(Vectors&&... vertices);

        /**
         * Constructs a new Line Strip object with given number
         * of vertices and the given base color
         *
         * @param vertices the number of the vertices
         * @param color the color of the vertices
         */
        LineStrip(
            std::size_t vertices = 0,
            Color const& color = colors::white);

        LineStrip(LineStrip const& lineStrip) = default;
        LineStrip(LineStrip&& lineStrip) noexcept = default;

        LineStrip& operator= (LineStrip const& lineStrip) = default;
        LineStrip& operator= (LineStrip&& lineStrip) noexcept = default;

        /**
         * Draws the polygon on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Checks whether the given pixel is located on the line strip
         *
         * @param position the pixel's position
         * @return if the given point is on the line strip
         */
        [[nodiscard]] virtual bool contains(
            Vector2u const& position) const noexcept;

        /**
         * Virtual destructor. Destroys the Line Strip object
         */
        virtual ~LineStrip(void) noexcept = default;

        friend class LineStripDrawer<Dim, Spec>;
        friend class LineStripClickChecker<Dim, Spec>;
        friend class LineStripClickCheckerNormalizer<Dim, Spec>;
    private:
        using Drawer = LineStripDrawer<Dim, Spec>;
        using Clicker = LineStripClickChecker<Dim, Spec>;

        static Drawer const                         drawer;
        static Clicker const                        clicker;
    };

    template class LineStrip<dim::Dim2>;
    template class LineStrip<dim::Dim3>;
    template class LineStrip<dim::Dim2, uint8>;
    template class LineStrip<dim::Dim3, uint8>;

    typedef LineStrip<dim::Dim2>                    LineStrip2D;
    typedef LineStrip<dim::Dim3>                    LineStrip3D;

}

#include <MPGL/Core/Figures/Primitives/LineStrip.tpp>
