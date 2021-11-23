#include "Polygon.hpp"

#include "../../../Mathematics/Systems.hpp"

#include <numbers>
#include <iterator>

namespace ge {

    Polygon::Polygon(Vector2f const& center, float radius, std::size_t segments,
        Color const& color) noexcept : Shape{segments + 1}, center{center}
    {
        float increment = 2.f * std::numbers::pi_v<float> / (segments - 1), angle = 0.f;
        vertices.front().position = center;
        for (auto& position : vertices | std::views::drop(1) | ge::views::position) {
            position = polarToCartesian(Vector2f{radius, angle}) + center;
            angle += increment;
        }
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    Polygon::Polygon(Polygon const& circle) noexcept
        : Shape{circle.vertices.size()}, center{circle.center}
    {
        shaderProgram = circle.shaderProgram;
        vertices.clear();
        vertices.reserve(circle.vertices.size());
        std::ranges::copy(circle, std::back_inserter(vertices));
    }

    Polygon& Polygon::operator= (Polygon const& circle) noexcept
    {
        shaderProgram = circle.shaderProgram;
        vertices.clear();
        vertices.reserve(circle.vertices.size());
        std::ranges::copy(circle, std::back_inserter(vertices));
        return *this;
    }


    Polygon::Polygon(Polygon&& circle) noexcept
        : Shape{std::move(circle.vertices)}, center{std::move(circle.center)}
    {
        vertexArrayObject = circle.vertexArrayObject;
        vertexBuffer = circle.vertexBuffer;
        shaderProgram = std::move(circle.shaderProgram);
        circle.vertexArrayObject = 0;
        circle.vertexBuffer = 0;
    }

    Polygon& Polygon::operator= (Polygon&& circle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = circle.vertexArrayObject;
        vertexBuffer = circle.vertexBuffer;
        shaderProgram = std::move(circle.shaderProgram);
        circle.vertexArrayObject = 0;
        circle.vertexBuffer = 0;
        return *this;
    }

    void Polygon::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLE_FAN, 0,  vertices.size());
        glBindVertexArray(0);
    }

}