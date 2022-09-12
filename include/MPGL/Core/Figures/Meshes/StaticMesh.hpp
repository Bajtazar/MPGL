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
#include <MPGL/Iterators/AccessRegisteringIterator.hpp>
#include <MPGL/Core/Vertex/Indices/IndicesTriangle.hpp>
#include <MPGL/Core/Figures/Meshes/MeshVertices.hpp>
#include <MPGL/Core/Figures/Figure.hpp>

namespace mpgl {

    /**
     * Represents a triangle mesh that cannot be modified after
     * initialization
     *
     * @tparam Spec the angular vertices specifier
     */
    template <MeshTraitSpecifier Spec = void>
    class StaticMesh : public Figure3D {
    public:
        using Vertex = MeshVertexT<Spec>;
        using Vertices = std::vector<Vertex>;
        using Indices = std::vector<IndicesTriangle>;

        /**
         * Constructs a new static mesh object
         *
         * @param vertices a rvalue reference to the vertices
         * @param indices a rvalue reference to the indices
         * @param program a constant reference to the shader
         * program name
         */
        StaticMesh(
            Vertices&& vertices,
            Indices&& indices,
            std::string const& program);

        /**
         * Constructs a new static mesh object
         *
         * @tparam VRange the type of the vertices range
         * @tparam IRange the type of the indices range
         * @param vertices an universal reference to the
         * vertices range
         * @param indices an universal reference to the
         * indices range
         * @param program a constant reference to the shader
         * program name
         */
        template <
            std::ranges::input_range VRange,
            std::ranges::input_range IRange>
        requires (SameRangeType<VRange, Vertex>,
            SameRangeType<IRange, IndicesTriangle>)
        StaticMesh(
            VRange&& vertices,
            IRange&& indices,
            std::string const& program);

        /**
         * Constructs a new static mesh object
         *
         * @tparam VIter the type of the vertices iterator
         * @tparam IIter the type of the vertices sentinel
         * @tparam VSent the type of the indices iterator
         * @tparam ISent the type of the indices sentinel
         * @param verticesBegin a constant reference to the begining
         * of the vertices range
         * @param verticesEnd a constant reference to the end
         * of the vertices range
         * @param indicesBegin a constant reference to the begining
         * of the indices range
         * @param indicesEnd a constant reference to the end
         * of the indices range
         * @param program a constant reference to the shader
         * program name
         */
        template <
            std::input_iterator VIter,
            std::input_iterator IIter,
            std::sentinel_for<VIter> VSent,
            std::sentinel_for<IIter> ISent>
        requires (SameIterType<VIter, Vertex>,
            SameIterType<IIter, IndicesTriangle>)
        StaticMesh(
            VIter const& verticesBegin,
            VSent const& verticesEnd,
            IIter const& indicesBegin,
            ISent const& indicesEnd,
            std::string const& program);

        /**
         * Construct a new static mesh object from the given
         * constant reference
         *
         * @param staticMesh a constant reference to the other
         * object
         */
        StaticMesh(StaticMesh const& staticMesh);

        StaticMesh(StaticMesh&& staticMesh) noexcept = default;

        /**
         * Assigns the other static mesh object to this object
         *
         * @param staticMesh a constant reference to the other
         * object
         * @return reference to this object
         */
        StaticMesh& operator=(StaticMesh const& staticMesh);

        StaticMesh& operator=(
            StaticMesh&& staticMesh) noexcept = default;

        /**
         * Draws the static mesh on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Performs transformation on the static mesh
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation3D const& transformator) noexcept;

        /**
         * Returns the reference to vertex with the given index
         *
         * @param index the vertex's index
         * @return the reference to vertex with the given index
         */
        [[nodiscard]] Vertex& operator[] (std::size_t index) noexcept;

        /**
         * Returns the constant reference to vertex with
         * the given index
         *
         * @param index the vertex's index
         * @return the constant reference to vertex with
         * the given index
         */
        [[nodiscard]] Vertex const&
            operator[] (std::size_t index) const noexcept;

        /**
         * Returns the reference to the front vertex
         *
         * @return the reference to the front vertex
         */
        [[nodiscard]] Vertex& front(void) noexcept;

        /**
         * Returns the constant reference to the front vertex
         *
         * @return the constant reference to the front vertex
         */
        [[nodiscard]] Vertex const& front(void) const noexcept;

        /**
         * Returns the reference to the back vertex
         *
         * @return the reference to the back vertex
         */
        [[nodiscard]] Vertex& back(void) noexcept;

        /**
         * Returns the constant reference to the back vertex
         *
         * @return the constant reference to the back vertex
         */
        [[nodiscard]] Vertex const& back(void) const noexcept;

        using iterator = AccessRegisteringIterator<
            typename Vertices::iterator>;
        using const_iterator = Vertices::const_iterator;
        using reverse_iterator = AccessRegisteringIterator<
            typename Vertices::reverse_iterator>;
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
        [[nodiscard]] iterator begin(void) noexcept;

        /**
         * Returns the iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        [[nodiscard]] iterator end(void) noexcept;

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator begin(void) const noexcept;

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept;

        /**
         * Returns the constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept;

        /**
         * Returns the constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept;

        /**
         * Returns the reverse iterator to the end of
         * the vertices
         *
         * @return the reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept;

        /**
         * Returns the reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept;

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept;

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator rend(void) const noexcept;

        /**
         * Returns the constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept;

        /**
         * Returns the constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept;

        /**
         * Destroys the static mesh object
         */
        ~StaticMesh(void) noexcept = default;
    private:
        /**
         * Reloads the element array buffer
         */
        void reloadElementBuffer(void) const noexcept;

        /**
         * Initializes buffers used by the static mesh
         */
        void initializeBuffers(void) const noexcept;

        /**
         * Actualizes vertex buffer before draw
         */
        void actualizeBufferBeforeDraw(void) const noexcept;

        Vertices                                    vertices;
        Indices                                     indices;
        ElementArrayBuffer                          elementBuffer;
    };

    template class StaticMesh<void>;
    template class StaticMesh<uint8>;
    template class StaticMesh<uint16>;
    template class StaticMesh<uint32>;
    template class StaticMesh<int8>;
    template class StaticMesh<int16>;
    template class StaticMesh<int32>;
    template class StaticMesh<int64>;

}

#include <MPGL/Core/Figures/Meshes/StaticMesh.tpp>
