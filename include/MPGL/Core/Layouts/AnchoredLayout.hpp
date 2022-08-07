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

#include <MPGL/Core/Transformations/Transformation.hpp>

#include <memory>

namespace mpgl {

    /**
     * The layout that ensures that the figure will be translated
     * accordingly to the hook's movement (hook is the point on the
     * screen which is described in the [0, 1]^2 space. That means
     * the (0.4, 0.4) hook will be always in the 40% x and 40% y
     * of the screen) during the screen transformation event (
     * object's scale will be intanct as in default layout)
     */
    class AnchoredLayout :
        public Transformation2D,
        private GraphicalObject
    {
    public:
        /**
         * Constructs a new anchored layout object
         *
         * @param oldDimensions the dimensions of the window
         * before screen transformation event
         * @param hook the layout's hook (in the [0, 1]^2
         * coordinate space)
         */
        explicit AnchoredLayout(
            Vector2u const& oldDimensions,
            Vector2f const& hook = {0.f, 0.f}) noexcept;

        /**
         * Constructs a new anchored layout object
         *
         * @param oldDimensions the dimensions of the window
         * before screen transformation event
         * @param hook the layout's hook (in the screen pixels
         * coordinate space)
         */
        explicit AnchoredLayout(
            Vector2u const& oldDimensions,
            Vector2u const& hook) noexcept;

        AnchoredLayout(AnchoredLayout const&) noexcept = default;
        AnchoredLayout(AnchoredLayout&&) noexcept = default;

        AnchoredLayout& operator=(
            AnchoredLayout const&) noexcept = default;
        AnchoredLayout& operator=(
            AnchoredLayout&&) noexcept = default;

        /**
         * Returns the reference to the layout's hook
         *
         * @return the reference to the layout's hook
         */
        [[nodiscard]] Vector2f& getHook(void) noexcept
            { return hook; }

        /**
         * Returns the constant reference to the layout's hook
         *
         * @return the constant reference to the layout's hook
         */
        [[nodiscard]] Vector2f const& getHook(void) const noexcept
            { return hook; }

        /**
         * Ensures that the range will be shifted acording to the
         * hook's translation also omitting stretching
         *
         * @param range the reference to the position range object
         */
        void operator() (
            any::InputRange<Adapter2D>& range) const noexcept final;

        /**
         * Ensures that the coordinate will be shifted acording to the
         * hook's translation also omitting stretching
         *
         * @param coordinate the reference to the coordinate
         * object
         */
        void operator() (
           Adapter2D& coordinate) const noexcept final;

        /**
         * Destroys the anchored layout object
         */
        ~AnchoredLayout(void) noexcept = default;
    private:
        using Vector2uR = std::reference_wrapper<Vector2u const>;

        Vector2uR                               dimensions;
        Vector2f                                hook;
    };

}
