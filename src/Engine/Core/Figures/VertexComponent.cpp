#include "VertexComponent.hpp"

namespace ge {

    template <FigureTypes::PrimitveType Type>
    VertexComponent::VertexComponent(const Vector2i&  scene, Type type) noexcept : VertexComponent(scene) {

    }

    template <FigureTypes::LineType Type>
    VertexComponent::VertexComponent(const Vector2i&  scene, Type type) noexcept : VertexComponent(scene) {

    }

    VertexComponent::VertexComponent(const Vector2i&  scene) noexcept : Drawable(scene) {
        glGenBuffers(1, &verticesBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
    }

    void VertexComponent::setShaders() const noexcept {

    }

    void VertexComponent::copyToGPU(void) const noexcept {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
    }

    void VertexComponent::draw(RenderWindow& target) const noexcept {

    }

}
