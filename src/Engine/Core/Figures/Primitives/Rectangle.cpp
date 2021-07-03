#include "Rectangle.hpp"
#include "../Views.hpp"

namespace ge {

    constexpr std::size_t VertexSizeRectangle = 24 * sizeof(float);

    const std::array<uint32_t, 6> Rectangle::indexes {0, 1, 2, 0, 3, 2};

    Rectangle::Rectangle(const Vector2i& scene) noexcept : Shape{scene, 4} {
        glGenBuffers(1, &elementArrayBuffer);
    }

    Rectangle::Rectangle(const Vector2i& scene, const Vector2f& firstVertex, const Vector2f& dimmensions,
        const Color& color) noexcept : Rectangle{scene} {

        vertices[0].position = firstVertex;
        vertices[1].position = firstVertex + Vector2f{0.f, dimmensions[1]};
        vertices[2].position = firstVertex + dimmensions;
        vertices[3].position = firstVertex + Vector2f{dimmensions[0], 0.f};
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    Rectangle::Rectangle(const Vector2i& scene, const Vector2f& firstVertex, const Vector2f& secondVertex,
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
        copyToGPU();
        return *this;
    }

    Rectangle::Rectangle(Rectangle&& rectangle) noexcept : Shape{scene, std::move(rectangle.vertices)} {
        vertexArrayObject = rectangle.vertexArrayObject;
        vertexBuffer = rectangle.vertexBuffer;
        elementArrayBuffer = rectangle.elementArrayBuffer;
        shaderProgram = rectangle.shaderProgram;
    }

    Rectangle& Rectangle::operator= (Rectangle&& rectangle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(1, &elementArrayBuffer);
        vertexArrayObject = rectangle.vertexArrayObject;
        vertexBuffer = rectangle.vertexBuffer;
        shaderProgram = rectangle.shaderProgram;
        elementArrayBuffer = rectangle.elementArrayBuffer;
        return *this;
    }

    void Rectangle::copyToGPU(void) noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, VertexSizeRectangle, vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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
