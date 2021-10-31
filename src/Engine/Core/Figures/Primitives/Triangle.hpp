#pragma once

#include "../Shape.hpp"

namespace ge {

    template <bool PolygonMode = false>
    class Triangle : public Shape {
    public:
        Triangle(Vector2f const& firstVertex,   Vector2f const& secondVertex,
                 Vector2f const& thirdVertex,   Color const& color = {}) noexcept;
        Triangle(Color const& color = {}) noexcept;

        Triangle(Triangle const& triangle) noexcept;
        Triangle(Triangle&& triangle) noexcept;

        Triangle& operator= (Triangle const& triangle) noexcept;
        Triangle& operator= (Triangle&& triangle) noexcept;

        void draw(void) const noexcept final;

        ~Triangle(void) noexcept = default;
    };

    template class Triangle<true>;
    template class Triangle<false>;

    typedef Triangle<true>      PolygonTriangle;
    typedef Triangle<false>     DefaultTriangle;

}
