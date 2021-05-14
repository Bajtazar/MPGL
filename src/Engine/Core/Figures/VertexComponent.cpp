#include "VertexComponent.hpp"

namespace ge {

    VertexComponent::VertexComponent(const Vector2i& scene, size_t size) noexcept : Drawable(scene), vertices(size) {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &verticesBuffer);
    }

    void VertexComponent::setShaders(const ShaderLibrary& shaderLibrary) noexcept {
        shaderProgram = shaderLibrary["2DDefault"];
    }

    VertexComponent::~VertexComponent(void) noexcept {
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(1, &verticesBuffer);
    }

    void VertexComponent::copyToGPU(void) noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void VertexComponent::draw(void) const noexcept {
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

}
