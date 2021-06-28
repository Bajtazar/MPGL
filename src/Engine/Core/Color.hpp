#pragma once

#include <inttypes.h>

namespace ge {

    struct Color {
        constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha=0) noexcept : red((float)red/255), green((float)green/255), blue((float)blue/255), alpha((float)alpha/255) {}
        constexpr Color(float red, float green, float blue, float alpha) noexcept : red(red), green(green), blue(blue), alpha(alpha) {}
        constexpr Color(void) noexcept : red(0.f), green(0.f), blue(0.f), alpha(0.f) {}

        struct literals;

        using value_type = float;

        float red;
        float green;
        float blue;
        float alpha;

        template<std::size_t Index>
        constexpr auto&& get() &  { return __getHelper<Index>(*this); }

        template<std::size_t Index>
        constexpr auto&& get() && { return __getHelper<Index>(*this); }

        template<std::size_t Index>
        constexpr auto&& get() const &  { return __getHelper<Index>(*this); }

        template<std::size_t Index>
        constexpr auto&& get() const && { return __getHelper<Index>(*this); }
    private:
        template<std::size_t Index, typename _This>
        constexpr auto&& __getHelper(_This&& __this) {
            static_assert(Index < 4, "Index out of Color bounds");
            if constexpr (Index == 0) return std::forward<_This>(__this).red;
            if constexpr (Index == 1) return std::forward<_This>(__this).green;
            if constexpr (Index == 2) return std::forward<_This>(__this).blue;
            if constexpr (Index == 3) return std::forward<_This>(__this).alpha;
        }
    };

    struct Color::literals {
        static constexpr Color Red {255, 0, 0};
        static constexpr Color Green {0, 255, 0};
        static constexpr Color Blue {0, 0, 255};
        static constexpr Color White {255, 255, 255};
        static constexpr Color Black {0, 0, 0};
    };

    constexpr Color operator/ (const Color& left, const Color& right) {
        return {left.red / right.red, left.green / right.green, left.blue / right.blue, left.alpha / right.alpha};
    }

    constexpr Color operator* (const Color& left, const Color& right) noexcept {
        return {left.red * right.red, left.green * right.green, left.blue * right.blue, left.alpha * right.alpha};
    }

    constexpr Color operator+ (const Color& left, const Color& right) noexcept {
        return {left.red + right.red, left.green + right.green, left.blue + right.blue, left.alpha + right.alpha};
    }

    constexpr Color operator- (const Color& left, const Color& right) noexcept {
        return {left.red - right.red, left.green - right.green, left.blue - right.blue, left.alpha - right.alpha};
    }

    constexpr Color operator+ (const Color& left, float right) noexcept {
        return {left.red + right, left.green + right, left.blue + right, left.alpha + right};
    }

    constexpr Color operator- (const Color& left, float right) noexcept {
        return {left.red - right, left.green - right, left.blue - right, left.alpha - right};
    }

    constexpr Color operator* (const Color& left, float right) noexcept {
        return {left.red * right, left.green * right, left.blue * right, left.alpha * right};
    }

    constexpr Color operator/ (const Color& left, float right) noexcept {
        return {left.red / right, left.green / right, left.blue / right, left.alpha / right};
    }


}
