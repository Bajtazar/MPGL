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

#include <MPGL/Core/Context/Buffers/ElementArrayBuffer.hpp>
#include <MPGL/Core/Figures/Angular.hpp>

namespace mpgl {

    /**
     * Represents a cone figure
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec = void>
    class Cone : public Angular<dim::Dim3, Spec> {
    public:
        using VertexTraits = Angular<dim::Dim3, Spec>::VertexTraits;

        /**
         * Constructs a new cone with given
         * color
         *
         * @param color the color of the cone (optional)
         * @param baseSegments a number of vertices that consists of
         * one base circle (optional)
         */
        Cone(
            Color const& color = colors::white,
            size_t baseSegments = 20);

        /**
         * Construct a new cone object from the given
         * base radius vector and height vector
         *
         * @throws NotPerpendicularException when the radius vector
         * and height vector are not perpendicular
         * @param position a position of the middle point of the lower
         * base circle
         * @param radiusVector a circles' radius vector
         * @param heightVector a height vector
         * @param color a color of the cone (optional)
         * @param baseSegments a number of vertices that consists of
         * one base circle (optional)
         */
        Cone(
            Vector3f const& position,
            Vector3f const& radiusVector,
            Vector3f const& heightVector,
            Color const& color = colors::white,
            size_t baseSegments = 20);

        /**
         * Constructs a new cone object from the given radius
         * and height. The newly created cone is inlined with all
         * euclidean axes
         *
         * @param position a position of the middle point of the lower
         * base circle
         * @param radius a circles' radius
         * @param height a cone's height
         * @param color a color of the cone (optional)
         * @param baseSegments a number of vertices that consists of
         * one base circle (optional)
         */
        Cone(
            Vector3f const& position,
            float32 radius,
            float32 height,
            Color const& color = colors::white,
            size_t baseSegments = 20);

        /**
         * Constructs a new cone object from the given
         * constant reference to the other object
         *
         * @param cone the constant reference to the other
         * object
         */
        Cone(Cone const& cone);

        Cone(Cone&& cone) noexcept = default;

        /**
         * Assigns an other cone object to this object
         *
         * @param cone a constant reference to the other object
         * @return a reference to this object
         */
        Cone& operator=(Cone const& cone);

        Cone& operator=(Cone&& cone) noexcept = default;

        /**
         * Draws the cone on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Virtual destructor. Destroys the cone object
         */
        virtual ~Cone(void) noexcept = default;
    private:
        using Indices = std::vector<uint32>;
        using Vertices = typename Angular<dim::Dim3, Spec>::Vertices;

        /**
         * Reloads the element array buffer object
         */
        void reloadElementBuffer(void) const noexcept;

        Indices                                     indices;
        ElementArrayBuffer                          elementBuffer;

        /**
         * Generates the cone's vertices
         *
         * @param color the color of the vertices
         * @param position the position of the middle of the bottom
         * circle
         * @param radiusVector the radius vector
         * @param heightVector the height vector
         * @param segments a number of vertices that consists of
         * one base circle
         * @return the cone's vertices
         */
        [[nodiscard]] static Vertices generateVertices(
            Color const& color,
            Vector3f const& position,
            Vector3f const& radiusVector,
            Vector3f const& heightVector,
            size_t segments);

        /**
         * Generates the base circle
         *
         * @param vertices a reference to the vertices vector
         * @param color the color of the vertices
         * @param middle the middle point of the circle
         * @param radiusVector the radius vector
         * @param normal the circle's plane normal vector
         * @param segments a number of vertices that consists of
         * one base circle
         */
        static void generateCircle(
            Vertices& vertices,
            Color const& color,
            Vector3f const& middle,
            Vector3f const& radiusVector,
            Vector3f const& normal,
            size_t segments);

        /**
         * Generates cone's indices
         *
         * @param segments a number of vertices that consists of
         * one base circle
         * @return the cone's indices
         */
        [[nodiscard]] static Indices generateIndices(
            size_t segments);

        /**
         * Generate base cone's indices
         *
         * @param indices a reference to the indices vector
         * @param segments a number of vertices that consists of
         * one base circle
         */
        static void generateCircleIndices(
            Indices& indices,
            size_t segments);

        /**
         * Generates cone's facade inidices
         *
         * @param indices a reference to the indices vector
         * @param segments number of vertices that consists of
         * one base circle
         */
        static void generateFacesIndices(
            Indices& indices,
            size_t segments);

        /**
         * Adds trinagle to the indices vector
         *
         * @param indices a reference to the indices vector
         * @param first the first trinagle's vertex position
         * @param second the second trinagle's vertex position
         * @param third the third trinagle's vertex position
         */
        static void addTriangle(
            Indices& indices,
            size_t first,
            size_t second,
            size_t third);
    };

    extern template class Cone<void>;

}

#include <MPGL/Core/Figures/Primitives/Cone.tpp>
