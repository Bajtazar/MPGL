#pragma once

#include <inttypes.h>
#include <utility>

namespace ge {

    struct Color {
        constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha=255) noexcept : red((float)red/255), green((float)green/255), blue((float)blue/255), alpha((float)alpha/255) {}
        constexpr Color(float red, float green, float blue, float alpha) noexcept : red(red), green(green), blue(blue), alpha(alpha) {}
        constexpr Color(void) noexcept : red(0.f), green(0.f), blue(0.f), alpha(1.f) {}

        struct literals;

        using value_type = float;

        float red;
        float green;
        float blue;
        float alpha;

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto& get(void) noexcept { return std::get<3 - Index>(reinterpret_cast<std::tuple<float, float, float, float>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr const auto& get(void) const noexcept { return std::get<3 - Index>(reinterpret_cast<const std::tuple<float, float, float, float>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr auto&& get(void) noexcept { return std::get<3 - Index>(reinterpret_cast<std::tuple<float, float, float, float>&&>(*this)); }

        template <std::size_t Index>
            requires (Index < 4)
        inline constexpr const auto&& get(void) const noexcept { return std::get<3 - Index>(reinterpret_cast<const std::tuple<float, float, float, float>&&>(*this)); }
    };

    struct Color::literals {
        static constexpr Color Red {255, 0, 0};
        static constexpr Color Green {0, 255, 0};
        static constexpr Color Blue {0, 0, 255};
        static constexpr Color White {255, 255, 255};
        static constexpr Color Black {0, 0, 0};
        static constexpr Color Yellow {255, 255, 0};
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
