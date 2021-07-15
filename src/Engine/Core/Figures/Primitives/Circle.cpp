#include "Circle.hpp"
#include "../../../Mathematics/Systems.hpp"

#include <numbers>
#include <iterator>

namespace ge {

    Circle::Circle(const std::shared_ptr<Vector2i>& scene, const Vector2f& center, float radius, const Color& color, std::size_t segments) noexcept : Shape{scene, segments + 1}, center{center} {
        float increment = 2.f * std::numbers::pi_v<float> / (segments - 1), angle = 0.f;
        vertices.front().position = center;
        for (auto& position : vertices | std::views::drop(1) | ge::views::position) {
            position = polarToCartesian(Vector2f{radius, angle}) + center;
            angle += increment;
        }
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    Circle::Circle(const Circle& circle) noexcept : Shape{circle.scene, circle.vertices.size()}, center{circle.center} {
        shaderProgram = circle.shaderProgram;
        vertices.clear();
        vertices.reserve(circle.vertices.size());
        std::ranges::copy(circle, std::back_inserter(vertices));
    }

    Circle& Circle::operator= (const Circle& circle) noexcept {
        shaderProgram = circle.shaderProgram;
        vertices.clear();
        vertices.reserve(circle.vertices.size());
        std::ranges::copy(circle, std::back_inserter(vertices));
        return *this;
    }


    Circle::Circle(Circle&& circle) noexcept : Shape{circle.scene, std::move(circle.vertices)}, center{std::move(circle.center)} {
        vertexArrayObject = circle.vertexArrayObject;
        vertexBuffer = circle.vertexBuffer;
        shaderProgram = circle.shaderProgram;
        circle.vertexArrayObject = 0;
        circle.vertexBuffer = 0;
    }

    Circle& Circle::operator= (Circle&& circle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = circle.vertexArrayObject;
        vertexBuffer = circle.vertexBuffer;
        shaderProgram = circle.shaderProgram;
        circle.vertexArrayObject = 0;
        circle.vertexBuffer = 0;
        return *this;
    }

    void Circle::copyToGPU(void) noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Circle::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);    // maybe will be moved
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLE_FAN, 0,  vertices.size());
        glBindVertexArray(0);
    }

}