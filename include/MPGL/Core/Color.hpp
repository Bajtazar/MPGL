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

#include <MPGL/Mathematics/Tensors/Vector.hpp>

namespace mpgl {

    /**
     * Contains informations about pixel color
     */
    class Color : public Vector4f {
    public:
        /**
         * Constructs a new color object from the given
         * rgba subpixels in scale from 0 to 255
         *
         * @param red the red subpixel
         * @param green the green subpixel
         * @param blue the blue subpixel
         * @param alpha the alpha value
         */
        constexpr Color(
            uint8 red,
            uint8 green,
            uint8 blue,
            uint8 alpha=0xFF) noexcept
                : Vector4f{red / 0xFF, green / 0xFF,
                    blue / 0xFF, alpha / 0xff} {}

        /**
         * Constructs a new color object from the given
         * rgba subpixels in scale from 0 to 1
         *
         * @param red the red subpixel
         * @param green the green subpixel
         * @param blue the blue subpixel
         * @param alpha the alpha value
         */
        constexpr Color(
            float32 red,
            float32 green,
            float32 blue,
            float32 alpha) noexcept
                : Vector4f{red, green, blue, alpha} {}

        /**
         * Constructs a new color object
         */
        constexpr Color(void) noexcept
            : Vector4f{0.f, 0.f, 0.f, 1.f} {}

        /**
         * Constructs a new color object from the given vector
         *
         * @param vector the constant reference to the
         * vector4f object
         */
        constexpr Color(Vector4f const& vector) noexcept
            : Vector4f{vector} {}

        /**
         * Constructs a new color object from the given vector
         *
         * @param vector the rvalue reference to the
         * vector4f object
         */
        constexpr Color(Vector4f&& vector) noexcept
            : Vector4f{std::move(vector)} {}

        /**
         * Returns the reference to the red subpixel
         *
         * @return the reference to the red subpixel
         */
        [[nodiscard]] constexpr float32& red(void) noexcept
            { return (*this)[0]; }

        /**
         * Returns the constant reference to the red subpixel
         *
         * @return the constant reference to the red subpixel
         */
        [[nodiscard]] constexpr float32 const& red(
            void) const noexcept
                { return (*this)[0]; }

        /**
         * Returns the reference to the green subpixel
         *
         * @return the reference to the green subpixel
         */
        [[nodiscard]] constexpr float32& green(void) noexcept
            { return (*this)[1]; }

        /**
         * Returns the constant reference to the green subpixel
         *
         * @return the constant reference to the green subpixel
         */
        [[nodiscard]] constexpr float32 const& green(
            void) const noexcept
                { return (*this)[1]; }

        /**
         * Returns the reference to the blue subpixel
         *
         * @return the reference to the blue subpixel
         */
        [[nodiscard]] constexpr float32& blue(void) noexcept
            { return (*this)[2]; }

        /**
         * Returns the constant reference to the blue subpixel
         *
         * @return the constant reference to the blue subpixel
         */
        [[nodiscard]] constexpr float32 const& blue(
            void) const noexcept
                { return (*this)[2]; }

        /**
         * Returns the reference to the alpha value
         *
         * @return the reference to the alpha value
         */
        [[nodiscard]] constexpr float32& alpha(void) noexcept
            { return (*this)[3]; }

        /**
         * Returns the constant reference to the alpha value
         *
         * @return the constant reference to the alpha value
         */
        [[nodiscard]] constexpr float32 const& alpha(
            void) const noexcept
                { return (*this)[3]; }

        /**
         * Returns the reference to the given element
         *
         * @tparam Index the element's index
         * @return the reference to the given element
         */
        template <std::size_t Index>
            requires (Index < 4)
        [[nodiscard]] constexpr float32& get(void) & noexcept
            { return (*this)[Index]; }

        /**
         * Returns the constant reference to the given element
         *
         * @tparam Index the element's index
         * @return the constant reference to the given element
         */
        template <std::size_t Index>
            requires (Index < 4)
        [[nodiscard]] constexpr float32 const& get(
            void) const& noexcept
                { return (*this)[Index]; }

        /// The embeded colors
        static constexpr Vector4f           Red
            {1.f, 0.f, 0.f, 1.f};
        static constexpr Vector4f           Green
            {0.f, 1.f, 0.f, 1.f};
        static constexpr Vector4f           Blue
            {0.f, 0.f, 1.f, 1.f};
        static constexpr Vector4f           White
            {1.f, 1.f, 1.f, 1.f};
        static constexpr Vector4f           Black
            {0.f, 0.f, 0.f, 1.f};
        static constexpr Vector4f           Yellow
            {1.f, 1.f, 0.f, 1.f};
        static constexpr Vector4f           Magenta
            {1.f, 0.f, 1.f, 1.f};
        static constexpr Vector4f           Cyan
            {0.f, 1.f, 1.f, 1.f};
        static constexpr Vector4f           Purple
            {0.5f, 0.f, 0.5f, 1.f};
        static constexpr Vector4f           Orange
            {1.f, 0.64f, 0.f, 1.f};
        static constexpr Vector4f           Violet
            {0.92f, 0.5f, 0.92f, 1.f};
        static constexpr Vector4f           Silver
            {0.75f, 0.75f, 0.75f, 1.f};
        static constexpr Vector4f           Gold
            {1.f, 0.83f, 0.f, 1.f};
        static constexpr Vector4f           Gray
            {0.5f, 0.5f, 0.5f, 1.f};
        static constexpr Vector4f           Pink
            {1.f, 0.75f, 0.79f, 1.f};
        static constexpr Vector4f           MPGLBlue
            {0.f, 0.63f, 0.91f, 1.f};
    };

}

namespace std {

    template <>
    struct tuple_size<mpgl::Color>
        : integral_constant<size_t, 4> {};

    template <size_t Index>
        requires (Index < 4)
    struct tuple_element<Index, mpgl::Color> {
        typedef mpgl::float32                   type;
    };

}
