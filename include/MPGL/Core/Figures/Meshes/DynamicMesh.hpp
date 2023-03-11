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

#include <deque>

namespace mpgl {

    /**
     * Represents a triangle mesh that can be modified during
     * its lifetime. Provides a convinient views that presents
     * vertices along the triangles that they are part of
     *
     * @tparam Spec the angular vertices specifier
     */
    template <MeshTraitSpecifier Spec = void>
    class DynamicMesh : public Figure3D {
    public:
        using Vertex = MeshVertexT<Spec>;
        using Vertices = std::vector<Vertex>;
        using Indices = std::vector<IndicesTriangle>;

        /**
         * Constructs a new dynamic mesh object
         *
         * @param vertices a rvalue reference to the vertices
         * @param indices a rvalue reference to the indices
         * @param program a constant reference to the shader
         * program name
         */
        DynamicMesh(
            Vertices&& vertices,
            Indices&& indices,
            std::string const& program);

        /**
         * Constructs a new dynamic mesh object
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
        DynamicMesh(
            VRange&& vertices,
            IRange&& indices,
            std::string const& program);

        /**
         * Constructs a new dynamic mesh object
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
        DynamicMesh(
            VIter const& verticesBegin,
            VSent const& verticesEnd,
            IIter const& indicesBegin,
            ISent const& indicesEnd,
            std::string const& program);

        /**
         * Construct a new dynamic mesh object from the given
         * constant reference
         *
         * @param dynamicMesh a constant reference to the other
         * object
         */
        DynamicMesh(DynamicMesh const& dynamicMesh);

        DynamicMesh(DynamicMesh&& dynamicMesh) noexcept = default;

        /**
         * Assigns the other dynamic mesh object to this object
         *
         * @param dynamicMesh a constant reference to the other
         * object
         * @return reference to this object
         */
        DynamicMesh& operator=(DynamicMesh const& dynamicMesh);

        DynamicMesh& operator=(
            DynamicMesh&& dynamicMesh) noexcept = default;

        /**
         * Draws the dynamic mesh on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Performs transformation on the dynamic mesh
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        virtual void transform(
            Transformation3D const& transformator) noexcept;

        /**
         * Convinient view for a vertex and the triangles that
         * includes the given vertex
         */
        class VertexView {
        private:
            using IndicesVector = std::vector<uint32>;
        public:
            VertexView(VertexView const&) = default;
            VertexView(VertexView&&) noexcept = default;

            VertexView& operator=(VertexView const&) = default;
            VertexView& operator=(VertexView&&) noexcept = default;

            /**
             * Returns a reference to the vertex object
             *
             * @return the reference to the vertex object
             */
            [[nodiscard]] Vertex& getVertex(void) noexcept;

            /**
             * Returns a constant reference to the vertex object
             *
             * @return the constant reference to the vertex object
             */
            [[nodiscard]] Vertex const& getVertex(void) const noexcept;

            /**
             * Iterates by the vertex's triangles
             *
             * @tparam Iter the type of the inner iterator
             * @tparam Tp the value type used by the iterator
             */
            template <std::bidirectional_iterator Iter, typename Tp>
            class Iterator {
            public:
                using value_type = Tp;
                using reference = Tp&;
                using pointer = Tp*;
                using difference_type = typename Iter::difference_type;
                using iterator_category = std::random_access_iterator_tag;

                /**
                 * Constructs a new iterator object
                 *
                 * @param iter a constant reference to the inner
                 * iterator
                 * @param mesh a pointer to the parent dynamic mesh
                 */
                explicit Iterator(Iter const& iter, DynamicMesh* mesh)
                    : iter{iter}, mesh{mesh} {}

                /**
                 * Constructs a new iterator object
                 */
                explicit Iterator(void) noexcept = default;

                /**
                 * Increments iterator by one
                 *
                 * @return reference to this object
                 */
                Iterator& operator++(void) noexcept
                    { ++iter; return *this; }

                /**
                 * Post-increments iterator by one and returns copy
                 * of the object
                 *
                 * @return the copied object
                 */
                [[nodiscard]] Iterator operator++(int) noexcept
                    { return Iterator{iter--}; }

                /**
                 * Decrements iterator by one
                 *
                 * @return reference to this object
                 */
                Iterator& operator--(void) noexcept
                    { --iter; return *this; }

                /**
                 * Post-decrements iterator by one and returns copy
                 * of the object
                 *
                 * @return the copied object
                 */
                [[nodiscard]] Iterator operator--(int) noexcept
                    { return Iterator{iter--}; }

                /**
                 * Returns a reference to the current indices triangle
                 *
                 * @return the reference to the current indices
                 * triangle
                 */
                [[nodiscard]] reference operator*(void) const noexcept
                    { return mesh->indices[*iter]; }

                /**
                 * Returns a pointer to the current indices triangle
                 *
                 * @return the pointer to the current indices
                 * triangle
                 */
                [[nodiscard]] pointer operator->(void) const noexcept
                    { return &mesh->indices[*iter]; }

                /**
                 * Increments iterator by the given distance
                 *
                 * @param offset the incremented distance
                 * @return reference to this object
                 */
                Iterator& operator+=(difference_type offset) noexcept
                    { iter += offset; return *this; }

                /**
                 * Decrements iterator by the given distance
                 *
                 * @param offset the decremented distance
                 * @return reference to this object
                 */
                Iterator& operator-=(difference_type offset) noexcept
                    { iter += offset; return *this; }

                /**
                 * Returns an indices triangle shifted by the
                 * given offset
                 *
                 * @param offset the incremented distance
                 * @return the indices triangle shifted by the
                 * given offse
                 */
                [[nodiscard]] reference operator[](
                    difference_type offset) const noexcept
                        { return mesh->indices[iter[offset]]; }

                /**
                 * Adds given distance to an iterator
                 *
                 * @param left the iterator
                 * @param right the distance
                 * @return the shifted iterator
                 */
                [[nodiscard]] friend Iterator operator+ (
                    Iterator const& left,
                    difference_type right) noexcept
                        { return Iterator{left.iter + right}; }

                /**
                 * Adds given distance to an iterator
                 *
                 * @param left the distance
                 * @param right the iterator
                 * @return the shifted iterator
                 */
                [[nodiscard]] friend Iterator operator+ (
                    difference_type left,
                    Iterator const& right) noexcept
                        { return Iterator{left + right.iter}; }

                /**
                 * Subtracts given distance from iterator
                 *
                 * @param left the iterator
                 * @param right the distance
                 * @return the shifted operator
                 */
                [[nodiscard]] friend Iterator operator- (
                    Iterator const& left,
                    difference_type right) noexcept
                        { return Iterator{left.iter - right}; }

                /**
                 * Returns distance between iterators
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return difference_type
                 */
                [[nodiscard]] friend difference_type operator- (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter - right.iter; }

                /**
                 * Compares two iterators to each other
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return the result of compare
                 */
                [[nodiscard]] friend auto operator<=> (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter <=> right.iter; }

                /**
                 * Checks whether two iterators are equal
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return whether two iterators are equal
                 */
                [[nodiscard]] friend bool operator== (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter == right.iter; }

                friend class DynamicMesh;
            private:
                Iter                                iter;
                DynamicMesh*                        mesh = nullptr;
            };

            /**
             * Returns a reference to the indices triangle with
             * the given index
             *
             * @param index the triangle's index
             * @return the reference to the indices triangle with
             * the given index
             */
            [[nodiscard]] IndicesTriangle& operator[] (
                std::size_t index) noexcept;

            /**
             * Returns a constant reference to the indices triangle
             * with the given index
             *
             * @param index the triangle's index
             * @return the constant reference to the indices triangle
             * with the given index
             */
            [[nodiscard]] IndicesTriangle const& operator[] (
                std::size_t index) const noexcept;

            /**
             * Returns a reference to the first indices triangle
             *
             * @return the reference to the first indices triangle
             */
            [[nodiscard]] IndicesTriangle& front(void) noexcept;

            /**
             * Returns a constant reference to the first indices
             * triangle
             *
             * @return the constant reference to the first indices
             * triangle
             */
            [[nodiscard]] IndicesTriangle const& front(
                void) const noexcept;

            /**
             * Returns a reference to the last indices triangle
             *
             * @return the reference to the last indices triangle
             */
            [[nodiscard]] IndicesTriangle& back(void) noexcept;

            /**
             * Returns a constant reference to the last indices
             * triangle
             *
             * @return the constant reference to the last indices
             * triangle
             */
            [[nodiscard]] IndicesTriangle const& back(
                void) const noexcept;
        private:
            using base_iterator = Iterator<
                typename IndicesVector::iterator, IndicesTriangle>;
            using reverse_base_iterator = Iterator<
                typename IndicesVector::reverse_iterator, IndicesTriangle>;
        public:
            using iterator = AccessRegisteringIterator<base_iterator>;
            using const_iterator = Iterator<
                typename IndicesVector::const_iterator,
                IndicesTriangle const>;
            using reverse_iterator = AccessRegisteringIterator<
                reverse_base_iterator>;
            using const_reverse_iterator = Iterator<
                typename IndicesVector::const_reverse_iterator,
                IndicesTriangle const>;

            /**
             * Returns the number of the indices triangles in the view
             *
             * @return the number of the indices triangles
             */
            [[nodiscard]] std::size_t size(void) const noexcept
                { return indicesIDs.size(); }

            /**
             * Returns an iterator to the begining of the indices
             * triangles
             *
             * @return the iterator to the begining of the indices
             * triangles
             */
            [[nodiscard]] iterator begin(void) noexcept;

            /**
             * Returns an iterator to the end of the indices
             * triangles
             *
             * @return the iterator to the end of the indices
             * triangles
             */
            [[nodiscard]] iterator end(void) noexcept;

            /**
             * Returns a constant iterator to the begining of
             * the indices triangles
             *
             * @return the constant iterator to the begining of
             * the indices triangles
             */
            [[nodiscard]] const_iterator begin(void) const noexcept;

            /**
             * Returns a constant iterator to the end of
             * the indices triangles
             *
             * @return the constant iterator to the end of
             * the indices triangles
             */
            [[nodiscard]] const_iterator end(void) const noexcept;

            /**
             * Returns a constant iterator to the begining of
             * the indices triangles
             *
             * @return the constant iterator to the begining of
             * the indices triangles
             */
            [[nodiscard]] const_iterator cbegin(void) const noexcept;

            /**
             * Returns a constant iterator to the end of
             * the indices triangles
             *
             * @return the constant iterator to the end of
             * the indices triangles
             */
            [[nodiscard]] const_iterator cend(void) const noexcept;

            /**
             * Returns a reverse iterator to the begining of the
             * indices triangles
             *
             * @return the reverse iterator to the begining of the
             * indices triangles
             */
            [[nodiscard]] reverse_iterator rbegin(void) noexcept;

            /**
             * Returns a reverse iterator to the end of the
             * indices triangles
             *
             * @return the reverse iterator to the end of the
             * indices triangles
             */
            [[nodiscard]] reverse_iterator rend(void) noexcept;

            /**
             * Returns a constant reverse iterator to the begining of
             * the indices triangles
             *
             * @return the constant reverse iterator to the begining of
             * the indices triangles
             */
            [[nodiscard]] const_reverse_iterator rbegin(
                void) const noexcept;

            /**
             * Returns a constant reverse iterator to the end of
             * the indices triangles
             *
             * @return the constant reverse iterator to the end of
             * the indices triangles
             */
            [[nodiscard]] const_reverse_iterator rend(
                void) const noexcept;

            /**
             * Returns a constant reverse iterator to the begining of
             * the indices triangles
             *
             * @return the constant reverse iterator to the begining of
             * the indices triangles
             */
            [[nodiscard]] const_reverse_iterator crbegin(
                void) const noexcept;

            /**
             * Returns a constant reverse iterator to the end of
             * the indices triangles
             *
             * @return the constant reverse iterator to the end of
             * the indices triangles
             */
            [[nodiscard]] const_reverse_iterator crend(
                void) const noexcept;

            /**
             * Reserves space for the future indices trinagles
             *
             * @param size the size of the reserved space
             */
            void reserve(std::size_t size);

            /**
             * Deallocates space unused by the underlying vector
             */
            void shrinkToFit(void);

            /**
             * Returns whether the view containts any indices
             * triangles
             *
             * @return if the view containts any indices
             * triangles
             */
            [[nodiscard]] bool empty(void) const noexcept;

            friend class DynamicMesh;

            /**
             * Destroys the vertex view object
             */
            ~VertexView(void) noexcept = default;
        private:
            using MeshRef = std::reference_wrapper<DynamicMesh>;

            /**
             * Construct a new vertex view object refering the
             * given parent and vertex
             *
             * @param parent a reference to the parent dynamic
             * mesh object
             * @param vertexID an index of the displayed vertex
             */
            explicit VertexView(
                DynamicMesh& parent,
                uint32 vertexID);

            /**
             * Emplaces indices triangle in the view
             *
             * @param triangleID an index of the emplaced
             * indices triangle
             */
            void emplaceTriangle(uint32 triangleID);

            MeshRef                                 parent;
            IndicesVector                           indicesIDs;
            uint32                                  vertexID;
        };
    private:
        using VerticesView = std::deque<VertexView>;
    public:
        /**
         * Returns a reference to the vertex view with the given
         * index
         *
         * @param index an index of the vertex view
         * @return the reference to the vertex view with the given
         * index
         */
        [[nodiscard]] VertexView& operator[] (
            std::size_t index) noexcept;

        /**
         * Returns constant a reference to the vertex view with the
         * given index
         *
         * @param index an index of the vertex view
         * @return the constant reference to the vertex view with the
         * given index
         */
        [[nodiscard]] VertexView const& operator[] (
            std::size_t index) const noexcept;

        /**
         * Returns a reference to the first vertex view
         *
         * @return the reference to the first vertex view
         */
        [[nodiscard]] VertexView& front(void) noexcept;

        /**
         * Returns a constant reference to the first vertex view
         *
         * @return the constant reference to the first vertex view
         */
        [[nodiscard]] VertexView const& front(void) const noexcept;

        /**
         * Returns a reference to the last vertex view
         *
         * @return the reference to the last vertex view
         */
        [[nodiscard]] VertexView& back(void) noexcept;

        /**
         * Returns a constant reference to the last vertex view
         *
         * @return the constant reference to the last vertex view
         */
        [[nodiscard]] VertexView const& back(void) const noexcept;

        using iterator = typename VerticesView::iterator;
        using const_iterator = typename VerticesView::const_iterator;
        using reverse_iterator
            = typename VerticesView::reverse_iterator;
        using const_reverse_iterator
            = typename VerticesView::const_reverse_iterator;

        /**
         * Returns the number of vertices in the object
         *
         * @return the number of vertices
         */
        [[nodiscard]] std::size_t size(void) const noexcept
            { return verticesView.size(); }

        /**
         * Returns an iterator to the begining of the vertices
         *
         * @return the iterator to the begining of the vertices
         */
        [[nodiscard]] iterator begin(void) noexcept;

        /**
         * Returns an iterator to the end of the vertices
         *
         * @return the iterator to the end of the vertices
         */
        [[nodiscard]] iterator end(void) noexcept;

        /**
         * Returns a constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator begin(void) const noexcept;

        /**
         * Returns a constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator end(void) const noexcept;

        /**
         * Returns a constant iterator to the begining
         * of the vertices
         *
         * @return the constant iterator to the begining
         * of the vertices
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept;

        /**
         * Returns a constant iterator to the end
         * of the vertices
         *
         * @return the constant iterator to the end
         * of the vertices
         */
        [[nodiscard]] const_iterator cend(void) const noexcept;

        /**
         * Returns a reverse iterator to the end of
         * the vertices
         *
         * @return a reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept;

        /**
         * Returns a reverse iterator to the begining of
         * the vertices
         *
         * @return the reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept;

        /**
         * Returns a constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept;

        /**
         * Returns a constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator rend(
            void) const noexcept;

        /**
         * Returns a constant reverse iterator to the end of
         * the vertices
         *
         * @return the constant reverse iterator to the end of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept;

        /**
         * Returns a constant reverse iterator to the begining of
         * the vertices
         *
         * @return the constant reverse iterator to the begining of
         * the vertices
         */
        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept;

        /**
         * Reserves space for the future vertices
         *
         * @param size the size of the reserved space
         */
        void reserve(std::size_t size);

        /**
         * Deallocates space unused by the underlying vector
         */
        void shrinkToFit(void);

        /**
         * Checks whether the dynamic mesh contains no vertices
         *
         * @return if the dynamic mesh contains no vertices
         */
        [[nodiscard]] bool empty(void) const noexcept;

        /**
         * Pushes a new vertex into the dynamic mesh
         *
         * @param vertex a constant reference to the vertex
         */
        void push(Vertex const& vertex);

        /**
         * Pushes a new vertex into the dynamic mesh
         *
         * @param vertex a rvalue reference to the vertex
         */
        void push(Vertex&& vertex);

        /**
         * Emplaces a new vertex in the dynamic mesh
         *
         * @tparam Args the argument types
         * @param args an arguments used to create a new vertex
         */
        template <typename... Args>
            requires std::constructible_from<Vertex, Args...>
        void emplace(Args&&... args);

        /**
         * Erases vertex lying under the given iterator
         *
         * @param position a constant reference to the iterator
         * containing erased vertex's position
         */
        void erase(iterator const& position);

        /**
         * Erases vertex lying under the given constant iterator
         *
         * @param position a constant reference to the constant
         * iterator containing erased vertex's position
         */
        void erase(const_iterator const& position);

        /**
         * Erases vertices lying within the range specified
         * by the two iterators
         *
         * @param first a constant reference to the iterator to the
         * first vertex of the erased range
         * @param last a constant reference to the iterator to the
         * last vertex of the erased range
         */
        void erase(
            iterator const& first,
            iterator const& last);

        /**
         * Erases vertices lying within the range specified
         * by the two constant iterators
         *
         * @param first a constant reference to the constant iterator
         * to the first vertex of the erased range
         * @param last a constant reference to the constant iterator
         * to the last vertex of the erased range
         */
        void erase(
            const_iterator const& first,
            const_iterator const& last);

        /**
         * Pushes a new indices triangle into the mesh
         *
         * @param triangle a constant reference to the indices
         * triangle
         */
        void pushIndex(IndicesTriangle const& triangle);

        /**
         * Emplaces a new indices triangle in the mesh
         *
         * @param firstVertex the first vertex's index
         * @param secondVertex the second vertex's index
         * @param thirdVertex the third vertex's index
         */
        void emplaceIndex(
            uint32 firstVertex,
            uint32 secondVertex,
            uint32 thirdVertex);

        /**
         * Erases indices triangle lying under the given iterator
         *
         * @param position a constant reference to the iterator
         * containing erased triangle's position
         */
        void eraseIndex(typename VertexView::iterator const& position);

        /**
         * Erases indices triangle lying under the given constant
         * iterator
         *
         * @param position a constant reference to the constant
         * iterator containing erased triangle's position
         */
        void eraseIndex(typename VertexView::const_iterator const& position);

        /**
         * Erases indices triangles lying within the range specified
         * by the two iterators
         *
         * @param first a constant reference to the iterator to the
         * first triangle of the erased range
         * @param last a constant reference to the iterator to the
         * last triangle of the erased range
         */
        void eraseIndex(
            typename VertexView::iterator first,
            typename VertexView::iterator const& last);

        /**
         * Erases indices triangles lying within the range specified
         * by the two constant iterators
         *
         * @param first a constant reference to the constant iterator
         * to the first triangle of the erased range
         * @param last a constant reference to the constant iterator
         * to the last triangle of the erased range
         */
        void eraseIndex(
            typename VertexView::const_iterator first,
            typename VertexView::const_iterator const& last);

        /**
         * Destroys the dynamic mesh object
         */
        ~DynamicMesh(void) noexcept = default;
    private:
        using EmptyVertices = std::deque<uint32>;

        /**
         * Reloads the element array buffer
         */
        void reloadElementBuffer(void) const noexcept;

        /**
         * Reloads the vertex buffer object
         */
        void reloadVertexBuffer(void) const noexcept;

        /**
         * Reshapes the vertex buffer object
         */
        void reshapeVertexBuffer(void) const noexcept;

        /**
         * Initializes buffers used by the dynamic mesh
         */
        void initializeBuffers(void) const noexcept;

        /**
         * Actualizes vertex buffer before draw
         */
        void actualizeBufferBeforeDraw(void) const noexcept;

        /**
         * Builds vertex views vector
         */
        void buildVertexViews(void);

        /**
         * Moves a vertex view from the old adress to the new one
         *
         * @param oldAddress the old vertex view's adress
         * @param newAddress the new vertex view's adress
         */
        void adjustVertexView(
            uint32 oldAddress,
            uint32 newAddress) noexcept;

        /**
         * Returns a new address of the vertex
         *
         * @param vertexID an old address of the vertex
         * @return the new address of the vertex
         */
        [[nodiscard]] uint32 getAddress(
            uint32 vertexID) const noexcept;

        /**
         * Recognizes the given vertex as an empty one
         *
         * @param vertexID the vertex's index
         */
        void addEmptySpace(uint32 vertexID);

        /**
         * Erases view indices lying under the given iterator
         *
         * @tparam Iter the iterator's type
         * @param position a constant reference to the position
         * iterator
         */
        template <std::random_access_iterator Iter>
        void eraseViewIndices(Iter const& position);

        Vertices                                    vertices;
        Indices                                     indices;
        EmptyVertices                               emptyVertices;
        VerticesView                                verticesView;
        ElementArrayBuffer                          elementBuffer;
        bool mutable                                isExtended = false;
        bool mutable                                isIndiciesChanged = false;
    };

    template class DynamicMesh<void>;
    template class DynamicMesh<uint8>;
    template class DynamicMesh<uint16>;
    template class DynamicMesh<uint32>;
    template class DynamicMesh<int8>;
    template class DynamicMesh<int16>;
    template class DynamicMesh<int32>;
    template class DynamicMesh<int64>;

}

#include <MPGL/Core/Figures/Meshes/DynamicMesh.tpp>
