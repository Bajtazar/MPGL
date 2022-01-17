#pragma once

#include "../Angular.hpp"

namespace ge {

    class Triangle : public Angular {
    public:
        Triangle(Vector2f const& firstVertex,
                Vector2f const& secondVertex,
                Vector2f const& thirdVertex,
                Color const& color = {});
        Triangle(Color const& color = {});

        Triangle(Triangle const& triangle);
        Triangle(Triangle&& triangle) noexcept;

        Triangle& operator= (Triangle const& triangle);
        Triangle& operator= (Triangle&& triangle) noexcept;

        void draw(void) const noexcept final;

        ~Triangle(void) noexcept = default;
    };

}
