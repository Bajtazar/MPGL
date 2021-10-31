#pragma once

#include "../Shape.hpp"

#include <array>

namespace ge {

    class Rectangle : public Shape {
    public:
        Rectangle(Color const& color) noexcept;
        // parallelogram
        Rectangle(Vector2f const& firstVertex,      Vector2f const& secondVertex,
                  Vector2f const& thirdVertex,      Color const& color = {}) noexcept;
        // rectangle parallel to the x and y axis
        Rectangle(Vector2f const& firstVertex,      Vector2f const& dimmensions,
                  Color const& color = {}) noexcept;
        Rectangle(Rectangle const& rectangle) noexcept;
        Rectangle(Rectangle&& rectangle) noexcept;

        Rectangle& operator= (Rectangle const& rectangle) noexcept;
        Rectangle& operator= (Rectangle&& rectangle) noexcept;

        void draw(void) const noexcept final;

        ~Rectangle(void) noexcept;
    private:
        static const std::array<uint32_t, 6>        indexes;

        uint32_t                                    elementArrayBuffer;

        void bindBuffers(void) const noexcept override;
        void unbindBuffers(void) const noexcept override;
    };

}
