#pragma once

#include <inttypes.h>

namespace ge {

    struct Color {
        constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha=0) noexcept : red((float)red/255), green((float)green/255), blue((float)blue/255), alpha((float)alpha/255) {}
        constexpr Color(void) noexcept : red(0.f), green(0.f), blue(0.f), alpha(0.f) {}

        struct literals;

        const float red;
        const float green;
        const float blue;
        const float alpha;
    };

    struct Color::literals {
        static constexpr Color Red {255, 0, 0};
        static constexpr Color Green {0, 255, 0};
        static constexpr Color Blue {0, 0, 255};
        static constexpr Color White {255, 255, 255};
        static constexpr Color Black {0, 0, 0};
    };

}
