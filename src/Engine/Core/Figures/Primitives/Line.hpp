#pragma once

#include "../Angular.hpp"

namespace ge {

    class Line : public Angular {
    public:
        Line(Vector2f const& firstVertex,
            Vector2f const& secondVertex,
            Color const& color);
        Line(Color const& color = {});

        Line(Line const& line);
        Line(Line&& line) noexcept;

        Line& operator= (Line const& line);
        Line& operator= (Line&& line) noexcept;

        Vector2f getLineCenter(void) const noexcept;

        void draw(void) const noexcept final;

        ~Line(void) noexcept = default;
    };

}
