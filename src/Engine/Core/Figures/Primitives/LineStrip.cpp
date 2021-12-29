#include "LineStrip.hpp"

namespace ge {

    LineStrip::LineStrip(std::size_t vertices,
        Color const& color) : ResizableShape{vertices, color} {}

    LineStrip::LineStrip(LineStrip const& lineStrip)
        : ResizableShape{lineStrip.vertices,
            lineStrip.shaderProgram} {}

    LineStrip& LineStrip::operator=(LineStrip const& lineStrip) {
        ResizableShape::operator=(lineStrip);
        return *this;
    }

    LineStrip::LineStrip(LineStrip&& lineStrip) noexcept
        : ResizableShape{std::move(lineStrip.vertices),
            std::move(lineStrip.shaderProgram)}
    {
        moveShape(std::move(lineStrip));
    }

    LineStrip& LineStrip::operator= (LineStrip&& lineStrip) noexcept
    {
        ResizableShape::operator=(std::move(lineStrip));
        return *this;
    }

    void LineStrip::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_LINE_STRIP, 0,  vertices.size());
        glBindVertexArray(0);
    }

}
