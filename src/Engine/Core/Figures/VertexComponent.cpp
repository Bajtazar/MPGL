#include "VertexComponent.hpp"

namespace ge {

    VertexComponent::VertexComponent(const Vector2i&  scene, FigureTypes::PrimitveType auto type) noexcept : VertexComponent(scene) {

    }

    VertexComponent::VertexComponent(const Vector2i&  scene, FigureTypes::LineType auto type) noexcept : VertexComponent(scene) {

    }

    VertexComponent::VertexComponent(const Vector2i&  scene) noexcept : Drawable(scene) {
        glGenBuffers(1, &verticesBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
    }

    void VertexComponent::setShaders(const ShaderLibrary&) const noexcept {

    }

    void VertexComponent::copyToGPU(void) const noexcept {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
    }

    void VertexComponent::draw(void) const noexcept {

    }

}
