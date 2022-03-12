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

#include "Texturable.hpp"
#include "../Figures/Figure.hpp"

namespace mpgl {

    /**
     * Base class for independent texturable shapes
     *
     * @tparam IsColorable specifies whether the vertices
     * should contain information about color
     */
    template <bool IsColorable>
    class ShadeableSprite : public Texturable,
        public Figure
    {
    private:
        /// The default texturable vertex
        using DefaultVertex = mpgl::Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>
        >;

        /// The colorable texturable vertex
        using ColorableVertex = mpgl::Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;
    public:
        using Vertex = std::conditional_t<IsColorable, ColorableVertex,
            DefaultVertex>;

        typedef std::vector<Vertex>                     Vertices;
        /**
         * Pure virtual function. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Transforms the figure during the screen
         * transformation event
         *
         * @param oldDimensions the old screen dimensions
         */
        virtual void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept;

        /**
         * Translates the figure by the given shift vector
         *
         * @param shift the shift vector
         */
        virtual void translate(Vector2f const& shift) noexcept;

        /**
         * Scales the figure around given center by
         * a given factor
         *
         * @param center the scale center
         * @param factor the scale factor
         */
        virtual void scale(Vector2f const& center,
            float32 factor) noexcept;

        /**
         * Rotates the figure around given point by
         * the given angle counter clockwise
         *
         * @param center the rotation point
         * @param angle the rotation angle [in rads]
         */
        virtual void rotate(Vector2f const& center,
            float32 angle) noexcept;

        /**
         * Rotates the figure around given point using
         * given matrix
         *
         * @param center the rotation point
         * @param rotation the rotation matrix
         */
        virtual void rotate(Vector2f const& center,
            Matrix2f const& rotation) noexcept;

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the
         * shader program object
         */
        virtual void setShader(
            ShaderProgram const& program) noexcept override;

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the
         * shader program object
         */
        virtual void setShader(
            ShaderProgram&& program) noexcept override;

        /**
         * Sets the given shader program from an internal
         * library
         *
         * @param name the name of the shader program
         */
        virtual void setShader(
            std::string const& name) override;

        /**
         * Destroy the Shadeable Sprite object
         */
        virtual ~ShadeableSprite(void) noexcept = default;
    protected:
        typedef Texturable::Positions           Positions;

        /**
         * Construct a new Shadeable Sprite object from a given
         * texture and shader name
         *
         * @param texture the texture object
         * @param shaderName the shader program name
         */
        ShadeableSprite(
            Texture const& texture,
            std::string const& shaderName);

        /**
         * Construct a new Shadeable Sprite object from a given
         * texture and shader name with given vertices color
         *
         * @param texture the texture object
         * @param shaderName the shader program name
         * @param color the color of the vertices
         */
        ShadeableSprite(
            Texture const& texture,
            std::string const& shaderName,
            Color const& color) requires IsColorable;

        /**
         * Construct a new Shadeable Sprite object from a given
         * vertices vector with a given texture. Sets the given
         * shader program
         *
         * @param positions the vertices positions
         * @param texture the texture object
         * @param shaderName the shader program name
         */
        ShadeableSprite(
            Positions positions,
            Texture const& texture,
            std::string const& shaderName);

        /**
         * Construct a new Shadeable Sprite object from a given
         * vertices vector with a given texture using
         * given color. Sets the given shader program
         *
         * @param positions the vertices positions
         * @param texture the texture object
         * @param shaderName the shader program name
         * @param color the vertices color
         */
        ShadeableSprite(
            Positions positions,
            Texture const& texture,
            std::string const& shaderName,
            Color const& color) requires IsColorable;

        /**
         * Construct a new Shadeable Sprite object from a
         * given constant reference shadeable sprite object
         *
         * @param spite the constant reference to the shadeable
         * sprite object
         */
        ShadeableSprite(ShadeableSprite const& sprite);

        ShadeableSprite(ShadeableSprite&&) noexcept = default;

        /**
         * Assigns a given shadeable sprite to this object
         *
         * @param sprite the constant reference to sprite object
         * @return the reference to this object
         */
        ShadeableSprite& operator=(
            ShadeableSprite const& sprite);

        /**
         * Assigns the given rvalue reference to the object
         *
         * @note Wvirtual-move-assign warning workaround
         *
         * @param sprite the given shadeable sprite rvalue reference
         * @return the reference to this object
         */
        ShadeableSprite& operator=(
            ShadeableSprite&& sprite) noexcept;

        /**
         * Actualizes buffers before draw
         */
        virtual void actualizeBufferBeforeDraw(void) const noexcept;

        Vertices                                vertices;
    private:
        /**
         * Creates the four vertices vector with the correct
         * texture positions
         *
         * @param positions the vertices positions
         * @return the vertices vector
         */
        static Vertices makeVertices(
            Positions const& positions = {});

        /**
         * Creates the four vertices vector with the correct
         * texture positions and with a given color
         *
         * @param color the color of the vertices
         * @param positions the vertices positions
         * @return the vertices vector
         */
        static Vertices makeVertices(
            Color const& color,
            Positions const& positions = {}
            ) requires IsColorable;

        /**
         * Initializes inner buffers
         */
        void initializeBuffers(void) const noexcept;

        /// The shader initializing program
        static Executable const                 shaderExec;
    };

    template class ShadeableSprite<true>;
    template class ShadeableSprite<false>;

}
