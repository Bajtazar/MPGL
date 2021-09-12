#pragma once

#include "../Shape.hpp"

namespace ge {

    class Line : public Shape {
    public:
        Line(const std::shared_ptr<Vector2ui>& scene, const Vector2f& firstVertex,
             const Vector2f& secondVertex, const Color& color) noexcept;
        Line(const std::shared_ptr<Vector2ui>& scene, const Color& color) noexcept;

        Line(const Line& line) noexcept;
        Line(Line&& line) noexcept;

        Line& operator= (const Line& line) noexcept;
        Line& operator= (Line&& line) noexcept;

        Vector2f getLineCenter(void) const noexcept;

        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        ~Line(void) noexcept = default;
    };

}
