#pragma once

#include "../Shape.hpp"

#include <array>

namespace ge {

    class Rectangle : public Shape {
    public:
        Rectangle(const Color& color) noexcept;
        // parallelogram
        Rectangle(const Vector2f& firstVertex,      const Vector2f& secondVertex,
                  const Vector2f& thirdVertex,      const Color& color = {}) noexcept;
        // rectangle parallel to the x and y axis
        Rectangle(const Vector2f& firstVertex,      const Vector2f& dimmensions,
                  const Color& color = {}) noexcept;
        Rectangle(const Rectangle& rectangle) noexcept;
        Rectangle(Rectangle&& rectangle) noexcept;

        Rectangle& operator= (const Rectangle& rectangle) noexcept;
        Rectangle& operator= (Rectangle&& rectangle) noexcept;

        void draw(void) const noexcept final;

        ~Rectangle(void) noexcept;
    private:
        static const std::array<uint32_t, 6> indexes;

        uint32_t elementArrayBuffer;

        void bindBuffers(void) const noexcept override;
        void unbindBuffers(void) const noexcept override;
    };

}
