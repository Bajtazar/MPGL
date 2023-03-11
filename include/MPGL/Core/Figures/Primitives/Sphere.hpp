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
#include <MPGL/Core/Vertex/Indices/IndicesTriangle.hpp>
#include <MPGL/Core/Figures/Angular.hpp>

namespace mpgl {

    /**
     * Represents a sphere figure
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec = void>
    class Sphere : public Angular<dim::Dim3, Spec> {
    public:
        using VertexTraits = Angular<dim::Dim3, Spec>::VertexTraits;

        /**
         * Constructs a new sphere object
         *
         * @param position a position of the sphere
         * @param radius a radius of the sphere
         * @param color a color of the sphere
         * @param tessellationSteps a number of tessellation
         * steps performed during sphere creation
         */
        Sphere(
            Vector3f const& position,
            float32 radius,
            Color const& color = colors::white,
            uint8 tessellationSteps = 2);

        /**
         * Constructs a new sphere object from the given
         * constant reference to the other object
         *
         * @param sphere the constant reference to the other
         * object
         */
        Sphere(Sphere const& sphere);

        Sphere(Sphere&& sphere) noexcept = default;

        /**
         * Assigns an other sphere object to this object
         *
         * @param sphere a constant reference to the other
         * sphere object
         * @return a reference to this object
         */
        Sphere& operator=(Sphere const& sphere);

        Sphere& operator=(Sphere&& sphere) noexcept = default;

        /**
         * Draws the sphere on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Virtual destructor. Destroys the sphere object
         */
        virtual ~Sphere(void) noexcept = default;
    private:
        using Vertex = typename VertexTraits::Vertex;
        using Indices = std::vector<IndicesTriangle>;
        using Vertices = typename Angular<dim::Dim3, Spec>::Vertices;
        using TessellationResult = std::pair<Vertices, Indices>;

        /**
         * Constructs a new sphere object from a pair of the
         * vertices and indices
         *
         * @param result a pair of vertices and indices
         */
        explicit Sphere(TessellationResult&& result);

        /**
         * Reloads the element array buffer object
         */
        void reloadElementBuffer(void) const noexcept;

        Indices                                     indices;
        ElementArrayBuffer                          elementBuffer;

        /**
         * Tessellates icosahedron ensuring that newly created
         * vertices will be in valid radius from sphere center
         *
         * @param position a position of the sphere
         * @param radius a radius of the sphere
         * @param color a color of the sphere
         * @param steps a number of tessellation steps
         * @return a pair of vertices and indices
         */
        [[nodiscard]] static TessellationResult tessellateIcosahedron(
            Vector3f const& position,
            float32 radius,
            Color const& color,
            uint8 steps);

        /**
         * Generates icosahedron vertices
         *
         * @param position a position of the origin
         * @param radius a radius of the icosahedron
         * @param color a color of the vertices
         * @return the icosahedron vertices
         */
        [[nodiscard]] static Vertices generateIcosahedron(
            Vector3f const& position,
            float32 radius,
            Color const& color);

        /**
         * Generates icosahefron faces
         *
         * @param vertices a reference to icosahedron verices
         * @param position a position of the origin
         * @param radius a radius of the icosahedron
         * @param color a color of the vertices
         */
        static void generateIcosahedronFaces(
            Vertices& vertices,
            Vector3f const& position,
            float32 radius,
            Color const& color);

        static Indices const                        IcosahedronIndices;
        static float32 const                        ATan;
    };

    template class Sphere<void>;

}
