#include "Line.hpp"

namespace ge {

    Line::Line(Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Color const& color) : Shape{
            Vertices{Vertex{firstVertex, color},
            Vertex{secondVertex, color}}, "2DDefault"} {}

    Line::Line(Color const& color) : Shape{2, color} {}

    Line::Line(Line const& line)
        : Shape{line.vertices, line.shaderProgram} {}

    Line& Line::operator= (Line const& line) {
        Shape::operator=(line);
        return *this;
    }

    Line::Line(Line&& line) noexcept
        : Shape{std::move(line.vertices),
            std::move(line.shaderProgram)}
    {
        moveShape(std::move(line));
    }

    Line& Line::operator= (Line&& line) noexcept {
        Shape::operator=(std::move(line));
        return *this;
    }

    Vector2f Line::getLineCenter(void) const noexcept {
        return (Vector2f{vertices[0].position}
            + Vector2f{vertices[1].position}) / 2.f;
    }

    void Line::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }

}
