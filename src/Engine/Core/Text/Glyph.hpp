#pragma once

#include <inttypes.h>

#include "../Texture.hpp"
#include "../../Mathematics/Vector.hpp"

namespace ge {

    template <Allocator Alloc = Texture<>::allocator_type>
    struct Glyph {
        Texture<Alloc>      texture;
        Vector2i            dimmensions;
        Vector2i            bearing;
        uint32_t            advance;

        // constructor

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) & noexcept { return getHelper<Index>(); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) && noexcept { return getHelper<Index>(); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) const& noexcept { return getHelper<Index>(); }

    private:
        template <std::size_t Index, class Base>
        inline constexpr auto&& getHelper(Base&& base) const noexcept {
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
    struct tuple_element<0, ge::Glyph<Alloc>> { using type = Texture<Alloc>; };

    template <ge::Allocator Alloc, size_t Index>
        requires (Index > 0 && Index < 3)
    struct tuple_element<Index, ge::Glyph<Alloc>> { using type = Vector2i; };

    template <ge::Allocator Alloc>
    struct tuple_element<3, ge::Glyph<Alloc>> { using type = uint32_t; };

}
