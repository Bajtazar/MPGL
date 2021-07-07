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

    Shape::~Shape(void) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
