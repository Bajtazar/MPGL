#include "LineStrip.hpp"

namespace ge {

    LineStrip::LineStrip(std::size_t vertices,
        Color const& color) : ResizableAngular{vertices, color} {}

    LineStrip::LineStrip(LineStrip const& lineStrip)
        : ResizableAngular{lineStrip.vertices,
            lineStrip.shaderProgram} {}

    LineStrip& LineStrip::operator=(LineStrip const& lineStrip) {
        ResizableAngular::operator=(lineStrip);
        return *this;
    }

    LineStrip::LineStrip(LineStrip&& lineStrip) noexcept
        : ResizableAngular{std::move(lineStrip.vertices),
            std::move(lineStrip.shaderProgram)}
    {
        moveAngular(std::move(lineStrip));
    }

    LineStrip& LineStrip::operator= (LineStrip&& lineStrip) noexcept
    {
        ResizableAngular::operator=(std::move(lineStrip));
        return *this;
    }

    void LineStrip::draw(void) const noexcept {
        shaderProgram->use();
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_LINE_STRIP, 0,  vertices.size());
        glBindVertexArray(0);
    }

}
