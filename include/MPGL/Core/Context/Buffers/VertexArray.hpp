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

#include <MPGL/Core/Context/Context.hpp>
#include <MPGL/Core/Vertex/Vertex.hpp>

namespace mpgl {

    /**
     * Buffers Management declaration
     */
    class BuffersManagement;

    /**
     * Manages the handle to the OpenGL vertex array object
     */
    class VertexArray : private GraphicalObject {
    public:
        /**
         * Available vertex array drawing modes
         */
        enum class DrawMode : uint16 {
            /// Draws points
            Points                                = GL_POINTS,
            /// Draws lines
            Lines                                 = GL_LINES,
            /// Draws line strip
            LineStrip                             = GL_LINE_STRIP,
            /// Draws line loop
            LineLoop                              = GL_LINE_LOOP,
            /// Draws adjacent line strip
            LineStripAdjacency                    = GL_LINE_STRIP_ADJACENCY,
            /// Draws adjacent lines
            LinesAdjacency                        = GL_LINES_ADJACENCY,
            /// Draws triangle strip
            TriangleStrip                         = GL_TRIANGLE_STRIP,
            /// Draws triangle fan
            TriangleFan                           = GL_TRIANGLE_FAN,
            /// Draws triangles
            Triangles                             = GL_TRIANGLES,
            /// Draws adjacent triangle strip
            TriangleStripAdjacency                = GL_TRIANGLE_STRIP_ADJACENCY,
            /// Draws adjacent triangles
            TrianglesAdjacency                    = GL_TRIANGLES_ADJACENCY,
            /// Draws patches
            Patches                               = GL_PATCHES
        };

        /**
         * Construct a new Vertex Array object. Allocates
         * the new OpenGL vertex array object
         */
        explicit VertexArray(void) noexcept;

        VertexArray(VertexArray const&) = delete;

        /**
         * Construct a new Vertex Array object from the given
         * rvalue Vertex Array object
         *
         * @param array the given rvalue vertex array object
         */
        VertexArray(VertexArray&& array) noexcept;

        VertexArray& operator=(VertexArray const&) = delete;

        /**
         * Assigns the given rvalue Vertex Array object
         *
         * @param array the given rvalue vertex array object
         * @return reference to this object
         */
        VertexArray& operator=(VertexArray&& array) noexcept;

        /**
         * Binds the vertex array object
         */
        void bind(void) const noexcept;

        /**
         * Unbinds the vertex array object
         */
        void unbind(void) const noexcept;

        /**
         * Holds a type of vertex
         *
         * @tparam Tp the vertex type
         */
        template <SpecializationOf<Vertex> Tp>
        struct VertexTag {};

        /**
         * Utilizes given vertex type to copy the memory layout
         * to the vertex array object
         *
         * @tparam Tp the vertex type
         */
        template <SpecializationOf<Vertex> Tp>
        void setArrayData(
            [[maybe_unused]] VertexTag<Tp>) const noexcept;

        /**
         * Draws the attached vertex buffer on the screen
         *
         * @param mode the drawing mode
         * @param size the number of drawed vertices
         */
        void drawArrays(DrawMode mode, uint32 size) const noexcept;

        /**
         * Draws the attached vertex buffer and element buffer
         * on the screen
         *
         * @param mode the drawing mode
         * @param size the number of drawed indices
         * @param dataType the type of the indices
         */
        void drawElements(
            DrawMode mode,
            uint32 size,
            DataType dataType) const noexcept;

        /**
         * Draws instances of the attached vertex buffer on the
         * screen
         *
         * @param mode the drawing mode
         * @param size the number of drawed vertices
         * @param instances the number of drawed instances
         */
        void drawInstancedArrays(
            DrawMode mode,
            uint32 size,
            uint32 instances) const noexcept;

        /**
         * Draws instances of the attached vertex buffer and the
         * element buffer on the screen
         *
         * @param mode the drawing mode
         * @param size the number of drawed indices
         * @param dataType the type of the indices
         * @param instances the number of drawed instances
         */
        void drawInstancedElements(
            DrawMode mode,
            uint32 size,
            DataType dataType,
            uint32 instances) const noexcept;

        /**
         * Destroy the Vertex Array object
         */
        ~VertexArray(void) noexcept;

        friend class BuffersManagement;
    private:
        /**
         * Construct a new Vertex Array object. Moves the
         * already existing handle to the vertex array
         *
         * @param arrayID the moved array id
         */
        explicit VertexArray(uint32 arrayID) noexcept;

        /**
         * Returns the reference to the inner buffer
         *
         * @return the reference to the inner buffer
         */
        uint32& getBuffer(void) noexcept
            { return arrayID; }

        /**
         * Destroys the array handled by this object. Does not
         * violate the object lifetime
         */
        void destroyArray(void) noexcept;

        uint32                                      arrayID;
    };

}

#include <MPGL/Core/Context/Buffers/VertexArray.tpp>
