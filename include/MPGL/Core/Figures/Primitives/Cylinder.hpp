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
#include <MPGL/Core/Figures/Angular.hpp>

namespace mpgl {

    /**
     * Represents a cylinder figure
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec = void>
    class Cylinder : public Angular<dim::Dim3, Spec> {
    public:
        using VertexTraits = Angular<dim::Dim3, Spec>::VertexTraits;

        /**
         * Constructs a new cylinder with given
         * color
         *
         * @param color the color of the cylinder (optional)
         * @param baseSegments a number of vertices that consists of
         * one base circle (optional)
         */
        Cylinder(
            Color const& color = Color::White,
            size_t baseSegments = 20);

        /**
         * Construct a new cylinder object from the given
         * base radius vector and height vector
         *
         * @throws NotPerpendicularException when the radius vector
         * and height vector are not perpendicular
         * @param position a position of the middle point of the lower
         * base circle
         * @param radiusVector a circles' radius vector
         * @param heightVector a height vector
         * @param color a color of the cylinder (optional)
         * @param baseSegments a number of vertices that consists of
         * one base circle (optional)
         */
        Cylinder(
            Vector3f const& position,
            Vector3f const& radiusVector,
            Vector3f const& heightVector,
            Color const& color = Color::White,
            size_t baseSegments = 20);

        /**
         * Constructs a new cylinder object from the given radius
         * and height. The newly created cylinder is inlined with all
         * euclidean axes
         *
         * @param position a position of the middle point of the lower
         * base circle
         * @param radius a circles' radius
         * @param height a cylinder's height
         * @param color a color of the cylinder (optional)
         * @param baseSegments a number of vertices that consists of
         * one base circle (optional)
         */
        Cylinder(
            Vector3f const& position,
            float32 radius,
            float32 height,
            Color const& color = Color::White,
            size_t baseSegments = 20);

        /**
         * Constructs a new cylinder object from the given
         * constant reference to the other object
         *
         * @param cylinder a constant reference to the other
         * object
         */
        Cylinder(Cylinder const& cylinder);

        Cylinder(Cylinder&& cylinder) noexcept = default;

        /**
         * Assigns an other cylinder object to this object
         *
         * @param cylinder a constant reference to the other
         * object
         * @return a reference to this object
         */
        Cylinder& operator=(Cylinder const& cylinder);

        Cylinder& operator=(
            Cylinder&& cylinder) noexcept = default;

        /**
         * Draws the cylinder on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Virtual destructor. Destroys the cylinder object
         */
        virtual ~Cylinder(void) noexcept = default;
    private:
        using Indicies = std::vector<uint32>;
        using Vertices = typename Angular<dim::Dim3, Spec>::Vertices;

        /**
         * Reloads the element array buffer object
         */
        void reloadElementBuffer(void) const noexcept;

        Indicies                                    indicies;
        ElementArrayBuffer                          elementBuffer;

        /**
         * Generates the cylinder's vertices
         *
         * @param color the color of the vertices
         * @param position the position of the middle of the bottom
         * circle
         * @param radiusVector the radius vector
         * @param heightVector the height vector
         * @param segments a number of vertices that consists of
         * one base circle
         * @return the cylinder's vertices
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
         * Generates cylinder's indicies
         *
         * @param segments a number of vertices that consists of
         * one base circle
         * @return the cylinder's indicies
         */
        [[nodiscard]] static Indicies generateIndicies(
            size_t segments);

        /**
         * Generate base circle's indicies
         *
         * @param indicies a reference to the indicies vector
         * @param startIndex a start index of the circile's indicies
         * @param segments a number of vertices that consists of
         * one base circle
         */
        static void generateCircleIndicies(
            Indicies& indicies,
            size_t startIndex,
            size_t segments);

        /**
         * Generates cylinder's facade inidices
         *
         * @param indicies a reference to the indicies vector
         * @param segments number of vertices that consists of
         * one base circle
         */
        static void generateFacesIndicies(
            Indicies& indicies,
            size_t segments);

        /**
         * Adds trinagle to the indicies vector
         *
         * @param indicies a reference to the indicies vector
         * @param first the first trinagle's vertex position
         * @param second the second trinagle's vertex position
         * @param third the third trinagle's vertex position
         */
        static void addTriangle(
            Indicies& indicies,
            size_t first,
            size_t second,
            size_t third);
    };

    template class Cylinder<void>;

}
