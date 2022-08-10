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
     * Represents a tetragon figure
     */
    class Tetragon : public Angular {
    public:
        /**
         * Construct a new Tetragon with given
         * color
         *
         * @param color the color of the tetragon
         */
        Tetragon(Color const& color = {});

        /**
         * Construct a new Parallelogram from a three
         * given vertices [fourth is then calculated]
         *
         * @param firstVertex the first vertex coord
         * @param secondVertex the second vertex coord
         * @param thirdVertex the third vertex coord
         * @param color the color of the tetragon
         */
        Tetragon(Vector2f const& firstVertex,
                Vector2f const& secondVertex,
                Vector2f const& thirdVertex,
                Color const& color = {});

        /**
         * Construct a new Rectangle parallel to
         * the x and y axis with the given color
         *
         * @param firstVertex the first vertex coord
         * @param dimensions the rectangle dimensions
         * @param color the color of the rectangle
         */
        Tetragon(Vector2f const& firstVertex,
                Vector2f const& dimensions,
                Color const& color = {});

        /**
         * Construct a new Tetragon object from the given
         * constant reference to the other object
         *
         * @param tetragon the constant reference to the other
         * object
         */
        Tetragon(Tetragon const& tetragon);

        Tetragon(Tetragon&& tetragon) noexcept = default;

        Tetragon& operator= (Tetragon const& tetragon) = default;
        Tetragon& operator= (Tetragon&& tetragon) noexcept = default;

        /**
         * Draws the tetragon on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Checks whether the given pixel is located inside
         * the tetragon
         *
         * @param position the pixel's position
         * @return if the given point is inside the tetragon
         */
        [[nodiscard]] bool contains(
            Vector2u const& position) const noexcept final;

        /**
         * Destroy the Tetragon object
         */
        ~Tetragon(void) noexcept = default;
    private:
        typedef std::array<uint32, 6>               Indexes;

        static constexpr const Indexes              indexes{
            0, 1, 2, 0, 3, 2};

        /**
         * Initializes the element buffer object
         */
        void initElementBuffer(void) const noexcept;

        /**
         * Calculates whether the given point is inside subtriangle
         *
         * @param position the point position
         * @param firstVertex the first vertex position
         * @param secondVertex the second vertex position
         * @param thirdVertex the third vertex position
         * @return if the given point is inside subtriangle
         */
        [[nodiscard]] bool insideSubtriangle(
            Vector2d const& position,
            Vector2d const& firstVertex,
            Vector2d const& secondVertex,
            Vector2d const& thirdVertex) const noexcept;

        ElementArrayBuffer                          elementBuffer;
    };

}
