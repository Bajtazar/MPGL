#pragma once

#include "Shape.hpp"

#include <optional>

namespace ge {

    class ResizableShape : public Shape {
    public:
        typedef std::size_t                 size_type;
        typedef std::optional<Vector2f>     OptionalVec2f;

        ResizableShape(ResizableShape const& shape) = delete;
        ResizableShape(ResizableShape&& shape) noexcept;

        ResizableShape& operator= (
            ResizableShape&& shape) noexcept;

        virtual void draw(void) const noexcept = 0;

        void resize(size_type size);
        void reserve(size_type size);

        void push(Vertex vertex);
        void emplace(Vector2f const& position,
            Color const& color);
        Vertex pop(void) noexcept;

        OptionalVec2f getCenter(void) const noexcept;

        virtual ~ResizableShape(void) noexcept = default;
    protected:
        using Shape::Shape;

        template <class ColorTp, AllConvertible<Vector2f>... Args>
            requires std::constructible_from<Color, ColorTp>
        explicit ResizableShape(ColorTp&& color,
            Args&&... vertices);
        template <AllConvertible<Vector2f>... Args>
        explicit ResizableShape(Args&&... vertices);

        ResizableShape& operator= (
            ResizableShape const& shape);
    };

    template <class ColorTp, AllConvertible<Vector2f>... Args>
        requires std::constructible_from<Color, ColorTp>
    ResizableShape::ResizableShape(ColorTp&& color,
        Args&&... vertices)
            : Shape{Vertices{Vertex{std::forward<Args>(vertices),
                color}...}, "2DDefault"} {}

    template <AllConvertible<Vector2f>... Args>
    ResizableShape::ResizableShape(Args&&... vertices)
        : Shape{Vertices{Vertex{std::forward<Args>(vertices),
            {}}...}, "2DDefault"} {}

}
