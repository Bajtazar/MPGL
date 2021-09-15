#include "Rectangle.hpp"
#include "../Views.hpp"

namespace ge {

    const std::array<uint32_t, 6> Rectangle::indexes {0, 1, 2, 0, 3, 2};

    Rectangle::Rectangle(const std::shared_ptr<Vector2ui>& scene) noexcept : Shape{scene, 4} {
        glGenBuffers(1, &elementArrayBuffer);
    }

    Rectangle::Rectangle(const std::shared_ptr<Vector2ui>& scene, const Vector2f& firstVertex, const Vector2f& dimmensions,
        const Color& color) noexcept : Rectangle{scene} {

        vertices[0].position = firstVertex;
        vertices[1].position = firstVertex + Vector2f{0.f, dimmensions[1]};
        vertices[2].position = firstVertex + dimmensions;
        vertices[3].position = firstVertex + Vector2f{dimmensions[0], 0.f};
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    Rectangle::Rectangle(const std::shared_ptr<Vector2ui>& scene, const Vector2f& firstVertex, const Vector2f& secondVertex,
        const Vector2f& thirdVertex, const Color& color) noexcept : Rectangle{scene} {

        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[2].position = thirdVertex;
        vertices[3].position = secondVertex - firstVertex + thirdVertex;
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    Rectangle::Rectangle(const Rectangle& rectangle) noexcept : Shape{rectangle.scene, 4} {
        glGenBuffers(1, &elementArrayBuffer);
        shaderProgram = rectangle.shaderProgram;
        std::ranges::copy(rectangle, begin());
    }

    Rectangle& Rectangle::operator= (const Rectangle& rectangle) noexcept {
        shaderProgram = rectangle.shaderProgram;
        std::ranges::copy(rectangle, begin());
        return *this;
    }

    Rectangle::Rectangle(Rectangle&& rectangle) noexcept : Shape{rectangle.scene, std::move(rectangle.vertices)} {
        vertexArrayObject = rectangle.vertexArrayObject;
        vertexBuffer = rectangle.vertexBuffer;
        elementArrayBuffer = rectangle.elementArrayBuffer;
        shaderProgram = rectangle.shaderProgram;
        rectangle.vertexArrayObject = 0;
        rectangle.vertexBuffer = 0;
        rectangle.elementArrayBuffer = 0;
    }

    Rectangle& Rectangle::operator= (Rectangle&& rectangle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(1, &elementArrayBuffer);
        vertexArrayObject = rectangle.vertexArrayObject;
        vertexBuffer = rectangle.vertexBuffer;
        shaderProgram = rectangle.shaderProgram;
        elementArrayBuffer = rectangle.elementArrayBuffer;
        rectangle.vertexArrayObject = 0;
        rectangle.vertexBuffer = 0;
        rectangle.elementArrayBuffer = 0;
        return *this;
    }

    void Rectangle::bindBuffers(void) const noexcept {
        Shape::bindBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);
    }

    void Rectangle::unbindBuffers(void) const noexcept {
        Shape::unbindBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Rectangle::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram); // maybe will be moved
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    Rectangle::~Rectangle(void) noexcept {
        glDeleteBuffers(1, &elementArrayBuffer);
    }
}
