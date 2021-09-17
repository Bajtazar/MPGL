#include "Triangle.hpp"
#include "../Views.hpp"

namespace ge {

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(const Vector2f& firstVertex, const Vector2f& secondVertex,
        const Vector2f& thirdVertex, const Color& color) noexcept : Triangle{color}
    {
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[2].position = thirdVertex;

    }

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(const Color& color) noexcept : Shape{3} {
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(const Triangle& triangle) noexcept : Shape{3} {
        shaderProgram = triangle.shaderProgram;
        std::ranges::copy(triangle, begin());
    }

    template <bool PolygonMode>
    Triangle<PolygonMode>& Triangle<PolygonMode>::operator= (const Triangle& triangle) noexcept {
        shaderProgram = triangle.shaderProgram;
        std::ranges::copy(triangle, begin());
        return *this;
    }

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(Triangle&& triangle) noexcept : Shape{std::move(triangle.vertices)} {
        vertexArrayObject = triangle.vertexArrayObject;
        vertexBuffer = triangle.vertexBuffer;
        shaderProgram = triangle.shaderProgram;
        triangle.vertexArrayObject = 0;
        triangle.vertexBuffer = 0;
    }

    template <bool PolygonMode>
    Triangle<PolygonMode>& Triangle<PolygonMode>::operator= (Triangle&& triangle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = triangle.vertexArrayObject;
        vertexBuffer = triangle.vertexBuffer;
        shaderProgram = triangle.shaderProgram;
        triangle.vertexArrayObject = 0;
        triangle.vertexBuffer = 0;
        return *this;
    }

    template <bool PolygonMode>
    void Triangle<PolygonMode>::draw(void) const noexcept {
        if constexpr (PolygonMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);    // maybe will be moved
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

}
