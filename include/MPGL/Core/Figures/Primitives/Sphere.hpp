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
#include <MPGL/Core/Vertex/Indicies/IndiciesTriangle.hpp>
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
            Color const& color = Color::White,
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

        Sphere& operator=(Sphere const& sphere) = default;
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
        using Indicies = std::vector<IndiciesTriangle>;
        using BaseIndicies = std::array<IndiciesTriangle, 20>;
        using Vertices = typename Angular<dim::Dim3, Spec>::Vertices;

        /**
         * Initializes the element buffer object
         */
        void initElementBuffer(void) const noexcept;

        Indicies                                    indicies;
        ElementArrayBuffer                          elementBuffer;

        [[nodiscard]] static Vertices generateIcosahedron(
            Vector3f const& positon,
            float32 radius,
            Color const& color);

        static constexpr BaseIndicies const         IcosahedronIndicies {
            {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 5, 1},
            {1, 7, 2}, {2, 8, 3}, {3, 9, 4}, {4, 10, 5}, {5, 6, 1},
            {6, 1, 7}, {7, 2, 8}, {8, 3, 9}, {9, 4, 10}, {10, 5, 6},
            {11, 6, 7}, {11, 7, 8}, {11, 8, 9}, {11, 9, 10}, {11, 10, 6}
        };
    };

    template class Sphere<void>;

}
