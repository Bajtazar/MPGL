#include "Line.hpp"

namespace ge {

    Line::Line(Vector2f const& firstVertex, Vector2f const& secondVertex,
        Color const& color) noexcept : Shape{2}
    {
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[0].color = color;
        vertices[1].color = color;
    }

    Line::Line(Color const& color) noexcept : Shape{2} {
        vertices[0].color = color;
        vertices[1].color = color;
    }

    Line::Line(Line const& line) noexcept : Shape{2} {
        shaderProgram = line.shaderProgram;
        std::ranges::copy(line, begin());
    }

    Line& Line::operator= (Line const& line) noexcept {
        shaderProgram = line.shaderProgram;
        std::ranges::copy(line, begin());
        return *this;
    }

    Line::Line(Line&& line) noexcept
        : Shape{std::move(line.vertices)}
    {
        vertexArrayObject = line.vertexArrayObject;
        vertexBuffer = line.vertexBuffer;
        shaderProgram = std::move(line.shaderProgram);
        line.vertexArrayObject = 0;
        line.vertexBuffer = 0;
    }

    Line& Line::operator= (Line&& line) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = line.vertexArrayObject;
        vertexBuffer = line.vertexBuffer;
        shaderProgram = std::move(line.shaderProgram);
        line.vertexArrayObject = 0;
        line.vertexBuffer = 0;
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
