#pragma once

#include "../Shape.hpp"

#include <array>

namespace ge {

    class Rectangle : public Shape {
    public:
        Rectangle(const std::shared_ptr<Vector2i>& scene) noexcept;
        Rectangle(const std::shared_ptr<Vector2i>& scene, const Vector2f& firstVertex,
                  const Vector2f& secondVertex,           const Vector2f& thirdVertex,
                  const Color& color) noexcept;
        // parallelogram
        Rectangle(const std::shared_ptr<Vector2i>& scene, const Vector2f& firstVertex,
                  const Vector2f& dimmensions,            const Color& color) noexcept;
        // rectangle parallel to the x and y axis
        Rectangle(const Rectangle& rectangle) noexcept;
        Rectangle(Rectangle&& rectangle) noexcept;

        Rectangle& operator= (const Rectangle& rectangle) noexcept;
        Rectangle& operator= (Rectangle&& rectangle) noexcept;

        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        ~Rectangle(void) noexcept;
    private:
        static const std::array<uint32_t, 6> indexes;
        uint32_t elementArrayBuffer;
    };

}
