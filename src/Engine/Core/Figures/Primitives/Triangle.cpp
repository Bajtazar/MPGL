#include "Triangle.hpp"
#include "../Views.hpp"

namespace ge {

    Triangle::Triangle(Vector2f const& firstVertex, Vector2f const& secondVertex,
        Vector2f const& thirdVertex, const Color& color) noexcept
        : Shape{std::vector<Vertex>{
            Vertex{firstVertex, color}, Vertex{secondVertex, color},
            Vertex{thirdVertex, color}}}
    {
        generateBuffers();
    }

    Triangle::Triangle(Color const& color) noexcept : Shape{3} {
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    Triangle::Triangle(Triangle const& triangle) noexcept : Shape{3} {
        shaderProgram = triangle.shaderProgram;
        std::ranges::copy(triangle, begin());
    }

    Triangle& Triangle::operator= (Triangle const& triangle) noexcept {
        shaderProgram = triangle.shaderProgram;
        std::ranges::copy(triangle, begin());
        return *this;
    }

    Triangle::Triangle(Triangle&& triangle) noexcept
        : Shape{std::move(triangle.vertices)}
    {
        vertexArrayObject = triangle.vertexArrayObject;
        vertexBuffer = triangle.vertexBuffer;
        shaderProgram = std::move(triangle.shaderProgram);
        triangle.vertexArrayObject = 0;
        triangle.vertexBuffer = 0;
    }

    Triangle& Triangle::operator= (Triangle&& triangle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = triangle.vertexArrayObject;
        vertexBuffer = triangle.vertexBuffer;
        shaderProgram = std::move(triangle.shaderProgram);
        triangle.vertexArrayObject = 0;
        triangle.vertexBuffer = 0;
        return *this;
    }

    void Triangle::draw(void) const noexcept {
        shaderProgram.use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

}
