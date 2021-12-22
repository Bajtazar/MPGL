#pragma once

#include <inttypes.h>
#include <utility>
#include <tuple>

namespace ge {

    struct Color {
        constexpr Color(uint8_t red, uint8_t green,
            uint8_t blue, uint8_t alpha=255) noexcept
                : red((float)red/255), green((float)green/255),
                blue((float)blue/255), alpha((float)alpha/255) {}
        constexpr Color(float red, float green, float blue,
            float alpha) noexcept
                : red(red), green(green),
                blue(blue), alpha(alpha) {}
        constexpr Color(void) noexcept
            : red(0.f), green(0.f), blue(0.f), alpha(1.f) {}

        struct                              literals;

        float                               red;
        float                               green;
        float                               blue;
        float                               alpha;

        template <std::size_t Index>
            requires (Index < 4)
        constexpr float& get(void) & noexcept
            { return getHelper<Index>(*this); }

        template <std::size_t Index>
            requires (Index < 4)
        constexpr float const& get(void) const& noexcept
            { return getHelper<Index>(*this); }

    private:
        template <std::size_t Index, class Base>
            requires (Index < 4)
        constexpr float&& getHelper(Base&& base) const noexcept
        {
            if constexpr (Index == 0)
                return std::forward<Base>(base).red;
            else if constexpr (Index == 1)
                return std::forward<Base>(base).green;
            else if constexpr (Index == 2)
                return std::forward<Base>(base).blue;
            else
                return std::forward<Base>(base).alpha;
        }
    };

    struct Color::literals {
        static constexpr Color              Red {255, 0, 0};
        static constexpr Color              Green {0, 255, 0};
        static constexpr Color              Blue {0, 0, 255};
        static constexpr Color              White {255, 255, 255};
        static constexpr Color              Black {0, 0, 0};
        static constexpr Color              Yellow {255, 255, 0};
    };

    constexpr Color operator/ (Color const& left,
        Color const& right)
    {
        return {left.red / right.red, left.green / right.green,
            left.blue / right.blue, left.alpha / right.alpha};
    }

    constexpr Color operator* (Color const& left,
        Color const& right) noexcept
    {
        return {left.red * right.red, left.green * right.green,
            left.blue * right.blue, left.alpha * right.alpha};
    }

    constexpr Color operator+ (Color const& left,
        Color const& right) noexcept
    {
        return {left.red + right.red, left.green + right.green,
            left.blue + right.blue, left.alpha + right.alpha};
    }

    constexpr Color operator- (Color const& left,
        Color const& right) noexcept
    {
        return {left.red - right.red, left.green - right.green,
            left.blue - right.blue, left.alpha - right.alpha};
    }

    constexpr Color operator+ (Color const& left,
        float right) noexcept
    {
        return {left.red + right, left.green + right,
            left.blue + right, left.alpha + right};
    }

    constexpr Color operator- (Color const& left,
        float right) noexcept
    {
        return {left.red - right, left.green - right,
            left.blue - right, left.alpha - right};
    }

    constexpr Color operator* (Color const& left,
        float right) noexcept
    {
        return {left.red * right, left.green * right,
            left.blue * right, left.alpha * right};
    }

    constexpr Color operator/ (Color const& left,
        float right) noexcept {
        return {left.red / right, left.green / right,
            left.blue / right, left.alpha / right};
    }

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
