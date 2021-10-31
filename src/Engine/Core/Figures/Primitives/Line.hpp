#pragma once

#include "../Shape.hpp"

namespace ge {

    class Line : public Shape {
    public:
        Line(Vector2f const& firstVertex, Vector2f const& secondVertex,
             Color const& color) noexcept;
        Line(Color const& color = {}) noexcept;

        Line(Line const& line) noexcept;
        Line(Line&& line) noexcept;

        Line& operator= (Line const& line) noexcept;
        Line& operator= (Line&& line) noexcept;

        Vector2f getLineCenter(void) const noexcept;

        void draw(void) const noexcept final;

        ~Line(void) noexcept = default;
    };

}
