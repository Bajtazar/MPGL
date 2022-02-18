#include "Tetragon.hpp"
#include "../Views.hpp"

namespace mpgl {

    void Tetragon::generateBuffers(void) noexcept {
        glGenBuffers(1, &elementArrayBuffer);
    }

    Tetragon::Tetragon(Color const& color) : Angular{4, color} {
        generateBuffers();
    }

    Tetragon::Tetragon(Vector2f const& firstVertex,
        Vector2f const& dimensions,
        Color const& color)
            : Angular{Vertices{
                Vertex{firstVertex, color},
                Vertex{firstVertex + 1._y * dimensions[1], color},
                Vertex{firstVertex + dimensions, color},
                Vertex{firstVertex + 1._x * dimensions[0], color}
            }}
    {
        generateBuffers();
    }

    Tetragon::Tetragon(Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color)
            : Angular{Vertices{
                Vertex{firstVertex, color},
                Vertex{secondVertex, color},
                Vertex{thirdVertex, color},
                Vertex{secondVertex - firstVertex
                    + thirdVertex, color}
            }}
    {
        generateBuffers();
    }

    Tetragon::Tetragon(Tetragon const& tetragon)
        : Angular{tetragon}
    {
        generateBuffers();
    }

    Tetragon& Tetragon::operator= (Tetragon const& tetragon) {
        Angular::operator=(tetragon);
        return *this;
    }

    void Tetragon::moveTetragon(Tetragon& tetragon) noexcept {
        elementArrayBuffer = tetragon.elementArrayBuffer;
        tetragon.elementArrayBuffer = 0;
    }

    Tetragon::Tetragon(Tetragon&& tetragon) noexcept
        : Angular{std::move(tetragon)}
    {
        moveTetragon(tetragon);
    }

    Tetragon& Tetragon::operator= (Tetragon&& rectangle) noexcept {
        moveTetragon(rectangle);
        Angular::operator=(std::move(rectangle));
        return *this;
    }

    void Tetragon::bindBuffers(void) const noexcept {
        Angular::bindBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indexes.size() * sizeof(uint32), indexes.data(), GL_STATIC_DRAW);
    }

    void Tetragon::unbindBuffers(void) const noexcept {
        Angular::unbindBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Tetragon::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    Tetragon::~Tetragon(void) noexcept {
        glDeleteBuffers(1, &elementArrayBuffer);
    }
}
