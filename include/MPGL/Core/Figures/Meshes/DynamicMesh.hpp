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

#include <list>

namespace mpgl {

    template <MeshTraitSpecifier Spec = void>
    class DynamicMesh : public Figure3D {
    public:
        using Vertex = MeshVertexT<Spec>;
        using Vertices = std::vector<Vertex>;
        using Indices = std::vector<IndicesTriangle>;

        DynamicMesh(
            Vertices&& vertices,
            Indices&& indices,
            std::string const& program);

        template <
            std::ranges::input_range VRange,
            std::ranges::input_range IRange>
        requires (SameRangeType<VRange, Vertex>,
            SameRangeType<IRange, IndicesTriangle>)
        DynamicMesh(
            VRange&& vertices,
            IRange&& indices,
            std::string const& program);

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

        DynamicMesh(DynamicMesh const& dynamicMesh);

        DynamicMesh(DynamicMesh&& dynamicMesh) noexcept = default;

        DynamicMesh& operator=(DynamicMesh const& dynamicMesh);

        DynamicMesh& operator=(
            DynamicMesh&& dynamicMesh) noexcept = default;

        virtual void draw(void) const noexcept;

        virtual void transform(
            Transformation3D const& transformator) noexcept;

        class VertexView {
        private:
            using IndicesVector = std::vector<uint32>;
        public:
            VertexView(VertexView const&) = default;
            VertexView(VertexView&&) noexcept = default;

            VertexView& operator=(VertexView const&) = default;
            VertexView& operator=(VertexView&&) noexcept = default;

            [[nodiscard]] Vertex& getVertex(void) noexcept;

            [[nodiscard]] Vertex const& getVertex(void) const noexcept;

            template <std::bidirectional_iterator Iter, typename Tp>
            class Iterator {
            public:
                using value_type = Tp;
                using reference = Tp&;
                using pointer = Tp*;
                using difference_type = typename Iter::difference_type;
                using iterator_category = std::random_access_iterator_tag;

                explicit Iterator(Iter const& iter, DynamicMesh* mesh)
                    : iter{iter}, parent{mesh} {}

                explicit Iterator(void) noexcept = default;

                Iterator& operator++(void) noexcept
                    { ++iter; return *this; }

                [[nodiscard]] Iterator operator++(int) noexcept
                    { return Iterator{iter--}; }

                Iterator& operator--(void) noexcept
                    { --iter; return *this; }

                [[nodiscard]] Iterator operator--(int) noexcept
                    { return Iterator{iter--}; }

                [[nodiscard]] reference operator*(void) const noexcept
                    { return parent->indices[*iter]; }

                [[nodiscard]] pointer operator->(void) const noexcept
                    { return &parent->indices[*iter]; }

                Iterator& operator+=(difference_type offset) noexcept
                    { iter += offset; return *this; }

                Iterator& operator-=(difference_type offset) noexcept
                    { iter += offset; return *this; }

                [[nodiscard]] reference operator[](
                    difference_type offset) const noexcept
                        { return parent->indices[iter[offset]]; }

                [[nodiscard]] friend Iterator operator+ (
                    Iterator const& left,
                    difference_type right) noexcept
                        { return Iterator{left.iter + right}; }

                [[nodiscard]] friend Iterator operator+ (
                    difference_type left,
                    Iterator const& right) noexcept
                        { return Iterator{left + right.iter}; }

                [[nodiscard]] friend Iterator operator- (
                    Iterator const& left,
                    difference_type right) noexcept
                        { return Iterator{left.iter - right}; }

                [[nodiscard]] friend difference_type operator- (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter - right.iter; }

                [[nodiscard]] friend auto operator<=> (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter <=> right.iter; }

                [[nodiscard]] friend bool operator== (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter == right.iter; }
            private:
                Iter                                iter;
                DynamicMesh*                        mesh = nullptr;
            };

            [[nodiscard]] IndicesTriangle& operator[] (
                std::size_t index) noexcept;

            [[nodiscard]] IndicesTriangle const& operator[] (
                std::size_t index) const noexcept;

            [[nodiscard]] IndicesTriangle& front(void) noexcept;

            [[nodiscard]] IndicesTriangle const& front(
                void) const noexcept;

            [[nodiscard]] IndicesTriangle& back(void) noexcept;

            [[nodiscard]] IndicesTriangle const& back(
                void) const noexcept;

            using iterator = Iterator<
                typename IndicesVector::iterator, IndicesTriangle>;
            using const_iterator = Iterator<
                typename IndicesVector::const_iterator,
                IndicesTriangle const>;
            using reverse_iterator = Iterator<
                typename IndicesVector::reverse_iterator, IndicesTriangle>;
            using const_reverse_iterator = Iterator<
                typename IndicesVector::const_reverse_iterator,
                IndicesTriangle const>;

            [[nodiscard]] std::size_t size(void) const noexcept
                { return indicesIDs.size(); }

            [[nodiscard]] iterator begin(void) noexcept;

            [[nodiscard]] iterator end(void) noexcept;

            [[nodiscard]] const_iterator begin(void) const noexcept;

            [[nodiscard]] const_iterator end(void) const noexcept;

            [[nodiscard]] const_iterator cbegin(void) const noexcept;

            [[nodiscard]] const_iterator cend(void) const noexcept;

            [[nodiscard]] reverse_iterator rbegin(void) noexcept;

            [[nodiscard]] reverse_iterator rend(void) noexcept;

            [[nodiscard]] const_reverse_iterator rbegin(
                void) const noexcept;

            [[nodiscard]] const_reverse_iterator rend(
                void) const noexcept;

            [[nodiscard]] const_reverse_iterator crbegin(
                void) const noexcept;

            [[nodiscard]] const_reverse_iterator crend(
                void) const noexcept;

            void reserve(std::size_t size);

            void shrinkToFit(void);

            [[nodiscard]] bool empty(void) const noexcept;

            friend class DynamicMesh;

            ~VertexView(void) noexcept = default;
        private:
            explicit VertexView(
                DynamicMesh& parent,
                uint32 vertexID);

            void emplaceTriangle(uint32 triangleID);

            DynamicMesh&                            parent;
            IndicesVector                           indicesIDs;
            uint32                                  vertexID;
        };
    private:
        using VerticesView = std::vector<VertexView>;
    public:
        [[nodiscard]] VertexView& operator[] (
            std::size_t index) noexcept;

        [[nodiscard]] VertexView const& operator[] (
            std::size_t index) const noexcept;

        [[nodiscard]] VertexView& front(void) noexcept;

        [[nodiscard]] VertexView const& front(void) const noexcept;

        [[nodiscard]] VertexView& back(void) noexcept;

        [[nodiscard]] VertexView const& back(void) const noexcept;

        using iterator = typename VerticesView::iterator;
        using const_iterator = typename VerticesView::const_iterator;
        using reverse_iterator
            = typename VerticesView::reverse_iterator;
        using const_reverse_iterator
            = typename VerticesView::const_reverse_iterator;

        [[nodiscard]] std::size_t size(void) const noexcept;

        [[nodiscard]] iterator begin(void) noexcept;

        [[nodiscard]] iterator end(void) noexcept;

        [[nodiscard]] const_iterator begin(void) const noexcept;

        [[nodiscard]] const_iterator end(void) const noexcept;

        [[nodiscard]] const_iterator cbegin(void) const noexcept;

        [[nodiscard]] const_iterator cend(void) const noexcept;

        [[nodiscard]] reverse_iterator rbegin(void) noexcept;

        [[nodiscard]] reverse_iterator rend(void) noexcept;

        [[nodiscard]] const_reverse_iterator rbegin(
            void) const noexcept;

        [[nodiscard]] const_reverse_iterator rend(
            void) const noexcept;

        [[nodiscard]] const_reverse_iterator crbegin(
            void) const noexcept;

        [[nodiscard]] const_reverse_iterator crend(
            void) const noexcept;

        void reserve(size_type size);

        void shrinkToFit(void);

        [[nodiscard]] bool empty(void) const noexcept;

        void push(Vertex const& vertex);

        void push(Vertex&& vertex);

        template <typename... Args>
            requires std::constructible_from<Vertex, Args...>
        void emplace(Args&&... args);

        void pop(void) noexcept;

        void erase(iterator const& position);

        void erase(const_iterator const& position);

        void erase(
            iterator const& first,
            iterator const& last);

        void erase(
            const_iterator const& first,
            const_iterator const& last);

        void pushTriangle(IndicesTriangle const& triangle);

        void emplaceTriangle(
            uint32 firstVertex,
            uint32 secondVertex,
            uint32 thirdVertex);

        void eraseTriangle(VertexView::iterator const& position);

        void eraseTriangle(VertexView::const_iterator const& position);

        void eraseTriangle(
            VertexView::iterator const& first,
            VertexView::iterator const& last);

        void eraseTriangle(
            VertexView::const_iterator const& first,
            VertexView::const_iterator const& last);

        ~DynamicMesh(void) noexcept = default;
    private:
        using EmptyVerticesList = std::list<uint32>;

        void reloadElementBuffer(void) const noexcept;

        void initializeBuffers(void) const noexcept;

        void actualizeBufferBeforeDraw(void) const noexcept;

        void buildVertexViews(void);

        Vertices                                    vertices;
        Indices                                     indices;
        EmptyVerticesList                           emptyVertices;
        VerticesView                                verticesView;
        ElementArrayBuffer                          elementBuffer;
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
