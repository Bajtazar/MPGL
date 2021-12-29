#include "Tetragon.hpp"
#include "../Views.hpp"

namespace ge {

    void Tetragon::generateBuffers(void) noexcept {
        glGenBuffers(1, &elementArrayBuffer);
    }

    Tetragon::Tetragon(Color const& color) : Shape{4, color} {
        generateBuffers();
    }

    Tetragon::Tetragon(Vector2f const& firstVertex,
        Vector2f const& dimmensions,
        Color const& color)
            : Shape{Vertices{
                Vertex{firstVertex, color},
                Vertex{firstVertex + 1._y * dimmensions[1], color},
                Vertex{firstVertex + dimmensions, color},
                Vertex{firstVertex + 1._x * dimmensions[0], color}
            }, "2DDefault"}
    {
        generateBuffers();
    }

    Tetragon::Tetragon(Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color)
            : Shape{Vertices{
                Vertex{firstVertex, color},
                Vertex{secondVertex, color},
                Vertex{thirdVertex, color},
                Vertex{secondVertex - firstVertex
                    + thirdVertex, color}
            }, "2DDefault"}
    {
        generateBuffers();
    }

    Tetragon::Tetragon(Tetragon const& tetragon)
        : Shape{tetragon.vertices, tetragon.shaderProgram}
    {
        generateBuffers();
    }

    Tetragon& Tetragon::operator= (Tetragon const& tetragon) {
        Shape::operator=(tetragon);
        return *this;
    }

    void Tetragon::moveTetragon(Tetragon& tetragon) noexcept {
        elementArrayBuffer = tetragon.elementArrayBuffer;
        tetragon.elementArrayBuffer = 0;
    }

    Tetragon::Tetragon(Tetragon&& tetragon) noexcept
        : Shape{std::move(tetragon.vertices),
            std::move(tetragon.shaderProgram)}
    {
        moveTetragon(tetragon);
        moveShape(std::move(tetragon));
    }

    Tetragon& Tetragon::operator= (Tetragon&& rectangle) noexcept {
        deleteBuffers();
        moveTetragon(rectangle);
        Shape::operator=(std::move(rectangle));
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
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Tetragon::deleteBuffers(void) noexcept {
        glDeleteBuffers(1, &elementArrayBuffer);
    }

    Tetragon::~Tetragon(void) noexcept {
        deleteBuffers();
    }
}
