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

#include "Texture.hpp"
#include "../Shape.hpp"
#include "../../Utility/Adapter.hpp"
#include "../Context/Buffers/Vertex.hpp"
#include "../Transformations/Transformable2D.hpp"
#include "../Context/Buffers/VertexArray/VertexArray.hpp"
#include "../Context/Buffers/VertexBuffer/VertexBuffer.hpp"
#include "../Context/Buffers/ElementArrayBuffer/ElementArrayBuffer.hpp"

namespace mpgl {

    /**
     * Base class for texturable shapes
     *
     * @tparam IsColorable specifies whether the vertices
     * should contain information about color
     */
    template <bool IsColorable>
    class Texturable : public virtual Shape,
        public virtual Transformable2D
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
         * Returns the reference to vertex with the given index
         *
         * @param index the vertex's index
         * @return the reference to vertex with the given index
         */
        [[nodiscard]] Vertex& operator[] (std::size_t index) noexcept
            { isModified = true; return vertices[index]; }

        /**
         * Returns the constant reference to vertex with
         * the given index
         *
         * @param index the vertex's index
         * @return the constant reference to vertex with
         * the given index
         */
        [[nodiscard]] Vertex const& operator[] (
            std::size_t index) const noexcept
                { return vertices[index]; }

        /**
         * Returns the reference to the front vertex
         *
         * @return the reference to the front vertex
         */
        [[nodiscard]] Vertex& front(void) noexcept
            { isModified = true; return vertices.front(); }

        /**
         * Returns the constant reference to the front vertex
         *
         * @return the constant reference to the front vertex
         */
        [[nodiscard]] Vertex const& front(void) const noexcept
            { return vertices.front(); }

        /**
         * Returns the reference to the back vertex
         *
         * @return the reference to the back vertex
         */
        [[nodiscard]] Vertex& back(void) noexcept
            { isModified = true; return vertices.back(); }

        /**
         * Returns the constant reference to the back vertex
         *
         * @return the constant reference to the back vertex
         */
        Vertex const& back(void) const noexcept
            { return vertices.back(); }

        using iterator                    = Vertices::iterator;
        using const_iterator              = Vertices::const_iterator;
        using reverse_iterator
            = Vertices::reverse_iterator;
        using const_reverse_iterator
            = Vertices::const_reverse_iterator;

        /**
         * Returns the number of vertices in the object
         *
         * @return the number of vertices
         */
        [[nodiscard]] std::size_t size(void) const noexcept
            { return vertices.size(); }

        /**
         * Returns the iterator to the begining of the vertices
         *
         * @return the iterator to the begining of the vertices
         */
        [[nodiscard]] iterator begin(void) noexcept
            { isModified = true; return vertices.begin(); }

        /**
         * Returns the iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        [[nodiscard]] iterator end(void) noexcept
            { isModified = true; return vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return vertices.end(); }

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return vertices.begin(); }

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return vertices.end(); }

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept
            { isModified = true; return vertices.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept
            { isModified = true; return vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return vertices.rbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return vertices.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return vertices.crbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return vertices.crend(); }

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
         * Construct a new Texturable object from a given texture
         * and with given vertices color
         *
         * @param texture the texture object
         * @param color the color of the vertices
         */
        Texturable(
            Texture const& texture,
            Color const& color) requires IsColorable;

        /**
         * Construct a new Texturable object from a given texture
         *
         * @param texture the texture object
         */
        Texturable(Texture const& texture = {});

        /**
         * Construct a new Texturable object from a given vertices
         * vector and with a given texture
         *
         * @param positions the vertices positions
         * @param texture the texture object
         */
        Texturable(Positions positions, Texture const& texture);

        /**
         * Construct a new Texturable object from a given vertices
         * vector with a given texture using given color
         *
         * @param positions the vertices positions
         * @param texture the texture object
         * @param color the vertices color
         */
        Texturable(Positions positions, Texture const& texture,
            Color const& color) requires (IsColorable);

        /**
         * Construct a new Texturable object from a given
         * texturable constant reference
         *
         * @param texturable the texturable constant reference
         */
        Texturable(Texturable const& texturable);

        Texturable(Texturable&& texturable) noexcept = default;

        /**
         * Assigns the given constant reference to the object
         *
         * @param texturable the given texturable constant reference
         * @return the reference to this object
         */
        Texturable& operator=(Texturable const& texturable);

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
         * Actualizes vertex buffer before draw
         */
        virtual void actualizeBufferBeforeDraw(void) const noexcept;

        Vertices                                        vertices;
        Texture                                         texture;
        ElementArrayBuffer                              elementBuffer;

        static constexpr const Indexes                  indexes {
            0, 1, 2, 0, 3, 2};
    private:
        /**
         * Initializes inner buffers
         */
        void initializeBuffers(void) const noexcept;

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
    };

    typedef Texturable<true>                            ColorableTexturable;
    typedef Texturable<false>                           DefaultTexturable;

    template class Texturable<true>;
    template class Texturable<false>;

}
