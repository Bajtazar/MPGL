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

#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Core/Textures/Texture.hpp>

#include <optional>

namespace mpgl {

    /**
     * Containts information needed to draw a glyph
     */
    struct Glyph {
        typedef std::optional<Texture>          TextureVar;

        TextureVar                              texture;
        Vector2u                                dimensions;
        Vector2i                                bearing;
        uint32                                  advance;

        /**
         * Construct a new Glyph object holding a given texture
         * [if the glyph has one], dimensions, bearing and advance
         *
         * @param texture the optional holding glyph's texture or
         * nothing
         * @param dimensions the glyph's dimensions
         * @param bearing the glyph's bearing
         * @param advance the glyph's advance
         */
        explicit Glyph(
            TextureVar const& texture,
            Vector2u const& dimensions,
            Vector2i const& bearing,
            uint32 advance) noexcept
                : texture{texture},
                dimensions{dimensions}, bearing{bearing},
                advance{advance} {}

        /**
         * Returns whether a glyph has an outline
         *
         * @return if the glyph has an outline
         */
        bool hasOutline(void) const noexcept
            { return bool(texture); }

        /**
         * Returns the reference to the element lying under
         * the given index
         *
         * @tparam Index the element's index
         * @return the reference to the given element
         */
        template <std::size_t Index>
            requires (Index < 4)
        constexpr auto&& get(void) & noexcept
            { return helper<Index>(*this); }

        /**
         * Returns the rvalue reference to the element lying
         * under the given index
         *
         * @tparam Index the element's index
         * @return the rvalue reference to the given element
         */
        template <std::size_t Index>
            requires (Index < 4)
        constexpr auto&& get(void) && noexcept
            { return helper<Index>(*this); }

        /**
         * Returns the constant reference to the element lying
         * under the given index
         *
         * @tparam Index the element's index
         * @return the constant reference to the given element
         */
        template <std::size_t Index>
            requires (Index < 4)
        constexpr auto&& get(void) const& noexcept
            { return helper<Index>(*this); }
    private:
        /**
         * Helps in getting the element with given index
         *
         * @tparam Index the element's index
         * @tparam Base the glyph object universal reference
         * @param base the glyph object
         * @return the universal reference to the given element
         */
        template <std::size_t Index, class Base>
        static constexpr auto&& helper(Base&& base) noexcept;

    };

}

#include <MPGL/Core/Text/Glyph.tpp>

namespace std {

    template <>
    struct tuple_size<mpgl::Glyph> : integral_constant<size_t, 4> {};

    template <>
    struct tuple_element<0, mpgl::Glyph>
        { using type = mpgl::Glyph::TextureVar; };

    template <>
    struct tuple_element<1, mpgl::Glyph>
        { using type = mpgl::Vector2u; };

    template <>
    struct tuple_element<2, mpgl::Glyph>
        { using type = mpgl::Vector2i; };

    template <>
    struct tuple_element<3, mpgl::Glyph>
        { using type = mpgl::uint32; };

}
