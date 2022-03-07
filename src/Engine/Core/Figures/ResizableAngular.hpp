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

#include "Angular.hpp"

#include <optional>

namespace mpgl {

    /**
     * Base class for all resizable angular shapes
     */
    class ResizableAngular : public Angular {
    public:
        typedef std::size_t                 size_type;
        typedef std::optional<Vector2f>     OptionalVec2f;

        /**
         * Pure virtual function. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Changes size of the vertices array
         *
         * @param size the new size of the vertices array
         */
        void resize(size_type size);

        /**
         * Reserves new space for the vertices in the vertices
         * array
         *
         * @param size the reserved size
         */
        void reserve(size_type size);

        /**
         * Pushes a new vertex into the vertices array
         *
         * @param vertex the new vertex
         */
        void push(Vertex vertex);

        /**
         * Emplaces a new vertex into the vertices array
         *
         * @param position the position of the vertex
         * @param color the color of the vertex
         */
        void emplace(Vector2f const& position,
            Color const& color);

        /**
         * Removes last vertex from the vertices array
         */
        void pop(void) noexcept;

        /**
         * Returns the center of the angular
         *
         * @return the vector containing center
         */
        OptionalVec2f getCenter(void) const noexcept;

        /**
         * Virtual Destructor. Destroys the Resizable Angular object
         */
        virtual ~ResizableAngular(void) noexcept = default;
    protected:
        /**
         * Construct a new Resizable Angular object with a given
         * size vertices array
         *
         * @param size the size of the vertices array
         * @param color the color of the vertices
         */
        explicit ResizableAngular(size_t size, Color const& color);

        /**
         * Construct a new Resizable Angular object with a given
         * vertices array
         *
         * @param vertices the vertices array
         */
        explicit ResizableAngular(Vertices vertices);

        ResizableAngular(
            ResizableAngular const& shape) noexcept = default;
        ResizableAngular(
            ResizableAngular&& shape) noexcept = default;

        /**
         * Construct a new Resizable Angular object from a given
         * vertices positions and their common color
         *
         * @tparam ColorTp the type of the color vector
         * @tparam Args the parameter pack of 2D vectors
         * @param color the color of the vertices
         * @param positions the positions of the vertices
         */
        template <class ColorTp, AllConvertible<Vector2f>... Args>
            requires std::constructible_from<Color, ColorTp>
        explicit ResizableAngular(ColorTp&& color,
            Args&&... positions);

        /**
         * Construct a new Resizable Angular object from a given
         * vertices positions
         *
         * @tparam Args the parameter pack of 2D vectors
         * @param positions the positions of the vertices
         */
        template <AllConvertible<Vector2f>... Args>
        explicit ResizableAngular(Args&&... positions);

        ResizableAngular& operator= (
            ResizableAngular&& shape) noexcept = default;

        /**
         * Assigns a given resizable angular to this object
         *
         * @param shape the assigned resizable angular
         * @return the reference to this object
         */
        ResizableAngular& operator= (
            ResizableAngular const& shape);

        /**
         * Actualizes vertex buffer before draw. Overloads
         * the Angular implementation
         */
        void actualizeBufferBeforeDraw(void) const noexcept;

        mutable bool                                    isExtended = false;
    };

    template <class ColorTp, AllConvertible<Vector2f>... Args>
        requires std::constructible_from<Color, ColorTp>
    ResizableAngular::ResizableAngular(ColorTp&& color,
        Args&&... positions)
            : Angular{Vertices{Vertex{std::forward<Args>(positions),
                color}...}} {}

    template <AllConvertible<Vector2f>... Args>
    ResizableAngular::ResizableAngular(Args&&... positions)
        : Angular{Vertices{Vertex{std::forward<Args>(positions),
            {}}...}} {}

}
