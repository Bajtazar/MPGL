#pragma once

#include "../Shape.hpp"

namespace ge {

    class LineStrip : public Shape {
    public:
        LineStrip(std::size_t vertices, const Color& color) noexcept;
        LineStrip(std::size_t vertices = 0) noexcept;

        LineStrip(const LineStrip& lineStrip) noexcept;
        LineStrip(LineStrip&& lineStrip) noexcept;

        LineStrip& operator= (const LineStrip& lineStrip) noexcept;
        LineStrip& operator= (LineStrip&& lineStrip) noexcept;

        Vector2f getCenter(void) const noexcept;

        void draw(void) const noexcept final;

        ~LineStrip(void) noexcept = default;
    };

}
