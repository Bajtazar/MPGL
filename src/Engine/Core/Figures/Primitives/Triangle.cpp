#include "Triangle.hpp"
#include "../Views.hpp"

namespace ge {

    constexpr std::size_t VertexSizeTriangle = 18 * sizeof(float);

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(const Vector2i& scene, const Vector2f& firstVertex,
        const Vector2f& secondVertex, const Vector2f& thirdVertex, const Color& color) noexcept
        : Shape{scene, 3} {
            vertices[0].position = firstVertex;
            vertices[1].position = secondVertex;
            vertices[2].position = thirdVertex;
            for (auto& color_ : vertices | ge::views::color)
                color_ = color;
        }

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(const Vector2i& scene) noexcept : Shape{scene, 3} {}

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(const Triangle& triangle) noexcept : Shape{triangle.scene, 3} {
        shaderProgram = triangle.shaderProgram;
        std::ranges::copy(triangle, begin());
    }

    template <bool PolygonMode>
    Triangle<PolygonMode>& Triangle<PolygonMode>::operator= (const Triangle& triangle) noexcept {
        shaderProgram = triangle.shaderProgram;
        std::ranges::copy(triangle, begin());
        copyToGPU();
        return *this;
    }

    template <bool PolygonMode>
    Triangle<PolygonMode>::Triangle(Triangle&& triangle) noexcept : Shape{triangle.scene, std::move(triangle.vertices)} {
        vertexArrayObject = triangle.vertexArrayObject;
        vertexBuffer = triangle.vertexBuffer;
        shaderProgram = triangle.shaderProgram;
    }

    template <bool PolygonMode>
    Triangle<PolygonMode>& Triangle<PolygonMode>::operator= (Triangle&& triangle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = triangle.vertexArrayObject;
        vertexBuffer = triangle.vertexBuffer;
        shaderProgram = triangle.shaderProgram;
        return *this;
    }

    template <bool PolygonMode>
    void Triangle<PolygonMode>::copyToGPU(void) noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, VertexSizeTriangle, vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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
