#include "Angular.hpp"
#include "../../Mathematics/Systems.hpp"

namespace ge {

    Angular::Angular(size_t size, Color const& color)
        : Figure{"2DDefault"}, vertices{size, Vertex{{}, color}} {}

    Angular::Angular(Vertices vertices) : Figure{"2DDefault"},
        vertices{std::move(vertices)} {}

    Angular::Angular(Angular&& shape) noexcept
        : Figure{std::move(shape)}, vertices{
            std::move(shape.vertices)} {}

    Angular::Angular(Angular const& shape)
        : Figure{shape}, vertices{shape.vertices} {}

    Angular& Angular::operator=(Angular&& shape) noexcept {
        Figure::operator=(std::move(shape));
        vertices = std::move(shape.vertices);
        return *this;
    }

    Angular& Angular::operator=(Angular const& shape) {
        Figure::operator=(shape);
        vertices = shape.vertices;
        return *this;
    }

    void Angular::copyToGPU(void) const noexcept {
        bindBuffers();
        copyBuffersToGPU();
        unbindBuffers();
    }

    void Angular::bindBuffers(void) const noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float32) * vertices.size(),
            vertices.data(), GL_STATIC_DRAW);
    }

    void Angular::copyBuffersToGPU(void) const noexcept {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            reinterpret_cast<void*>(2 * sizeof(float32)));
        glEnableVertexAttribArray(1);
    }

    void Angular::unbindBuffers(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Angular::onScreenTransformation(Vector2u const& oldDimensions) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Angular::Vertex::position)) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * vectorCast<float32>(oldDimensions)
                / vectorCast<float32>(context.windowDimensions) - 1.f;
        }
        copyToGPU();
    }

    void Angular::translate(Vector2f const& shift) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Angular::Vertex::position))
            vertexPosition = static_cast<Vector2f>(vertexPosition) + shift;
        copyToGPU();
    }

    void Angular::rotate(Vector2f const& center, float32 angle) noexcept {
        rotate(center, rotationMatrix<float32>(angle));
    }

    void Angular::rotate(Vector2f const& center, Matrix2f const& rotation) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Angular::Vertex::position)) {
            Vector2f position = vertexPosition;
            Vector2f radius = position - center;
            vertexPosition = rotation * radius + center;
        }
        copyToGPU();
    }

    void Angular::scale(Vector2f const& center, float32 factor) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Angular::Vertex::position))
            vertexPosition = (static_cast<Vector2f>(vertexPosition) - center) * factor + center;
        copyToGPU();
    }

}
