#pragma once

#include "../Shape.hpp"

namespace ge {

    class Circle : public Shape {
    public:
        Circle(Vector2f const& center, float radius, Color const& color,
            std::size_t segments=360) noexcept;
        Circle(Circle const& circle) noexcept;
        Circle(Circle&& circle) noexcept;

        Circle& operator= (Circle const& circle) noexcept;
        Circle& operator= (Circle&& circle) noexcept;

        void draw(void) const noexcept;

        Vector2f const& getCenter(void) const noexcept { return center; }

        ~Circle(void) noexcept = default;
    private:
        Vector2f center;
    };

}
