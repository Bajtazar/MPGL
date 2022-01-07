#include "Polygon.hpp"

#include "../../../Mathematics/Systems.hpp"
#include "../Views.hpp"

#include <numbers>
#include <iterator>

namespace ge {

    Polygon::Polygon(Vector2f const& center,
        float32 radius, std::size_t segments,
        Color const& color)
            : ResizableShape{segments + 1, color}
    {
        float32 increment = 2.f *
            std::numbers::pi_v<float32> / (segments - 1), angle = 0.f;
        vertices.front().position = center;
        for (auto& position : vertices | std::views::drop(1) |
            ge::views::position)
        {
            position = polarToCartesian(Vector2f{radius, angle}) + center;
            angle += increment;
        }
    }

    Polygon::Polygon(Polygon const& polygon)
        : ResizableShape{polygon.vertices, polygon.shaderProgram} {}

    Polygon& Polygon::operator= (Polygon const& polygon) {
        ResizableShape::operator=(polygon);
        return *this;
    }


    Polygon::Polygon(Polygon&& polygon) noexcept
        : ResizableShape{std::move(polygon.vertices),
        std::move(shaderProgram)}
    {
        moveShape(std::move(polygon));
    }

    Polygon& Polygon::operator= (Polygon&& polygon) noexcept {
        ResizableShape::operator=(std::move(polygon));
        return *this;
    }

    void Polygon::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLE_FAN, 0,  vertices.size());
        glBindVertexArray(0);
    }

}