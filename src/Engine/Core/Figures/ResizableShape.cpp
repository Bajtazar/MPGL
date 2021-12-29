#include "Views.hpp"
#include "ResizableShape.hpp"
#include "../../Utility/Ranges.hpp"

namespace ge {

    ResizableShape::ResizableShape(ResizableShape&& shape) noexcept
        : Shape{std::move(shape.vertices),
            std::move(shape.shaderProgram)}
    {
        moveShape(std::move(shape));
    }

    ResizableShape& ResizableShape::operator= (
        ResizableShape const& shape)
    {
        Shape::operator=(shape);
        return *this;
    }

    ResizableShape& ResizableShape::operator= (
        ResizableShape&& shape) noexcept
    {
        Shape::operator=(std::move(shape));
        return *this;
    }

    ResizableShape::OptionalVec2f
        ResizableShape::getCenter(void) const noexcept
    {
        if (!vertices.size())
            return {};
        return { accumulate(*this | views::position, Vector2f{},
            [](auto const& x){ return std::forward<Vector2f>(x); })
                / static_cast<float>(vertices.size()) };
    }

    void ResizableShape::resize(size_type size) {
        vertices.resize(size, Vertex{{}, {}});
    }

    void ResizableShape::reserve(size_type size) {
        vertices.reserve(size);
    }

    void ResizableShape::push(Vertex vertex) {
        vertices.push_back(std::move(vertex));
    }

    void ResizableShape::emplace(Vector2f const& position,
        Color const& color)
    {
        vertices.emplace_back(position, color);
    }

    ResizableShape::Vertex ResizableShape::pop(void) noexcept {
        auto last = std::move(vertices.back());
        vertices.pop_back();
        return last;
    }

}
