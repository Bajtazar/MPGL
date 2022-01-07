#pragma once

#include "../Mathematics/Vector.hpp"

namespace ge {

    class Color : public Vector4f {
    public:
        constexpr Color(uint8 red, uint8 green,
            uint8 blue, uint8 alpha=0xFF) noexcept
                : Vector4f{red / 0xFF, green / 0xFF,
                blue / 0xFF, alpha / 0xff} {}

        constexpr Color(float32 red, float32 green,
            float32 blue, float32 alpha) noexcept
                : Vector4f{red, green, blue, alpha} {}

        constexpr Color(void) noexcept
            : Vector4f{0.f, 0.f, 0.f, 1.f} {}

        constexpr Color(Vector4f const& vector) noexcept
            : Vector4f{vector} {}

        constexpr Color(Vector4f&& vector) noexcept
            : Vector4f{std::move(vector)} {}

        constexpr float32& red(void) noexcept
            { return (*this)[0]; }
        constexpr float32 const& red(void) const noexcept
            { return (*this)[0]; }

        constexpr float32& green(void) noexcept
            { return (*this)[1]; }
        constexpr float32 const& green(void) const noexcept
            { return (*this)[1]; }

        constexpr float32& blue(void) noexcept
            { return (*this)[2]; }
        constexpr float32 const& blue(void) const noexcept
            { return (*this)[2]; }

        constexpr float32& alpha(void) noexcept
            { return (*this)[3]; }
        constexpr float32 const& alpha(void) const noexcept
            { return (*this)[3]; }

        template <std::size_t Index>
            requires (Index < 4)
        constexpr float32& get(void) & noexcept
            { return (*this)[Index]; }

        template <std::size_t Index>
            requires (Index < 4)
        constexpr float32 const& get(void) const& noexcept
            { return (*this)[Index]; }

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
    };

}

namespace std {

    template <>
    struct tuple_size<ge::Color>
        : integral_constant<size_t, 4> {};

    template <size_t Index>
        requires (Index < 4)
    struct tuple_element<Index, ge::Color> {
        typedef ge::float32                   type;
    };

}
