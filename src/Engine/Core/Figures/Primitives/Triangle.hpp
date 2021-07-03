#pragma once

#include "../Shape.hpp"

namespace ge {

    template <bool PolygonMode = false>
    class Triangle : public Shape {
    public:
        Triangle(const Vector2i& scene,        const Vector2f& firstVertex,
                 const Vector2f& secondVertex, const Vector2f& thirdVertex,
                 const Color& color) noexcept;
        Triangle(const Vector2i& scene) noexcept;

        Triangle(const Triangle& triangle) noexcept;
        Triangle(Triangle&& triangle) noexcept;

        Triangle& operator= (const Triangle& triangle) noexcept;
        Triangle& operator= (Triangle&& triangle) noexcept;

        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        ~Triangle(void) noexcept = default;
    };

    template class Triangle<true>;
    template class Triangle<false>;

    typedef Triangle<true> PolygonTriangle;

}