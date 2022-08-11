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
#include <MPGL/Core/Transformations/Transformable.hpp>
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
    class Texturable : public virtual Shape2D,
        public virtual Transformable2D
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
            Transformation2D const& transformator) noexcept = 0;

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
