#pragma once

#include "Angular.hpp"

#include <optional>

namespace mpgl {

    class ResizableAngular : public Angular {
    public:
        typedef std::size_t                 size_type;
        typedef std::optional<Vector2f>     OptionalVec2f;

        virtual void draw(void) const noexcept = 0;

        void resize(size_type size);
        void reserve(size_type size);

        void push(Vertex vertex);
        void emplace(Vector2f const& position,
            Color const& color);
        Vertex pop(void) noexcept;

        OptionalVec2f getCenter(void) const noexcept;

        virtual ~ResizableAngular(void) noexcept = default;
    protected:
        explicit ResizableAngular(size_t size, Color const& color);
        explicit ResizableAngular(Vertices vertices);

        explicit ResizableAngular(
            ResizableAngular const& shape) noexcept;
        explicit ResizableAngular(
            ResizableAngular&& shape) noexcept;

        template <class ColorTp, AllConvertible<Vector2f>... Args>
            requires std::constructible_from<Color, ColorTp>
        explicit ResizableAngular(ColorTp&& color,
            Args&&... vertices);
        template <AllConvertible<Vector2f>... Args>
        explicit ResizableAngular(Args&&... vertices);

        ResizableAngular& operator= (
            ResizableAngular&& shape) noexcept;
        ResizableAngular& operator= (
            ResizableAngular const& shape);
    };

    template <class ColorTp, AllConvertible<Vector2f>... Args>
        requires std::constructible_from<Color, ColorTp>
    ResizableAngular::ResizableAngular(ColorTp&& color,
        Args&&... vertices)
            : Angular{Vertices{Vertex{std::forward<Args>(vertices),
                color}...}} {}

    template <AllConvertible<Vector2f>... Args>
    ResizableAngular::ResizableAngular(Args&&... vertices)
        : Angular{Vertices{Vertex{std::forward<Args>(vertices),
            {}}...}} {}

}
