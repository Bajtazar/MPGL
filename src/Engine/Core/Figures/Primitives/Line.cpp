#include "Line.hpp"

namespace ge {

    Line::Line(const std::shared_ptr<Vector2ui>& scene, const Vector2f& firstVertex, const Vector2f& secondVertex, const Color& color) noexcept : Shape{scene, 2} {
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[0].color = color;
        vertices[1].color = color;
    }

    Line::Line(const std::shared_ptr<Vector2ui>& scene, const Color& color) noexcept : Shape{scene, 2} {
        vertices[0].color = color;
        vertices[1].color = color;
    }

    Line::Line(const Line& line) noexcept : Shape{line.scene, 2} {
        shaderProgram = line.shaderProgram;
        std::ranges::copy(line, begin());
    }

    Line& Line::operator= (const Line& line) noexcept {
        shaderProgram = line.shaderProgram;
        std::ranges::copy(line, begin());
        return *this;
    }

    Line::Line(Line&& line) noexcept : Shape{line.scene, std::move(line.vertices)} {
        vertexArrayObject = line.vertexArrayObject;
        vertexBuffer = line.vertexBuffer;
        shaderProgram = line.shaderProgram;
        line.vertexArrayObject = 0;
        line.vertexBuffer = 0;
    }

    Line& Line::operator= (Line&& line) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = line.vertexArrayObject;
        vertexBuffer = line.vertexBuffer;
        shaderProgram = line.shaderProgram;
        line.vertexArrayObject = 0;
        line.vertexBuffer = 0;
        return *this;
    }

    Vector2f Line::getLineCenter(void) const noexcept {
        return (Vector2f{vertices[0].position} + Vector2f{vertices[1].position}) / 2.f;
    }

    void Line::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);    // maybe will be moved
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
    }

}
