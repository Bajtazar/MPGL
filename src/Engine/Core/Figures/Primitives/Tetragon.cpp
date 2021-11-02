#include "Tetragon.hpp"
#include "../Views.hpp"

namespace ge {

    Tetragon::Tetragon(Color const& color) noexcept : Shape{4} {
        glGenBuffers(1, &elementArrayBuffer);
        for (auto& color_ : vertices | ge::views::color)
            color_ = color;
    }

    Tetragon::Tetragon(Vector2f const& firstVertex, Vector2f const& dimmensions,
        Color const& color) noexcept
            : Tetragon{color}
    {
        vertices[0].position = firstVertex;
        vertices[1].position = firstVertex + Vector2f{0.f, dimmensions[1]};
        vertices[2].position = firstVertex + dimmensions;
        vertices[3].position = firstVertex + Vector2f{dimmensions[0], 0.f};
    }

    Tetragon::Tetragon(Vector2f const& firstVertex, Vector2f const& secondVertex,
        Vector2f const& thirdVertex, Color const& color) noexcept
            : Tetragon{color}
    {
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[2].position = thirdVertex;
        vertices[3].position = secondVertex - firstVertex + thirdVertex;
    }

    Tetragon::Tetragon(Tetragon const& rectangle) noexcept : Shape{4} {
        glGenBuffers(1, &elementArrayBuffer);
        shaderProgram = rectangle.shaderProgram;
        std::ranges::copy(rectangle, begin());
    }

    Tetragon& Tetragon::operator= (Tetragon const& rectangle) noexcept {
        shaderProgram = rectangle.shaderProgram;
        std::ranges::copy(rectangle, begin());
        return *this;
    }

    Tetragon::Tetragon(Tetragon&& rectangle) noexcept
        : Shape{std::move(rectangle.vertices)}
    {
        vertexArrayObject = rectangle.vertexArrayObject;
        vertexBuffer = rectangle.vertexBuffer;
        elementArrayBuffer = rectangle.elementArrayBuffer;
        shaderProgram = std::move(rectangle.shaderProgram);
        rectangle.vertexArrayObject = 0;
        rectangle.vertexBuffer = 0;
        rectangle.elementArrayBuffer = 0;
    }

    Tetragon& Tetragon::operator= (Tetragon&& rectangle) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(1, &elementArrayBuffer);
        vertexArrayObject = rectangle.vertexArrayObject;
        vertexBuffer = rectangle.vertexBuffer;
        shaderProgram = std::move(rectangle.shaderProgram);
        elementArrayBuffer = rectangle.elementArrayBuffer;
        rectangle.vertexArrayObject = 0;
        rectangle.vertexBuffer = 0;
        rectangle.elementArrayBuffer = 0;
        return *this;
    }

    void Tetragon::bindBuffers(void) const noexcept {
        Shape::bindBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);
    }

    void Tetragon::unbindBuffers(void) const noexcept {
        Shape::unbindBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Tetragon::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shaderProgram.use();
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    Tetragon::~Tetragon(void) noexcept {
        glDeleteBuffers(1, &elementArrayBuffer);
    }
}
