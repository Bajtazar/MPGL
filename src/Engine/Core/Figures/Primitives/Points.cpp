#include "Points.hpp"
#include "../Views.hpp"
#include "../../../Utility/Ranges.hpp"

namespace ge {

    Points::Points(std::size_t vertices, Color const& color)
        : ResizableAngular{vertices, color} {}

    Points::Points(Points const& points)
        : ResizableAngular{
            static_cast<ResizableAngular const&>(points)} {}

    Points& Points::operator= (Points const& points) {
        ResizableAngular::operator=(points);
        return *this;
    }

    Points::Points(Points&& points) noexcept
        : ResizableAngular{
            static_cast<ResizableAngular&&>(points)} {}

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
