#pragma once

#include "../ResizableAngular.hpp"

namespace ge {

    class Polygon : public ResizableAngular {
    public:
        Polygon(Vector2f const& center = {}, float32 radius = 0.f,
            std::size_t segments = 0, Color const& color = {});

        template <class ColorTp, AllConvertible<Vector2f>... Args>
            requires std::constructible_from<Color, ColorTp>
        Polygon(ColorTp&& color, Args&&... vertices);

        template <AllConvertible<Vector2f>... Args>
        Polygon(Args&&... vertices);

        Polygon(Polygon const& polygon);
        Polygon(Polygon&& polygon) noexcept;

        Polygon& operator= (Polygon const& polygon);
        Polygon& operator= (Polygon&& polygon) noexcept;

        void draw(void) const noexcept;

        ~Polygon(void) noexcept = default;
    };

    template <class ColorTp, AllConvertible<Vector2f>... Args>
        requires std::constructible_from<Color, ColorTp>
    Polygon::Polygon(ColorTp&& color, Args&&... args)
        : ResizableAngular{std::forward<ColorTp>(color),
            std::forward<Args>(vertices)...} {}

    template <AllConvertible<Vector2f>... Args>
    Polygon::Polygon(Args&&... args)
        : ResizableAngular{std::forward<Args>(vertices)...} {}

}
