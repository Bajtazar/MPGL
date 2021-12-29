#include "Triangle.hpp"
#include "../Views.hpp"

namespace ge {

    Triangle::Triangle(Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color)
            : Shape{{Vertex{firstVertex, color},
                Vertex{secondVertex, color},
                Vertex{thirdVertex, color}}, "2DDefault"} {}

    Triangle::Triangle(Color const& color)
        : Shape{3, color} {}

    Triangle::Triangle(Triangle const& triangle)
        : Shape{triangle.vertices, triangle.shaderProgram} {}

    Triangle& Triangle::operator= (
        Triangle const& triangle)
    {
        Shape::operator=(triangle);
        return *this;
    }

    Triangle::Triangle(Triangle&& triangle) noexcept
        : Shape{std::move(triangle.vertices),
            std::move(triangle.shaderProgram)}
    {
        moveShape(std::move(triangle));
    }

    Triangle& Triangle::operator= (Triangle&& triangle) noexcept {
        Shape::operator=(std::move(triangle));
        return *this;
    }

    void Triangle::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

}
