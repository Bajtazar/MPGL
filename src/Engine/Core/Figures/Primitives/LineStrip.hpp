#pragma once

#include "../ResizableShape.hpp"

#include <optional>

namespace ge {

    class LineStrip : public ResizableShape {
    public:
        template <class ColorTp, AllConvertible<Vector2f>... Args>
            requires std::constructible_from<Color, ColorTp>
        LineStrip(ColorTp&& color, Args&&... vertices);

        template <AllConvertible<Vector2f>... Args>
        LineStrip(Args&&... vertices);

        LineStrip(std::size_t vertices = 0,
            Color const& color = {});

        LineStrip(LineStrip const& lineStrip);
        LineStrip(LineStrip&& lineStrip) noexcept;

        LineStrip& operator= (LineStrip const& lineStrip);
        LineStrip& operator= (LineStrip&& lineStrip) noexcept;

        void draw(void) const noexcept final;

        ~LineStrip(void) noexcept = default;
    };

    template <class ColorTp, AllConvertible<Vector2f>... Args>
        requires std::constructible_from<Color, ColorTp>
    LineStrip::LineStrip(ColorTp&& color, Args&&... vertices)
        : ResizableShape{std::forward<Color>(color),
            std::forward<Args>(vertices)...}
    {
        this->vertices.push_back(this->vertices.front());
    }

    template <AllConvertible<Vector2f>... Args>
    LineStrip::LineStrip(Args&&... vertices)
        : ResizableShape{std::forward<Args>(vertices)...}
    {
        this->vertices.push_back(this->vertices.front());
    }

}
