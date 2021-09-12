#pragma once

#include "../Shape.hpp"

namespace ge {

    class Points : public Shape {
    public:
        Points(const std::shared_ptr<Vector2ui>& scene, std::size_t vertices, const Color& color) noexcept;
        Points(const std::shared_ptr<Vector2ui>& scene, std::size_t vertices) noexcept;

        Points(const Points& points) noexcept;
        Points(Points&& points) noexcept;

        Points& operator= (const Points& points) noexcept;
        Points& operator= (Points&& points) noexcept;

        Vector2f getCenter(void) const noexcept;

        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        ~Points(void) noexcept = default;
    };

}
