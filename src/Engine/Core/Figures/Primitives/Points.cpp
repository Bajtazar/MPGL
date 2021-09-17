#include "Points.hpp"
#include "../Views.hpp"
#include "../../../Utility/Ranges.hpp"

namespace ge {

    Points::Points(std::size_t vertices, const Color& color) noexcept : Shape{vertices} {
        for (auto& color_ : this->vertices | views::color)
            color_ = color;
    }

    Points::Points(std::size_t vertices) noexcept : Shape{vertices} {}

    Points::Points(const Points& points) noexcept : Shape{points.vertices.size()} {
        shaderProgram = points.shaderProgram;
        vertices.clear();
        vertices.reserve(points.vertices.size());
        std::ranges::copy(points, std::back_inserter(vertices));
    }

    Points& Points::operator= (const Points& points) noexcept {
        shaderProgram = points.shaderProgram;
        vertices.clear();
        vertices.reserve(points.vertices.size());
        std::ranges::copy(points, std::back_inserter(vertices));
        return *this;
    }

    Points::Points(Points&& points) noexcept : Shape{std::move(points.vertices)} {
        vertexArrayObject = points.vertexArrayObject;
        vertexBuffer = points.vertexBuffer;
        shaderProgram = points.shaderProgram;
        points.vertexArrayObject = 0;
        points.vertexBuffer = 0;
    }

    Points& Points::operator= (Points&& points) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = points.vertexArrayObject;
        vertexBuffer = points.vertexBuffer;
        shaderProgram = points.shaderProgram;
        points.vertexArrayObject = 0;
        points.vertexBuffer = 0;
        return *this;
    }

    Vector2f Points::getCenter(void) const noexcept {
        return accumulate(*this | views::position, Vector2f{},
            [](auto&& x){ return std::forward<Vector2f>(x); }) / static_cast<float>(vertices.size());
    }

    void Points::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);    // maybe will be moved
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_POINTS, 0,  vertices.size());
        glBindVertexArray(0);
    }

}
