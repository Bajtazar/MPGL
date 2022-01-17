#pragma once

#include "../ResizableAngular.hpp"

#include <optional>

namespace ge {

    class Points : public ResizableAngular {
    public:
        Points(std::size_t vertices = 0, Color const& color = {});

        template <class ColorTp, AllConvertible<Vector2f>... Args>
            requires std::constructible_from<Color, ColorTp>
        Points(ColorTp&& color, Args&&... vertices);

        template <AllConvertible<Vector2f>... Args>
        Points(Args&&... vertices);

        Points(Points const& points);
        Points(Points&& points) noexcept;

        Points& operator= (Points const& points);
        Points& operator= (Points&& points) noexcept;

        void draw(void) const noexcept final;

        ~Points(void) noexcept = default;
    };

    template <class ColorTp, AllConvertible<Vector2f>... Args>
        requires std::constructible_from<Color, ColorTp>
    Points::Points(ColorTp&& color, Args&&... args)
        : ResizableAngular{std::forward<ColorTp>(color),
            std::forward<Args>(vertices)...} {}

    template <AllConvertible<Vector2f>... Args>
    Points::Points(Args&&... args)
        : ResizableAngular{std::forward<Args>(vertices)...} {}

}
