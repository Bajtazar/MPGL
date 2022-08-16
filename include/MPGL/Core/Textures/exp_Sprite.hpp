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

#include <MPGL/Core/Textures/exp_Texturable.hpp>
#include <MPGL/Core/Figures/Clickable.hpp>

namespace mpgl::exp {

    /**
     * Base class for independent texturable shapes
     *
     * @tparam Dim the dimension of the space
     */
    template <Dimension Dim>
    class Sprite :
        public virtual Clickable,
        public Texturable<Dim>
    {
        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded.
         * Checks whether the given pixel is located
         * inside of the figure [boundry is concidered
         * as a part of the figure, the 3D figures are
         * projected onto screen and then checked]
         *
         * @param position the pixel's position
         * @return if point is inside figure
         */
        [[nodiscard]] virtual bool contains(
            Vector2u const& position) const noexcept = 0;

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation<Dim> const& transformator) noexcept;

        /**
         * Pure virtual method. Has to be overloaded. Applies
         * convolution shader with given convolution matrix
         *
         * @param convolution the convolution matrix
         */
        virtual void setConvolution(
            Matrix3f const& convolution) = 0;

        /**
         * Pure virtual method. Has to be overloaded. Applies
         * default shader - removes convolution
         */
        virtual void resetConvolution(void) = 0;

        /**
         * Destroy the Sprite object
         */
        virtual ~Sprite(void) noexcept = default;
    protected:
        /**
         * Construct a new Sprite object from the given
         * texture and shader name with given vertices color
         *
         * @param texture the texture object
         */
        Sprite(Texture const& texture);

        Sprite(Sprite const& sprite) = default;
        Sprite(Sprite&&) noexcept = default;

        Sprite& operator=(Sprite const& sprite) = default;
        Sprite& operator=(Sprite&& sprite) noexcept = default;
    };

}
