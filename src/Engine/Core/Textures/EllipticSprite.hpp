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

#include "ShadeableSprite.hpp"

#include "../../Iterators/VertexViewIterator.hpp"

namespace mpgl {

    /**
     * Base class for the elliptic sprites
     *
     * @tparam IsColorable specifies whether the vertices
     * should contain information about color
     */
    template <bool IsColorable>
    class EllipticSprite : public ShadeableSprite<IsColorable> {
    public:
        typedef ShadeableSprite<IsColorable>::Vertex    Vertex;
        typedef ShadeableSprite<IsColorable>::Vertices  Vertices;
    private:
        typedef std::conditional_t<IsColorable,
            TSHolder<"texCoords", "color">,
            TSHolder<"texCoords">>                      FieldHolder;
    public:
        typedef VertexViewIterFromHolderT<typename
            Vertices::iterator, FieldHolder>            iterator;
        typedef VertexViewIterFromHolderT<typename
            Vertices::const_iterator, FieldHolder>      const_iterator;

        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

        typedef typename iterator::value_type           VertexView;
        typedef typename const_iterator::value_type     ConstVertexView;

        /**
         * Pure virtual function. Has to be overloaded.
         * Allows to draw an object
         */
        virtual void draw(void) const noexcept = 0;

        /**
         * Returns the number of vertices in the object
         *
         * @return the number of vertices
         */
        [[nodiscard]] std::size_t size(void) const noexcept
            { return this->vertices.size(); }

        /**
         * Returns a vertex view to the vertex with
         * given index
         *
         * @param index the vertex's index
         * @return the view to a desired vertex
         */
        [[nodiscard]] VertexView operator[] (
            std::size_t index) noexcept;

        /**
         * Returns a constant vertex view to the vertex with
         * given index
         *
         * @param index the vertex's index
         * @return the constant view to a desired vertex
         */
        [[nodiscard]] ConstVertexView operator[] (
            std::size_t index) const noexcept;

        /**
         * Returns a vertex view to the first vertex
         *
         * @return the view to the first vertex
         */
        [[nodiscard]] VertexView front(void) noexcept;

        /**
         * Returns a constant vertex view to the first vertex
         *
         * @return the constant view to the first vertex
         */
        [[nodiscard]] ConstVertexView front(void) const noexcept;

        /**
         * Returns a vertex view to the last vertex
         *
         * @return the view to the last vertex
         */
        [[nodiscard]] VertexView back(void) noexcept;

        /**
         * Returns a constant vertex view to the last vertex
         *
         * @return the constant view to the last vertex
         */
        [[nodiscard]] ConstVertexView back(void) const noexcept;

        /**
         * Returns a vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] iterator begin(void) noexcept;

        /**
         * Returns a vertex view iterator to the end of
         * the vertices vector
         *
         * @return the vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] iterator end(void) noexcept;

        /**
         * Returns a constant vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] const_iterator begin(void) const noexcept;

        /**
         * Returns a constant vertex view iterator to the end of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] const_iterator end(void) const noexcept;

        /**
         * Returns a constant vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept;

        /**
         * Returns a constant vertex view iterator to the end of
         * the vertices vector
         *
         * @return the constant vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] const_iterator cend(void) const noexcept;

        /**
         * Returns a reverse vertex view iterator to the end of
         * the vertices vector
         *
         * @return the reverse vertex view iterator to the end of
         * the vertices vector
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept;

        /**
         * Returns a reverse vertex view iterator to the begin of
         * the vertices vector
         *
         * @return the reverse vertex view iterator to the begin of
         * the vertices vector
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept;

        /**
         * Returns a constant reverse vertex view iterator to
         * the end of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the end of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept;

        /**
         * Returns a constant reverse vertex view iterator to
         * the begin of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the begin of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept;

        /**
         * Returns a constant reverse vertex view iterator to
         * the end of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the end of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept;

        /**
         * Returns a constant reverse vertex view iterator to
         * the begin of the vertices vector
         *
         * @return the constant reverse vertex view iterator to
         * the begin of the vertices vector
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept;

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram const& program) noexcept override final;

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram&& program) noexcept override final;

        /**
         * Sets the given shader program from an internal
         * library
         *
         * @param name the name of the shader program
         */
        void setShader(
            std::string const& name) override final;

        /**
         * Virtual destructor. Destroy the Elliptic Sprite object
         */
        virtual ~EllipticSprite(void) noexcept = default;
    protected:
        typedef ShadeableSprite<IsColorable>::Positions Positions;

        /**
         * Construct a new Elliptic Sprite object from a vertices
         * positions vector, texture and shader program name
         *
         * @param positions the vector of vertices positions
         * @param texture the sprite's texture
         * @param programName the shader program name
         */
        explicit EllipticSprite(
            Positions positions,
            Texture const& texture,
            std::string const& programName);

        /**
         * Construct a new Elliptic Sprite object from a vertices
         * positions vector, texture, vertices color and shader
         * program name
         *
         * @param positions the vector of vertices positions
         * @param texture the sprite's texture
         * @param programName the shader program name
         * @param color the vertices color
         */
        explicit EllipticSprite(
            Positions positions,
            Texture const& texture,
            std::string const& programName,
            Color const& color) requires IsColorable;

        EllipticSprite(EllipticSprite const& sprite) = default;
        EllipticSprite(EllipticSprite&& sprite) noexcept = default;

        EllipticSprite& operator=(
            EllipticSprite const& sprite) = default;
        EllipticSprite& operator=(
            EllipticSprite&& sprite) noexcept = default;

        /**
         * Constructs the vector with ellipse positions
         *
         * @param center the center of the ellipse
         * @param semiAxis the semi-axis of the ellipse
         * @param angle the angle between x-axis and semi-axis
         * @return the ellipse's positions
         */
        static Positions ellipsePositions(
            Vector2f const& center,
            Vector2f const& semiAxis,
            float32 angle);

        /**
         * Constructs the vector with circle positions
         *
         * @param center the center of the circle
         * @param radius the radius of the circle
         * @return the circle's positions
         */
        static Positions circlePositions(
            Vector2f const& center,
            float32 radius);

        /**
         * Pure virtual method. Has to be overloaded. Actualizes
         * the matrices responsible for the elliptic shape
         */
        virtual void actualizeMatrices(void) noexcept = 0;
    private:
        typedef Shadeable::Executable                   Executable;

        static Executable const                         shaderExec;
    };

    template class EllipticSprite<true>;
    template class EllipticSprite<false>;

}
