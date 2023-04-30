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
     * Represents a convex hexahedron figure
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec = void>
    class ConvexHexahedron : public Angular<dim::Dim3, Spec> {
    public:
        using VertexTraits = Angular<dim::Dim3, Spec>::VertexTraits;

        /**
         * Constructs a new convex hexahedron with given
         * color
         *
         * @param color the color of the hexahedron (optional)
         */
        ConvexHexahedron(Color const& color = colors::white);

        /**
         * Constructs a new convex hexahedron from a point and
         * three dimension versors
         *
         * @param firstVertex the first vertex coord
         * @param firstVersor the first versor
         * @param secondVersor the second versor
         * @param thirdVersor the third versor
         * @param color the color of the convex hexahedron (optional)
         */
        ConvexHexahedron(
            Vector3f const& firstVertex,
            Vector3f const& firstVersor,
            Vector3f const& secondVersor,
            Vector3f const& thirdVersor,
            Color const& color = colors::white);

        /**
         * Constructs a new convex hexahedron object from the given
         * constant reference to the other object
         *
         * @param hexahedron the constant reference to the other
         * object
         */
        ConvexHexahedron(ConvexHexahedron const& hexahedron);

        ConvexHexahedron(ConvexHexahedron&& hexahedron) noexcept = default;

        ConvexHexahedron& operator=(
            ConvexHexahedron const& hexahedron) = default;
        ConvexHexahedron& operator=(
            ConvexHexahedron&& hexahedron) noexcept = default;

        /**
         * Draws the convex hexahedron on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Virtual destructor. Destroys the convex hexahedron object
         */
        virtual ~ConvexHexahedron(void) noexcept = default;
    private:
        using Indices = std::array<uint32, 36>;

        static constexpr Indices const              indices{
            0, 1, 3, 1, 2, 3, 1, 2, 4, 2, 4, 7, 0, 1, 5, 1, 5, 4,
            4, 5, 7, 7, 5, 6, 0, 3, 5, 3, 5, 6, 3, 2, 6, 2, 6, 7};

        /**
         * Initializes the element buffer object
         */
        void initElementBuffer(void) const noexcept;

        ElementArrayBuffer                          elementBuffer;
    };

    extern template class ConvexHexahedron<void>;

}

#include <MPGL/Core/Figures/Primitives/ConvexHexahedron.tpp>
