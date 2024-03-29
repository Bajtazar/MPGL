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

#include <MPGL/Core/Context/Buffers/VertexBuffer.hpp>
#include <MPGL/Core/Context/Buffers/VertexArray.hpp>
#include <MPGL/Core/Drawable.hpp>

namespace mpgl {

    /**
     * Represents every entity that has to utilize the vertex
     * buffer object and vertex array object
     *
     * @tparam Dim the dimension of the space
     */
    template <Dimension Dim>
    class Shape : virtual public Drawable<Dim> {
    public:
        Shape(Shape const& shape) = delete;

        Shape& operator=(Shape const& shape) = delete;

        /**
         * Pure virtual method. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Destroy the Shape object
         */
        virtual ~Shape(void) noexcept = default;
    protected:
        /**
         * Construct a new Shape object
         */
        explicit Shape(void) noexcept = default;

        Shape(Shape&& shape) noexcept = default;

        Shape& operator=(Shape&& shape) noexcept = default;

        /**
         * Pure virtual method. Has to be overloaded. Actualizes
         * buffers before drawing them
         */
        virtual void actualizeBufferBeforeDraw(
            void) const noexcept = 0;

        VertexBuffer                            vertexBuffer;
        VertexArray                             vertexArray;

        mutable bool                            isModified = false;
    };

    typedef Shape<dim::Dim2>                    Shape2D;
    typedef Shape<dim::Dim3>                    Shape3D;

}
