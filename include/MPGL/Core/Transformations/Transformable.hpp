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

#include <MPGL/Events/Types/ScreenTransformationEvent.hpp>
#include <MPGL/Core/Transformations/Transformation.hpp>

namespace mpgl {

    /**
     * Definision of the transformable class. Allows to the
     * transform the objects with the transformations suitable
     * for their dimensions
     *
     * @tparam Dim the transformed space's dimensions
     */
    template <Dimension Dim>
    class Transformable : public ScreenTransformationEvent {
    public:
        /**
         * Constructs a new transformable object
         */
        explicit Transformable(void) noexcept = default;

        /**
         * Pure virtual method. Has to be overloaded. Performs
         * transformation on this object
         *
         * @param transformator the constant reference to the object
         * perfoming a transformation
         */
        virtual void transform(
            Transformation<Dim> const& transformator) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Transforms the figure during the screen
         * transformation event
         *
         * @param layout the layout of the figure
         * @param oldDimensions the old screen dimensions
         */
        virtual void onScreenTransformation(
            Layout& layout,
            Vector2u const& oldDimensions) noexcept = 0;

        /**
         * Virtual destructor. Destroys the transformable object
         */
        virtual ~Transformable(void) noexcept = default;
    protected:
        Transformable(
            Transformable const&) noexcept = default;

        Transformable(
            Transformable&&) noexcept = default;

        Transformable& operator=(
            Transformable const&) noexcept = default;

        Transformable& operator=(
            Transformable&&) noexcept = default;
    };

    typedef Transformable<dim::Dim2>                Transformable2D;
    typedef Transformable<dim::Dim3>                Transformable3D;

}
