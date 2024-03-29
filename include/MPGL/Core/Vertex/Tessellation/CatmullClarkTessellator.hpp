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

#include <MPGL/Core/Vertex/Indices/IndicesTetragon.hpp>
#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Core/Vertex/VertexTraits.hpp>
#include <MPGL/Core/Vertex/VertexCast.hpp>

#include <unordered_map>
#include <map>

namespace mpgl {

    /**
     * Tessellator that uses the catmull-clark subdivision algorithm in
     * order to tessellate tetragons
     */
    class CatmullClarkTessellator {
    public:

        /**
         * Perfroms tessellation on the given tetragon mesh by the given
         * number of times and returns the tessellated mesh
         *
         * @tparam VRange the vertices range type
         * @tparam IRange the inidcies range type
         * @tparam Predicate the tessellation predicate type
         * @param vertices the vertices range
         * @param indices the indices range
         * @param tessellationSteps the number of tessellation steps
         * @param pred the tessellation predicate
         */
        template <
            FlexibleRange VRange,
            UnderlyingRange<IndicesTetragon> IRange,
            std::invocable<Vector3f const&> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate, Vector3f const&>>)
        [[nodiscard]] std::pair<VRange, IRange> operator() (
            VRange vertices,
            IRange indices,
            uint8 tessellationSteps,
            Predicate pred) const;
    private:
        /**
         * Inner representation of tessellation algorithm that helps
         * in implementation
         *
         * @tparam VRange the vertices range type
         * @tparam IRange the inidcies range type
         * @tparam Predicate the tessellation predicate type
         */
        template <
            FlexibleRange VRange,
            UnderlyingRange<IndicesTetragon> IRange,
            std::invocable<Vector3f const&> Predicate>
                requires (
                    VertexType<std::ranges::range_value_t<VRange>>
                    && SameRangeType<VRange, std::invoke_result_t<
                        Predicate, Vector3f const&>>)
        class Algorithm {
        public:
            /**
             * Constructs a new algorithm object
             *
             * @param predicate the tessellation predicate
             */
            explicit Algorithm(Predicate builder);

            /**
             * Performs tessellation on the given indices and vertices
             *
             * @param vertices a constant reference to the vertices range
             * @param indices a constant reference to the indices
             * range
             * @return a tessellated tetragon mesh
             */
            [[nodiscard]] std::pair<VRange, IRange> operator() (
                VRange const& vertices,
                IRange const& indices);
        private:
            using Vertex = std::ranges::range_value_t<VRange>;
            using VertexAdapter = std::remove_cvref_t<decltype(
                std::declval<Vertex>() | cast::position)>;
            using Vector = typename VertexAdapter::value_type;

            /**
             * Represents an edge in the tetragon mesh
             */
            struct Edge {
                uint32                                  vertex;
                uint32                                  firstFaceID;
                uint32                                  secondFaceID;
            };

            /**
             * Represents a token used to find adjacent tetragons
             */
            struct Token {
                Edge const*                             edge;
                uint32                                  face;
            };

            using HashMap = std::unordered_map<uint64, uint32>;
            using EdgeMap = std::map<uint64, Edge>;
            using Edges = std::vector<Edge const*>;
            using VerticesGraph = std::map<uint32, Edges>;
            using Tokens = std::vector<Token>;

            VRange                                      vertices;
            IRange                                      indices;
            VerticesGraph                               graph;
            EdgeMap                                     edges;
            HashMap                                     edgeFaces;
            Predicate                                   builder;

            /**
             * Builds a tetragons face vertices
             *
             * @param vertices a constant reference to the vertices range
             * @param indices a constant reference to the indices
             * range
             */
            void buildFaces(
                VRange const& vertices,
                IRange const& indices);

            /**
             * Builds a tetragons edge vertices
             *
             * @param vertices a constant reference to the vertices range
             * @param indices a constant reference to the indices
             * range
             */
            void buildEdges(
                VRange const& vertices,
                IRange const& indices);

            /**
             * Builds a tetragon edge vertex
             *
             * @param vertices a constant reference to the vertices range
             * @param tetragonID an index of the tetragon
             * @param firstVertex an index of the edge's first vertex
             * @param secondVertex an index of the edge's second vertex
             */
            void buildEdge(
                VRange const& vertices,
                uint32 tetragonID,
                uint32 firstVertex,
                uint32 secondVertex);

            /**
             * Builds a tetragon edge vertices that lies on the border
             *
             * @param vertices a constant reference to the vertices range
             */
            void buildBorderEdges(VRange const& vertices);

            /**
             * Generates vertices dependencies graph
             */
            void generateVerticesDependencies(void);

            /**
             * Adds an edge to the vertices dependencies graph
             *
             * @param vertex an index of the vertex
             * @param edgePtr a pointer to the constant edge object
             */
            void addToGraph(uint32 vertex, Edge const* edgePtr);

            /**
             * Calculates tessellated vertices
             *
             * @param vertices a constant reference to the vertices range
             */
            void calculateTessellatedVertices(VRange const& vertices);

            /**
             * Calculates an average of edges vertices
             *
             * @param edges a constant reference to the edges vector
             * @return the average of edges vertices
             */
            [[nodiscard]] Vector averageOfEdges(Edges const& edges);

            /**
             * Calculates an average of faces vertices
             *
             * @param edges a constant reference to the edges vector
             * @return the average of faces vertices
             */
            [[nodiscard]] Vector averageOfFaces(Edges const& edges);

            /**
             * Adds tetragons surrounding given vertex
             *
             * @param vertex an index of the vertex
             * @param edges a reference to the edges vector
             */
            void addTetragonVertices(
                uint32 vertex,
                Edges const& edges);

            /**
             * Generates tokens used to find adjucent tetragons
             *
             * @param edges a reference to the edges vector
             * @return a token range
             */
            [[nodiscard]] Tokens generateTokens(Edges const& edges);

            /**
             * Adds tetragon lying between two edges and locked with
             * the vertex
             *
             * @param vertex an index of the vertex
             * @param firstToken a constant reference to the first
             * token
             * @param secondToken a constant reference to the
             * second token
             */
            void addTetragon(
                uint32 vertex,
                Token const& firstToken,
                Token const& secondToken);

            /**
             * Calculates a position of the vertex lying between
             * indicated vertices
             *
             * @param vertices a constant reference to the vertices
             * range
             * @param firstIndex an index of the first vertex
             * @param secondIndex an index of the second vertex
             * @return the position of the vertex lying between
             * indicated vertices
             */
            [[nodiscard]] Vertex calculateVertex(
                VRange const& vertices,
                uint32 firstIndex,
                uint32 secondIndex);

            /**
             * Calculates a position of the vertex lying between
             * indicated vertices
             *
             * @param vertices a constant reference to the vertices
             * range
             * @param firstIndex an index of the first vertex
             * @param secondIndex an index of the second vertex
             * @param thirdIndex an index of the third vertex
             * @param fourthIndex an index of the fourth vertex
             * @return the position of the vertex lying between
             * indicated vertices
             */
            [[nodiscard]] Vertex calculateVertex(
                VRange const& vertices,
                uint32 firstIndex,
                uint32 secondIndex,
                uint32 thirdIndex,
                uint32 fourthIndex);

            /**
             * Calculates a position of the vertex lying between
             * indicated vertices on the mesh's edge
             *
             * @param vertices a constant reference to the vertices
             * range
             * @param firstIndex an index of the first vertex
             * @param secondIndex an index of the second vertex
             * @param thirdIndex an index of the third vertex [face]
             * @param fourthIndex an index of the fourth vertex [face]
             * @return the position of the vertex lying between
             * indicated vertices
             */
            [[nodiscard]] Vertex calculateEdgeVertex(
                VRange const& vertices,
                uint32 firstIndex,
                uint32 secondIndex,
                uint32 thirdIndex,
                uint32 fourthIndex);

            /**
             * Generates edge's tag
             *
             * @param first an index of the first vertex
             * @param second an index of the second vertex
             * @return the edge' tag
             */
            [[nodiscard]] static uint64 generateTag(
                uint64 first,
                uint64 second) noexcept;
        };

        static constexpr uint64                         LowerMask
            = 0x00000000FFFFFFFF;
        static constexpr uint32                         Placeholder
            = 0xFFFFFFFF;
    };

    inline constexpr CatmullClarkTessellator            catmullClarkTessellator;

}

#include <MPGL/Core/Vertex/Tessellation/CatmullClarkTessellator.tpp>
