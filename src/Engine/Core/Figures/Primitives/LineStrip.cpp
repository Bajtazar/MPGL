#include "LineStrip.hpp"
#include "../Views.hpp"
#include "../../../Utility/Ranges.hpp"

namespace ge {

    LineStrip::LineStrip(const Vector2i& scene, std::size_t vertices, const Color& color) noexcept : Shape{scene, vertices} {
        for (auto& color_ : this->vertices | ge::views::color)
            color_ = color;
    }

    LineStrip::LineStrip(const Vector2i& scene, std::size_t vertices) noexcept : Shape{scene, vertices} {}

    LineStrip::LineStrip(const LineStrip& lineStrip) noexcept : Shape{lineStrip.scene, lineStrip.vertices.size()} {
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
        copyToGPU();
        return *this;
    }

    LineStrip::LineStrip(LineStrip&& lineStrip) noexcept : Shape{lineStrip.scene, std::move(lineStrip.vertices)} {
        vertexArrayObject = lineStrip.vertexArrayObject;
        vertexBuffer = lineStrip.vertexBuffer;
        shaderProgram = lineStrip.shaderProgram;
    }

    LineStrip& LineStrip::operator= (LineStrip&& lineStrip) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = lineStrip.vertexArrayObject;
        vertexBuffer = lineStrip.vertexBuffer;
        shaderProgram = lineStrip.shaderProgram;
        return *this;
    }

    Vector2f LineStrip::getCenter(void) const noexcept {
        return accumulate(*this | views::position, Vector2f{},
            [](auto&& x){ return std::forward<Vector2f>(x); }) / static_cast<float>(vertices.size());
    }

    void LineStrip::copyToGPU(void) noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void LineStrip::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);    // maybe will be moved
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_LINE_STRIP, 0,  vertices.size());
        glBindVertexArray(0);
    }

}
