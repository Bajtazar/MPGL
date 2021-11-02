#include "Ellipse.hpp"
#include "../../../Mathematics/Systems.hpp"

namespace ge {

    Ellipse::Vertices Ellipse::ellipseVertices(Vector2f const& center,
        Vector2f const& semiAxis, float angle) noexcept
    {
        Matrix2f rotation = rotationMatrix<float>(angle);
        Vector2f rot1 = rotation * semiAxis;
        Vector2f rot2 = rotation * Vector2f{semiAxis[0], -semiAxis[1]};
        return {
            center - rot2,
            center + rot1,
            center + rot2,
            center - rot1
        };
    }

    Ellipse::Vertices Ellipse::circleVertices(Vector2f const& center,
        float radius) noexcept
    {
        Vector2f semiMajor = Vector2f{radius, 0.f};
        Vector2f semiMinor = Vector2f{0.f, radius};
        return {
            center - semiMajor + semiMinor,
            center + semiMajor + semiMinor,
            center + semiMajor - semiMinor,
            center - semiMajor - semiMinor
        };
    }

    void Ellipse::generateBuffers(void) noexcept {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementArrayBuffer);
    }

    Ellipse::Ellipse(Vector2f const& center, Vector2f const& semiAxis,
        float angle, Color const& color) noexcept
            : vertices{ellipseVertices(center, semiAxis, angle)},
            color{color}
    {
        generateBuffers();
        recalculateUniforms();
    }

    Ellipse::Ellipse(Vector2f const& center, float radius,
        Color const& color) noexcept
            : vertices{circleVertices(center, radius)}, color{color}
    {
        generateBuffers();
        recalculateUniforms();
    }

    Ellipse::Ellipse(Ellipse const& ellipse) noexcept
        : vertices{ellipse.vertices}, transform{ellipse.transform},
        color{ellipse.color},
        shaderProgram{ellipse.shaderProgram}
    {
        generateBuffers();
    }

    Ellipse::Ellipse(Ellipse&& ellipse) noexcept
        : vertices{std::move(ellipse.vertices)}, transform{ellipse.transform},
        color{ellipse.color}, shaderProgram{std::move(ellipse.shaderProgram)},
        vertexBuffer{ellipse.vertexBuffer}, vertexArrayObject{
        ellipse.vertexArrayObject}, elementArrayBuffer{
        ellipse.elementArrayBuffer}
    {
        ellipse.vertexBuffer = 0;
        ellipse.vertexArrayObject = 0;
        ellipse.elementArrayBuffer = 0;
    }

    Ellipse& Ellipse::operator= (Ellipse const& ellipse) noexcept {
        std::ranges::copy(ellipse.vertices, vertices.begin());
        shaderProgram = ellipse.shaderProgram;
        transform = ellipse.transform;
        color = ellipse.color;
        return *this;
    }

    Ellipse& Ellipse::operator= (Ellipse&& ellipse) noexcept {
        this->~Ellipse();
        vertexArrayObject = ellipse.vertexArrayObject;
        vertexBuffer = ellipse.vertexBuffer;
        elementArrayBuffer = ellipse.elementArrayBuffer;
        shaderProgram = std::move(ellipse.shaderProgram);
        transform = ellipse.transform;
        color = ellipse.color;
        ellipse.vertexArrayObject = 0;
        ellipse.vertexBuffer = 0;
        ellipse.elementArrayBuffer = 0;
        return *this;
    }

    void Ellipse::setShaders(ShaderLibrary const& library) noexcept {
        shaderProgram = library["2DEllipse"];
    }

    void Ellipse::recalculateUniforms(void) noexcept {
        transform = *Matrix2f{Vector2f{vertices[1]} - Vector2f{vertices[0]},
            Vector2f{vertices[3]} - Vector2f{vertices[0]}}.transpose().inverse();
    }

    void Ellipse::bindBuffers(void) const noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Adapter<Vector2f>) * 4,
            vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t),
            indexes.data(), GL_STATIC_DRAW);
    }

    void Ellipse::copyBuffersToGPU(void) const noexcept {
        auto size = sizeof(Adapter<Vector2f>);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, size, nullptr);
        glEnableVertexAttribArray(0);
    }

    void Ellipse::unbindBuffers(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Ellipse::copyToGPU(void) const noexcept {
        bindBuffers();
        copyBuffersToGPU();
        unbindBuffers();
    }

    void Ellipse::onScreenTransformation(Vector2ui const& oldDimmensions) noexcept {
        for (auto& vertexPosition : vertices) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(oldDimmensions)
                / static_cast<Vector2f>(context.windowDimmensions) - 1.f;
        }
        copyToGPU();
        recalculateUniforms();
    }

    void Ellipse::translate(Vector2f const& shift) noexcept {
        for (auto& vertexPosition : vertices)
            vertexPosition = Vector2f(vertexPosition) + shift;
        copyToGPU();
        recalculateUniforms();
    }

    void Ellipse::scale(Vector2f const& center, float factor) noexcept {
        for (auto& vertexPosition : vertices)
            vertexPosition = (static_cast<Vector2f>(vertexPosition) - center) * factor + center;
        copyToGPU();
        recalculateUniforms();
    }

    void Ellipse::rotate(Vector2f const& center, float angle) noexcept {
        rotate(center, rotationMatrix<float>(angle));
    }

    void Ellipse::rotate(Vector2f const& center, Matrix2f const& rotation) noexcept {
        for (auto& vertexPosition : vertices) {
            Vector2f position = vertexPosition;
            Vector2f radius = position - center;
            vertexPosition = rotation * radius + center;
        }
        copyToGPU();
        recalculateUniforms();
    }

    void Ellipse::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shaderProgram.use();
        shaderProgram.setUniform("color", color);
        shaderProgram.setUniform("shift", Vector2f{vertices.front()});
        shaderProgram.setUniform("transform", transform);
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Ellipse::~Ellipse(void) noexcept {
        glDeleteBuffers(1, &elementArrayBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
