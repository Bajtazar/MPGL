#include "LineStrip.hpp"
#include "../Views.hpp"
#include "../../../Utility/Ranges.hpp"

namespace ge {

    LineStrip::LineStrip(std::size_t vertices, const Color& color) noexcept : Shape{vertices} {
        for (auto& color_ : this->vertices | views::color)
            color_ = color;
    }

    LineStrip::LineStrip(std::size_t vertices) noexcept : Shape{vertices} {}

    LineStrip::LineStrip(const LineStrip& lineStrip) noexcept : Shape{lineStrip.vertices.size()} {
        shaderProgram = lineStrip.shaderProgram;
        vertices.clear();
        vertices.reserve(lineStrip.vertices.size());
        std::ranges::copy(lineStrip, std::back_inserter(vertices));
    }

    LineStrip& LineStrip::operator=(const LineStrip& lineStrip) noexcept {
        shaderProgram = lineStrip.shaderProgram;
        vertices.clear();
        vertices.reserve(lineStrip.vertices.size());
        std::ranges::copy(lineStrip, std::back_inserter(vertices));
        return *this;
    }

    LineStrip::LineStrip(LineStrip&& lineStrip) noexcept : Shape{std::move(lineStrip.vertices)} {
        vertexArrayObject = lineStrip.vertexArrayObject;
        vertexBuffer = lineStrip.vertexBuffer;
        shaderProgram = lineStrip.shaderProgram;
        lineStrip.vertexArrayObject = 0;
        lineStrip.vertexBuffer = 0;
    }

    LineStrip& LineStrip::operator= (LineStrip&& lineStrip) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = lineStrip.vertexArrayObject;
        vertexBuffer = lineStrip.vertexBuffer;
        shaderProgram = lineStrip.shaderProgram;
        lineStrip.vertexArrayObject = 0;
        lineStrip.vertexBuffer = 0;
        return *this;
    }

    Vector2f LineStrip::getCenter(void) const noexcept {
        return accumulate(*this | views::position, Vector2f{},
            [](auto&& x){ return std::forward<Vector2f>(x); }) / static_cast<float>(vertices.size());
    }

    void LineStrip::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);    // maybe will be moved
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_LINE_STRIP, 0,  vertices.size());
        glBindVertexArray(0);
    }

}
