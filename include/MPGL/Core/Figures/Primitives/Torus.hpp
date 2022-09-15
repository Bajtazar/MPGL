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
#include <MPGL/Core/Vertex/Indices/IndicesTetragon.hpp>
#include <MPGL/Core/Figures/Angular.hpp>

namespace mpgl {

    /**
     * Represents a torus figure
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec = void>
    class Torus : public Angular<dim::Dim3, Spec> {
    public:
        using VertexTraits = Angular<dim::Dim3, Spec>::VertexTraits;

        /**
         * Constructs a new torus object
         *
         * @param position a position of the torus
         * @param radius a radius of the torus
         * @param ringRadius a radius of the torus ring
         * @param color a color of the torus
         * @param tessellationSteps a number of tessellation
         * steps performed during torus creation
         */
        Torus(
            Vector3f const& position,
            float32 radius,
            float32 ringRadius,
            Color const& color = colors::white,
            uint8 tessellationSteps = 2);

        /**
         * Constructs a new torus object from the given
         * constant reference to the other object
         *
         * @param torus the constant reference to the other
         * object
         */
        Torus(Torus const& torus);

        Torus(Torus&& torus) noexcept = default;

        /**
         * Assigns an other torus object to this object
         *
         * @param torus a constant reference to the other
         * object
         * @return a reference to this object
         */
        Torus& operator=(Torus const& torus);

        Torus& operator=(Torus&& torus) noexcept = default;

        /**
         * Draws the torus on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Virtual destructor. Destroys the torus object
         */
        virtual ~Torus(void) noexcept = default;
    private:
        using Vertex = typename VertexTraits::Vertex;
        using Indices = std::vector<IndicesTetragon>;
        using Vertices = typename Angular<dim::Dim3, Spec>::Vertices;
        using TessellationResult = std::pair<Vertices, Indices>;

        /**
         * Constructs a new torus object from a pair of the
         * vertices and indices
         *
         * @param result a pair of vertices and indices
         */
        explicit Torus(TessellationResult&& result);

        /**
         * Reloads the element array buffer object
         */
        void reloadElementBuffer(void) const noexcept;

        Indices                                     indices;
        ElementArrayBuffer                          elementBuffer;

        /**
         * Generates base vertices
         *
         * @param position a position of the torus
         * @param radius a radius of the torus
         * @param ringRadius a radius of the torus ring
         * @param color a color of the torus
         * @return the base vertices
         */
        [[nodiscard]] static Vertices generateBaseVertices(
            Vector3f const& position,
            float32 radius,
            float32 ringRadius,
            Color const& color);

        /**
         * Generates one of the rings used as a base for the torus
         *
         * @param vertices a reference to the vertices range
         * @param position a position of the torus
         * @param versor a current ring's versor
         * @param radius a radius of the torus
         * @param ringRadius a radius of the torus ring
         * @param color a color of the torus
         */
        static void generateRing(
            Vertices& vertices,
            Vector3f const& position,
            Vector3f const& versor,
            float32 radius,
            float32 ringRadius,
            Color const& color);

        /**
         * Tessellates the torus base creating a better resolution
         * torus
         *
         * @param position a position of the torus
         * @param radius a radius of the torus
         * @param ringRadius a radius of the torus ring
         * @param color a color of the torus
         * @param tessellationSteps a number of tessellation steps
         * @return a pair of vertices and indices
         */
        [[nodiscard]] static TessellationResult tessellateBase(
            Vector3f const& position,
            float32 radius,
            float32 ringRadius,
            Color const& color,
            uint8 tessellationSteps);

        static Indices const                        BaseIndices;
    };

    template class Torus<void>;

}
