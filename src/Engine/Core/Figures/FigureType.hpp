#pragma once

#include <inttypes.h>
#include <concepts>

namespace ge {

    class FigureType {
    public:
        constexpr FigureType(uint16_t vertices) noexcept : vertices(vertices) {}

        constexpr uint16_t verticiesCount(void) const noexcept { return vertices; }
        virtual consteval bool isLineType(void) const noexcept = 0;
    protected:
        const uint16_t vertices;
    };

    class LineType : public FigureType {
    public:
        using FigureType::FigureType;

        virtual consteval bool isLineType(void) const noexcept final { return true; }
    };

    class BasicType : public FigureType {
    public:
        using FigureType::FigureType;

        virtual consteval bool isLineType(void) const noexcept final { return false; }
    };

    namespace FigureTypes {
        constexpr BasicType Triangle {3};

        template <class Figure>
        concept LineType =  std::same_as<Figure, ge::LineType>;

        template <class Figure>
        concept PrimitveType =  std::same_as<Figure, BasicType>;
    };

}
