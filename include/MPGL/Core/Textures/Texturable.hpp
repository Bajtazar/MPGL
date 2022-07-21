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

#include <MPGL/Core/Context/Buffers/ElementArrayBuffer.hpp>
#include <MPGL/Core/Transformations/Transformable2D.hpp>
#include <MPGL/Core/Context/Buffers/VertexBuffer.hpp>
#include <MPGL/Core/Context/Buffers/VertexArray.hpp>
#include <MPGL/Core/Context/Buffers/Vertex.hpp>
#include <MPGL/Core/Textures/Texture.hpp>
#include <MPGL/Utility/Adapter.hpp>
#include <MPGL/Core/Shape.hpp>

namespace mpgl {

    /**
     * Base class for texturable shapes
     */
    class Texturable : public virtual Shape,
        public virtual Transformable2D
    {
    public:
        /**
         * Pure virtual function. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

       /**
         * Pure virtual function. Has to be overloaded.
         * Translates the figure by the given shift vector
         *
         * @param shift the shift vector
         */
        virtual void translate(
            Vector2f const& shift) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Scales the figure around given center by
         * the given factor
         *
         * @param center the scale center
         * @param factor the scale factor
         */
        virtual void scale(Vector2f const& center,
            float32 factor) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Rotates the figure around given point by
         * the given angle counter clockwise
         *
         * @param center the rotation point
         * @param angle the rotation angle [in rads]
         */
        virtual void rotate(Vector2f const& center,
            float32 angle) noexcept = 0;

        /**
         * Pure virtual function. Has to be overloaded.
         * Rotates the figure around given point using
         * given matrix
         *
         * @param center the rotation point
         * @param rotation the rotation matrix
         */
        virtual void rotate(Vector2f const& center,
            Matrix2f const& rotation) noexcept = 0;

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
        typedef std::array<uint32, 6>                   Indexes;
        typedef std::array<Vector2f, 4>                 Positions;

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

        /**
         * Assigns the given rvalue reference to the object
         *
         * @note Wvirtual-move-assign warning workaround
         *
         * @param texturable the given texturable rvalue reference
         * @return the reference to this object
         */
        Texturable& operator=(
            Texturable&& texturable) noexcept;

        /**
         * Pure virtual method. Has to be overloaded. Actualizes
         * buffers before drawing them
         */
        virtual void actualizeBufferBeforeDraw(void) const noexcept = 0;

        Texture                                         texture;
        ElementArrayBuffer                              elementBuffer;

        static constexpr const Indexes                  indexes {
            0, 1, 2, 0, 3, 2};
    };

}
