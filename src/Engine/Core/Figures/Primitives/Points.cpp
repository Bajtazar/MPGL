#include "Points.hpp"
#include "../Views.hpp"
#include "../../../Utility/Ranges.hpp"

namespace ge {

    Points::Points(std::size_t vertices, Color const& color)
        : ResizableShape{vertices, color} {}

    Points::Points(Points const& points)
        : ResizableShape{points.vertices, points.shaderProgram} {}

    Points& Points::operator= (Points const& points) {
        ResizableShape::operator=(points);
        return *this;
    }

    Points::Points(Points&& points) noexcept
        : ResizableShape{std::move(points.vertices),
            std::move(points.shaderProgram)}
    {
        moveShape(std::move(points));
    }

    Points& Points::operator= (Points&& points) noexcept {
        ResizableShape::operator=(std::move(points));
        return *this;
    }

    void Points::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_POINTS, 0,  vertices.size());
        glBindVertexArray(0);
    }

}
