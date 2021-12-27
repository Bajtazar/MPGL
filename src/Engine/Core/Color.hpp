#pragma once

#include "../Mathematics/Vector.hpp"

namespace ge {

    class Color : public Vector4f {
    public:
        constexpr Color(uint8_t red, uint8_t green,
            uint8_t blue, uint8_t alpha=0xFF) noexcept
                : Vector4f{red / 0xFF, green / 0xFF,
                blue / 0xFF, alpha / 0xff} {}

        constexpr Color(float red, float green,
            float blue, float alpha) noexcept
                : Vector4f{red, green, blue, alpha} {}

        constexpr Color(void) noexcept
            : Vector4f{0.f, 0.f, 0.f, 1.f} {}

        constexpr Color(Vector4f const& vector) noexcept
            : Vector4f{vector} {}

        constexpr Color(Vector4f&& vector) noexcept
            : Vector4f{std::move(vector)} {}

        constexpr float& red(void) noexcept
            { return (*this)[0]; }
        constexpr float const& red(void) const noexcept
            { return (*this)[0]; }

        constexpr float& green(void) noexcept
            { return (*this)[1]; }
        constexpr float const& green(void) const noexcept
            { return (*this)[1]; }

        constexpr float& blue(void) noexcept
            { return (*this)[2]; }
        constexpr float const& blue(void) const noexcept
            { return (*this)[2]; }

        constexpr float& alpha(void) noexcept
            { return (*this)[3]; }
        constexpr float const& alpha(void) const noexcept
            { return (*this)[3]; }

        template <std::size_t Index>
            requires (Index < 4)
        constexpr float& get(void) & noexcept
            { return (*this)[Index]; }

        template <std::size_t Index>
            requires (Index < 4)
        constexpr float const& get(void) const& noexcept
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
        typedef float                       type;
    };

}
