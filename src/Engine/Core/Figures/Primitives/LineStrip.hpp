#pragma once

#include "../Shape.hpp"

namespace ge {

    class LineStrip : public Shape {
    public:
        LineStrip(const Vector2i& scene, std::size_t vertices, const Color& color) noexcept;
        LineStrip(const Vector2i& scene, std::size_t vertices) noexcept;

        LineStrip(const LineStrip& lineStrip) noexcept;
        LineStrip(LineStrip&& lineStrip) noexcept;

        LineStrip& operator= (const LineStrip& lineStrip) noexcept;
        LineStrip& operator= (LineStrip&& lineStrip) noexcept;

        Vector2f getCenter(void) const noexcept;

        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        ~LineStrip(void) noexcept = default;
    };

}
