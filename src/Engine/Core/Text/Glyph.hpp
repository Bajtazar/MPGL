#pragma once

#include "../Texture.hpp"
#include "../../Mathematics/Vector.hpp"

#include <optional>

namespace ge {

    template <Allocator Alloc = Texture<>::allocator_type>
    struct Glyph {
        typedef std::optional<Texture<Alloc>>    TextureVar;

        TextureVar          texture;
        Vector2ui           dimmensions;
        Vector2i            bearing;
        uint32_t            advance;

        explicit Glyph(TextureVar const& texture,
            Vector2ui const& dimmensions, Vector2i const& bearing,
            uint32_t advance) noexcept : texture{texture},
                dimmensions{dimmensions}, bearing{bearing},
                advance{advance} {}

        bool hasOutline(void) const noexcept { return bool(texture); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) & noexcept { return helper<Index>(*this); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) && noexcept { return helper<Index>(*this); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) const& noexcept { return helper<Index>(*this); }

    private:
        template <std::size_t Index, class Base>
        inline constexpr auto&& helper(Base&& base) const noexcept {
            if constexpr (Index == 0)
                return std::forward<Base>(base).texture;
            else if constexpr (Index == 1)
                return std::forward<Base>(base).dimmensions;
            else if constexpr (Index == 2)
                return std::forward<Base>(base).bearing;
            else
                return std::forward<Base>(base).advance;
        }

    };

}

namespace std {

    template <ge::Allocator Alloc>
    struct tuple_size<ge::Glyph<Alloc>> : integral_constant<size_t, 4> {};

    template <ge::Allocator Alloc>
    struct tuple_element<0, ge::Glyph<Alloc>> { using type = ge::Glyph<Alloc>::TextureVar; };

    template <ge::Allocator Alloc>
    struct tuple_element<1, ge::Glyph<Alloc>> { using type = ge::Vector2ui; };

    template <ge::Allocator Alloc>
    struct tuple_element<2, ge::Glyph<Alloc>> { using type = ge::Vector2i; };

    template <ge::Allocator Alloc>
    struct tuple_element<3, ge::Glyph<Alloc>> { using type = uint32_t; };

}
