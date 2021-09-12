#include "Shape.hpp"
#include "../../Mathematics/Systems.hpp"

namespace ge {

    Shape::Shape(const std::shared_ptr<Vector2ui>& scene, size_t size) noexcept : Drawable{scene}, vertices{size, Vertex{{}, {}, scene}} {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
    }

    Shape::Shape(const std::shared_ptr<Vector2ui>& scene, std::vector<Vertex>&& vertices) noexcept : Drawable{scene}, vertices{std::move(vertices)} {}

    void Shape::setShaders(const ShaderLibrary& shaderLibrary) noexcept {
        shaderProgram = shaderLibrary["2DDefault"];
    }

    void Shape::onScreenTransformation(const Vector2ui& oldDimmensions) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Shape::Vertex::position)) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * vectorCast<float>(oldDimmensions) / vectorCast<float>(*scene) - 1.f;
        }
        copyToGPU();
    }

    void Shape::translate(const Vector2f& shift) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Shape::Vertex::position))
            vertexPosition = Vector2f(vertexPosition) + shift;
        copyToGPU();
    }

    void Shape::rotate(const Vector2f& center, float angle) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Shape::Vertex::position)) {
            Vector2f position = vertexPosition;
            Vector2f radius = position - center;
            vertexPosition = rotationMatrix<float>(angle) * radius + center;
        }
        copyToGPU();
    }

    void Shape::scale(const Vector2f& center, float factor) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Shape::Vertex::position))
            vertexPosition = (static_cast<Vector2f>(vertexPosition) - center) * factor + center;
        copyToGPU();
    }

    Shape::~Shape(void) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
