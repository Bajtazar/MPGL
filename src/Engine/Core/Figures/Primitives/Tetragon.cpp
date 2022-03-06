#include "Tetragon.hpp"

#include "../../Context/Buffers/BindGuard.hpp"

#include <iostream>

namespace mpgl {

    Tetragon::Tetragon(Color const& color) : Angular{4, color} {
        initElementBuffer();
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
        initElementBuffer();
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
        initElementBuffer();
    }

    Tetragon::Tetragon(Tetragon const& tetragon)
        : Angular{tetragon}
    {
        initElementBuffer();
    }

    void Tetragon::initElementBuffer(void) const noexcept {
        BindGuard<VertexArray> vaoGuard{vertexArray};
        elementBuffer.bind();
        elementBuffer.setBufferData(indexes);
    }

    void Tetragon::draw(void) const noexcept {
        actualizeBufferBeforeDraw();
        shaderProgram->use();
        BindGuard<VertexArray> vaoGuard{vertexArray};
        vertexArray.drawElements(VertexArray::DrawMode::Triangles,
            6, DataType::UInt32);
    }

}
