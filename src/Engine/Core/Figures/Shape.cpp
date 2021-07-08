#include "Shape.hpp"

namespace ge {

    Shape::Shape(const std::shared_ptr<Vector2i>& scene, size_t size) noexcept : Drawable{scene}, vertices{size, Vertex{{}, {}, scene}} {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
    }

    Shape::Shape(const std::shared_ptr<Vector2i>& scene, std::vector<Vertex>&& vertices) noexcept : Drawable{scene}, vertices{std::move(vertices)} {}

    void Shape::setShaders(const ShaderLibrary& shaderLibrary) noexcept {
        shaderProgram = shaderLibrary["2DDefault"];
    }

    void Shape::onScreenTransformation(const Vector2i& oldDimmensions) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Shape::Vertex::position)) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(oldDimmensions) / static_cast<Vector2f>(*scene) - 1.f;
        }
        copyToGPU();
    }

    Shape::~Shape(void) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
