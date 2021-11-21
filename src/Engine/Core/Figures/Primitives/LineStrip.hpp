#pragma once

#include "../Shape.hpp"

namespace ge {

    class LineStrip : public Shape {
    public:
        template <AllConvertible<Vector2f>... Args>
        LineStrip(Color const& color, Args&&... points) noexcept;
        template <AllConvertible<Vector2f>... Args>
        LineStrip(Args&&... args) noexcept;

        LineStrip(std::size_t vertices, Color const& color) noexcept;
        LineStrip(std::size_t vertices = 0) noexcept;

        LineStrip(LineStrip const& lineStrip) noexcept;
        LineStrip(LineStrip&& lineStrip) noexcept;

        LineStrip& operator= (LineStrip const& lineStrip) noexcept;
        LineStrip& operator= (LineStrip&& lineStrip) noexcept;

        Vector2f getCenter(void) const noexcept;

        void draw(void) const noexcept final;

        ~LineStrip(void) noexcept = default;
    private:
        typedef std::vector<Vertex>         Vertices;

        template <typename... Args>
        static Vertices createVertices(Color const& color,
            Args&&... args) noexcept;
    };

    template <AllConvertible<Vector2f>... Args>
    LineStrip::LineStrip(Color const& color, Args&&... points) noexcept
        : Shape{createVertices(color, std::forward<Args>(points)...)}
    {
        generateBuffers();
    }

    template <AllConvertible<Vector2f>... Args>
    LineStrip::LineStrip(Args&&... points) noexcept
        : Shape{createVertices({}, std::forward<Args>(points)...)}
    {
        generateBuffers();
    }

    template <typename... Args>
    LineStrip::Vertices LineStrip::createVertices(Color const& color,
        Args&&... args) noexcept
    {
        Vertices vertices;
        vertices.reserve(sizeof...(Args) + 1);
        (vertices.emplace_back(args, color), ...);
        vertices.push_back(vertices.front());
        return vertices;
    }

}
