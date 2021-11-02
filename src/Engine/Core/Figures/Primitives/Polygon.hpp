#pragma once

#include "../Shape.hpp"

namespace ge {

    class Polygon : public Shape {
    public:
        Polygon(Vector2f const& center, float radius,
            std::size_t segments, Color const& color) noexcept;
        Polygon(Polygon const& circle) noexcept;
        Polygon(Polygon&& circle) noexcept;

        Polygon& operator= (Polygon const& circle) noexcept;
        Polygon& operator= (Polygon&& circle) noexcept;

        void draw(void) const noexcept;

        Vector2f const& getCenter(void) const noexcept { return center; }

        ~Polygon(void) noexcept = default;
    private:
        Vector2f center;
    };

}
