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

#include "Context/Context.hpp"
#include "Dimensions.hpp"

namespace mpgl {

    /**
     * Definision of the drawable class. Base class for
     * all drawable objects. Separates the shapes of
     * different dimensions
     */
    template <Dimension Dim>
    struct Drawable : virtual protected GraphicalObject {
        /**
         * Constructs a new Drawable object
         */
        explicit Drawable(void) noexcept = default;

        /// Disabled due to bug in the DrawableCollection

        // Drawable(Drawable const&) = delete;
        // Drawable(Drawable&&) = delete;

        // Drawable& operator=(Drawable const&) = delete;
        // Drawable& operator=(Drawable&&) = delete;

        /**
         * Pure virtual function. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Destroys the Drawable object
         */
        virtual ~Drawable(void) noexcept = default;
    };

    typedef Drawable<dim::Dim2>                     StackedDrawable;
    typedef Drawable<dim::Dim3>                     VolumetricDrawable;

    typedef Drawable<dim::Dim2>                     Drawable2D;
    typedef Drawable<dim::Dim3>                     Drawable3D;

}
