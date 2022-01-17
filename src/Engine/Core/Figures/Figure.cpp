#include "Figure.hpp"

namespace ge {

    void Figure::init(void) noexcept {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
    }

    Figure::Figure(void) noexcept {
        init();
    }

    Figure::Figure(std::string const& programName)
        : Shadeable{programName}
    {
        init();
    }

    Figure::Figure(std::string const& programName,
        Executable exec) : Shadeable{programName, exec}
    {
        init();
    }

    Figure::Figure(Figure const& shape)
        : Shadeable{shape.shaderProgram}
    {
        init();
    }

    Figure::Figure(Figure&& shape) noexcept
        : Shadeable{std::move(shape.shaderProgram)},
        vertexBuffer{shape.vertexBuffer},
        vertexArrayObject{shape.vertexArrayObject}
    {
        shape.vertexArrayObject = shape.vertexBuffer = 0;
    }

    Figure& Figure::operator=(Figure&& shape) noexcept {
        this->~Figure();
        shaderProgram = std::move(shape.shaderProgram);
        vertexBuffer = shape.vertexBuffer;
        vertexArrayObject = shape.vertexArrayObject;
        shape.vertexArrayObject = shape.vertexBuffer = 0;
        return *this;
    }

    Figure& Figure::operator=(Figure const& shape) {
        shaderProgram = shape.shaderProgram;;
        return *this;
    }

    Figure::~Figure(void) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
