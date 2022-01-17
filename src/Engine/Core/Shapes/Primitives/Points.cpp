#include "Points.hpp"
#include "../Views.hpp"
#include "../../../Utility/Ranges.hpp"

namespace ge {

    Points::Points(std::size_t vertices, Color const& color)
        : ResizableAngular{vertices, color} {}

    Points::Points(Points const& points)
        : ResizableAngular{points.vertices, points.shaderProgram} {}

    Points& Points::operator= (Points const& points) {
        ResizableAngular::operator=(points);
        return *this;
    }

    Points::Points(Points&& points) noexcept
        : ResizableAngular{std::move(points.vertices),
            std::move(points.shaderProgram)}
    {
        moveAngular(std::move(points));
    }

    Points& Points::operator= (Points&& points) noexcept {
        ResizableAngular::operator=(std::move(points));
        return *this;
    }

    void Points::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_POINTS, 0,  vertices.size());
        glBindVertexArray(0);
    }

}
