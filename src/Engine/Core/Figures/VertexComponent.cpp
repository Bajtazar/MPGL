#include "VertexComponent.hpp"

namespace ge {

    template <bool PolygonMode>
    VertexComponent<PolygonMode>::VertexComponent(const Vector2i& scene, size_t size) noexcept : Drawable(scene), vertices{size, Vertex{{}, {}, scene}} {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &verticesBuffer);
    }

    template <bool PolygonMode>
    void VertexComponent<PolygonMode>::setShaders(const ShaderLibrary& shaderLibrary) noexcept {
        shaderProgram = shaderLibrary["2DDefault"];
    }

    template <bool PolygonMode>
    VertexComponent<PolygonMode>::~VertexComponent(void) noexcept {
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(1, &verticesBuffer);
    }

    template <bool PolygonMode>
    void VertexComponent<PolygonMode>::copyToGPU(void) noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    template <bool PolygonMode>
    void VertexComponent<PolygonMode>::draw(void) const noexcept {
        if constexpr (PolygonMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

}
