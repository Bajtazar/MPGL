#include "Views.hpp"
#include "ResizableAngular.hpp"
#include "../../Utility/Ranges.hpp"

namespace mpgl {

    ResizableAngular::ResizableAngular(size_t size,
        Color const& color) : Angular{size, color} {}

    ResizableAngular::ResizableAngular(Vertices vertices)
        : Angular{vertices} {}

    ResizableAngular::ResizableAngular(
        ResizableAngular const& shape) noexcept
            : Angular{shape} {}

    ResizableAngular::ResizableAngular(
        ResizableAngular&& shape) noexcept
            : Angular{std::move(shape)} {}

    ResizableAngular& ResizableAngular::operator= (
        ResizableAngular const& shape)
    {
        Angular::operator=(shape);
        return *this;
    }

    ResizableAngular& ResizableAngular::operator= (
        ResizableAngular&& shape) noexcept
    {
        Angular::operator=(std::move(shape));
        return *this;
    }

    ResizableAngular::OptionalVec2f
        ResizableAngular::getCenter(void) const noexcept
    {
        if (!vertices.size())
            return {};
        return { accumulate(*this | views::position, Vector2f{},
            [](auto const& x){ return std::forward<Vector2f>(x); })
                / static_cast<float32>(vertices.size()) };
    }

    void ResizableAngular::resize(size_type size) {
        vertices.resize(size, Vertex{{}, {}});
    }

    void ResizableAngular::reserve(size_type size) {
        vertices.reserve(size);
    }

    void ResizableAngular::push(Vertex vertex) {
        vertices.push_back(std::move(vertex));
    }

    void ResizableAngular::emplace(Vector2f const& position,
        Color const& color)
    {
        vertices.emplace_back(position, color);
    }

    ResizableAngular::Vertex ResizableAngular::pop(void) noexcept {
        auto last = std::move(vertices.back());
        vertices.pop_back();
        return last;
    }

}
