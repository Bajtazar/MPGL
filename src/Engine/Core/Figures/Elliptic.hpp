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

#include "Figure.hpp"

#include "../../Utility/Adapter.hpp"
#include "../Context/Buffers/Vertex.hpp"
#include "../Context/Buffers/ElementArrayBuffer/ElementArrayBuffer.hpp"

namespace mpgl {

    /**
     * Base class for all elliptic shapes
     */
    class Elliptic : public Figure {
    public:
        /// The vertex used by all elliptic shapes
        using Vertex = mpgl::Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>>;

        typedef std::vector<Vertex>                     Vertices;

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
         * a given factor
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
         * @param oldDimensions the old screen dimensions
         */
        virtual void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept = 0;

        /**
         * Sets the elliptic color
         *
         * @param color the color of the elliptic
         */
        void setColor(Color const& color = {}) noexcept
            { this->color = color; }

        /**
         * Returns the constant reference to the elliptic color
         *
         * @return the constant reference to the elliptic color
         */
        Color const& getColor(void) const noexcept
            { return color; }

        /**
         * Virtual destructor. Destroy the Elliptic object
         */
        virtual ~Elliptic(void) noexcept = default;
    protected:
        /**
         * Construct a new Elliptic object with given
         * vector of vertices, color, program name and
         * the executable
         *
         * @param vertices the vector of vertices
         * @param programName the shader program name
         * @param exec the shader's executible
         * @param color the ellipse's color
         */
        explicit Elliptic(
            Vertices vertices,
            std::string const& programName,
            Executable exec,
            Color const& color);

        /**
         * Construct a new Elliptic object from a given
         * constant reference
         *
         * @param shape the given elliptic constant reference
         */
        Elliptic(Elliptic const& shape);

        Elliptic(Elliptic&& shape) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param shape the given elliptic constant reference
         * @return the reference to this object
         */
        Elliptic& operator=(Elliptic const& shape);

        Elliptic& operator=(Elliptic&& shape) noexcept = default;

        typedef std::array<uint32, 6>                   Indexes;

        ElementArrayBuffer                              elementBuffer;
        Color                                           color = {};
        Vertices                                        vertices;

        /**
         * Actualizes vertex buffer before draw
         */
        void actualizeBufferBeforeDraw(void) const noexcept;

        /**
         * Constructs the vector with ellipse vertices
         *
         * @param center the center of the ellipse
         * @param semiAxis the semi-axis of the ellipse
         * @param angle the angle between x-axis and semi-axis
         * @return the ellipse's vertices
         */
        static Vertices ellipseVertices(Vector2f const& center,
            Vector2f const& semiAxis, float32 angle);

        /**
         * Constructs the vector with circle vertices
         *
         * @param center the center of the circle
         * @param radius the radius of the circle
         * @return the circle's vertices
         */
        static Vertices circleVertices(Vector2f const& center,
            float32 radius);

        /**
         * Pure virtual method. Has to be overloaded. Actualizes
         * the matrices responsible for the elliptic shape
         */
        virtual void actualizeMatrices(void) noexcept = 0;
    private:
        /**
         * Initializes inner buffers
         */

        void initializeBuffers(void) const noexcept;

        static constexpr const Indexes                  indexes {
            0, 1, 2, 0, 3, 2};
    };

}
