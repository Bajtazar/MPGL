#include "Shape.hpp"

namespace mpgl {

    Shape::Shape(void) noexcept {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
    }

    Shape::Shape(Shape&& shape) noexcept
        : vertexArrayObject{shape.vertexArrayObject},
        vertexBuffer{shape.vertexBuffer}
    {
        shape.vertexArrayObject = shape.vertexBuffer = 0;
    }

    Shape& Shape::operator=(Shape&& shape) noexcept {
        this->~Shape();
        vertexBuffer = shape.vertexBuffer;
        vertexArrayObject = shape.vertexArrayObject;
        shape.vertexArrayObject = shape.vertexBuffer = 0;
        return *this;
    }

    Shape::~Shape(void) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
