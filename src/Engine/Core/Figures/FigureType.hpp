#pragma once

#include <inttypes.h>
#include <concepts>

namespace ge {

    class FigureType {
    public:
        constexpr FigureType(uint16_t vertices) noexcept : vertices(vertices) {}

        constexpr uint16_t verticiesCount(void) const noexcept { return vertices; }
    protected:
        const uint16_t vertices;
    };

    namespace FigureTypes {

        constexpr FigureType Triangle {3};

    };

}
