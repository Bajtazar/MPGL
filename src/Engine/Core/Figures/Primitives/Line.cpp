#include "Line.hpp"

namespace mpgl {

    Line::Line(Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Color const& color) : Angular{
            Vertices{Vertex{firstVertex, color},
            Vertex{secondVertex, color}}} {}

    Line::Line(Color const& color) : Angular{2, color} {}

    Line::Line(Line const& line)
        : Angular{line} {}

    Line& Line::operator= (Line const& line) {
        Angular::operator=(line);
        return *this;
    }

    Line::Line(Line&& line) noexcept
        : Angular{std::move(line)} {}

    Line& Line::operator= (Line&& line) noexcept {
        Angular::operator=(std::move(line));
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
