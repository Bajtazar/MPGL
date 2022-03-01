#pragma once

#include "../Textures/Texture.hpp"
#include "../../Mathematics/Vector.hpp"

#include <optional>

namespace mpgl {

    struct Glyph {
        typedef std::optional<Texture>          TextureVar;

        TextureVar                              texture;
        Vector2u                               dimensions;
        Vector2i                                bearing;
        uint32                                  advance;

        explicit Glyph(TextureVar const& texture,
            Vector2u const& dimensions, Vector2i const& bearing,
            uint32 advance) noexcept : texture{texture},
                dimensions{dimensions}, bearing{bearing},
                advance{advance} {}

        bool hasOutline(void) const noexcept
            { return bool(texture); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) & noexcept
            { return helper<Index>(*this); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) && noexcept
            { return helper<Index>(*this); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) const& noexcept
            { return helper<Index>(*this); }

    private:
        template <std::size_t Index, class Base>
        inline constexpr auto&& helper(Base&& base) const noexcept {
            if constexpr (Index == 0)
                return std::forward<Base>(base).texture;
            else if constexpr (Index == 1)
                return std::forward<Base>(base).dimensions;
            else if constexpr (Index == 2)
                return std::forward<Base>(base).bearing;
            else
                return std::forward<Base>(base).advance;
        }

    };

}

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
