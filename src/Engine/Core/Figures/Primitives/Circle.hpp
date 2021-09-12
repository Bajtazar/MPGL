#pragma once

#include "../Shape.hpp"

namespace ge {

    class Circle : public Shape {
    public:
        Circle(const std::shared_ptr<Vector2ui>& scene, const Vector2f& center, float radius, const Color& color, std::size_t segments=360) noexcept;
        Circle(const Circle& circle) noexcept;
        Circle(Circle&& circle) noexcept;

        Circle& operator= (const Circle& circle) noexcept;
        Circle& operator= (Circle&& circle) noexcept;

        virtual void copyToGPU(void) noexcept;
        virtual void draw(void) const noexcept;

        const Vector2f& getCenter(void) const noexcept { return center; }

        ~Circle(void) noexcept = default;
    private:
        Vector2f center;
    };

}
