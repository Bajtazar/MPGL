#pragma once

#include "../Shape.hpp"

namespace ge {

    class Points : public Shape {
    public:
        Points(std::size_t vertices, const Color& color) noexcept;
        Points(std::size_t vertices = 0) noexcept;

        Points(const Points& points) noexcept;
        Points(Points&& points) noexcept;

        Points& operator= (const Points& points) noexcept;
        Points& operator= (Points&& points) noexcept;

        Vector2f getCenter(void) const noexcept;

        void draw(void) const noexcept final;

        ~Points(void) noexcept = default;
    };

}
