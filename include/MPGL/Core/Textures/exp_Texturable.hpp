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

#include <MPGL/Core/Transformations/Transformable.hpp>
#include <MPGL/Core/Textures/Texture.hpp>
#include <MPGL/Utility/Adapter.hpp>
#include <MPGL/Core/Drawable.hpp>

namespace mpgl::exp {

    /**
     * Base class for texturable shapes
     *
     * @tparam Dim the dimension of the space
     */
    template <Dimension Dim>
    class Texturable :
        public virtual Drawable<Dim>,
        public virtual Transformable<Dim>
    {
    public:
        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Pure virtual method. Has to be overloaded
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation<Dim> const& transformator) noexcept = 0;

        /**
         * Sets the current texture object
         *
         * @param texture the constant reference to the texture
         */
        void setTexture(Texture const& texture)
            { this->texture = texture; }

        /**
         * Sets the current texture object
         *
         * @param texture the rvalue reference to the texture
         */
        void setTexture(Texture&& texture)
            { this->texture = std::move(texture); }

        /**
         * Returns the constant reference to the texture
         *
         * @return the constant reference to the texture
         */
        Texture const& getTexture(void) const noexcept
            { return texture; }

        /**
         * Virtual destructor. Destroy the Texturable object
         */
        virtual ~Texturable(void) noexcept = default;
    protected:
        /**
         * Construct a new Texturable object from the given texture
         *
         * @param texture the texture object
         */
        Texturable(Texture const& texture);

        /**
         * Construct a new Texturable object from the given
         * texturable constant reference
         *
         * @param texturable the texturable constant reference
         */
        Texturable(Texturable const& texturable);

        Texturable(Texturable&& texturable) noexcept = default;

        Texturable& operator=(Texturable const& texturable) = default;
        Texturable& operator=(
            Texturable&& texturable) noexcept = default;

        Texture                                         texture;
    };

    template class Texturable<dim::Dim2>;
    template class Texturable<dim::Dim3>;

    typedef Texturable<dim::Dim2>                       Texturable2D;
    typedef Texturable<dim::Dim3>                       Texturable3D;

}
