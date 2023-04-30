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

    namespace details {

        /**
         * Base class for the tetrahedron figure. Holds all type-independent
         * methods and attributes that are shared between all tetrahedron
         * specializations
        */
        class TetrahedronBase {
        protected:
            using IndicesArray = std::array<uint32, 12>;

            static constexpr IndicesArray           Indices{
                0, 1, 2, 0, 2, 3, 0, 1, 3, 1, 2, 3};

            /**
             * Constructs a new tetrahedron base object
            */
            explicit TetrahedronBase(void) noexcept = default;

            /**
             * Destroys a tetrahedron base object
            */
            virtual ~TetrahedronBase(void) noexcept = default;
        };

    }

    /**
     * Represents a tetrahedron figure
     *
     * @tparam Spec the angular vertices specifier
     */
    template <AngularTraitSpecifier<dim::Dim3> Spec = void>
    class Tetrahedron :
        public Angular<dim::Dim3, Spec>,
        private details::TetrahedronBase
    {
    public:
        using VertexTraits = Angular<dim::Dim3, Spec>::VertexTraits;

        /**
         * Constructs a new tetrahedron with given
         * color
         *
         * @param color the color of the tetrahedron (optional)
         */
        Tetrahedron(Color const& color = colors::white);

        /**
         * Constructs a new tetrahedron from a four
         * given vertices [fourth is then calculated]
         *
         * @param firstVertex the first vertex coord
         * @param secondVertex the second vertex coord
         * @param thirdVertex the third vertex coord
         * @param fourthVertex the fourth vertex coord
         * @param color the color of the tetragon (optional)
         */
        Tetrahedron(
            Vector3f const& firstVertex,
            Vector3f const& secondVertex,
            Vector3f const& thirdVertex,
            Vector3f const& fourthVertex,
            Color const& color = colors::white);

        /**
         * Constructs a new tetrahedron object from the given
         * constant reference to the other object
         *
         * @param tetrahedron the constant reference to the other
         * object
         */
        Tetrahedron(Tetrahedron const& tetrahedron);

        Tetrahedron(Tetrahedron&& tetrahedron) noexcept = default;

        Tetrahedron& operator=(
            Tetrahedron const& tetrahedron) = default;
        Tetrahedron& operator=(
            Tetrahedron&& tetrahedron) noexcept = default;

        /**
         * Draws the tetrahedron on the screen
         */
        virtual void draw(void) const noexcept;

        /**
         * Virtual destructor. Destroys the tetrahedron object
         */
        virtual ~Tetrahedron(void) noexcept = default;
    private:

        /**
         * Initializes the element buffer object
         */
        void initElementBuffer(void) const noexcept;

        ElementArrayBuffer                          elementBuffer;
    };

    extern template class Tetrahedron<void>;

}

#include <MPGL/Core/Figures/Primitives/Tetrahedron.tpp>
