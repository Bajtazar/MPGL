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

#include <MPGL/Core/Transformations/Transformable.hpp>
#include <MPGL/Core/Figures/Clickable.hpp>
#include <MPGL/Traits/DeriveIf.hpp>
#include <MPGL/Core/Model.hpp>
#include <MPGL/Core/Shape.hpp>

namespace mpgl {

    /**
     * Base class for figures
     *
     * @tparam Dim the dimension of the space
     */
    template <Dimension Dim>
    class Figure :
        public virtual Shape<Dim>,
        public virtual Transformable<Dim>,
        public virtual DeriveIfT<TwoDimensional<Dim>, Clickable>,
        public Shadeable,
        public DeriveIfT<ThreeDimensional<Dim>, Model>
    {
    public:
        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation<Dim> const& transformator) noexcept = 0;

        /**
         * Destroys the Figure object
         */
        virtual ~Figure(void) noexcept = default;
    protected:
        /**
         * Construct a new Figure object. Loads the given
         * shaders
         *
         * @param programName the given shader name
         */
        explicit Figure(std::string const& programName);

        /**
         * Construct a new Figure object from the given
         * constant reference
         *
         * @param shape the given shape constant reference
         */
        explicit Figure(Figure const& shape);

        explicit Figure(Figure&& shape) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param shape the given figure constant reference
         * @return the reference to this object
         */
        Figure& operator=(Figure const& shape);

        /**
         * Assigns the given rvalue reference to the object
         *
         * @note Wvirtual-move-assign warning workaround
         *
         * @param shape the given figure rvalue reference
         * @return the reference to this object
         */
        Figure& operator=(Figure&& shape) noexcept;

        /**
         * Sets the shader locations
         */
        void setLocations(void);
    };

    template class Figure<dim::Dim2>;
    template class Figure<dim::Dim3>;

    typedef Figure<dim::Dim2>                   Figure2D;
    typedef Figure<dim::Dim3>                   Figure3D;

}
