#include "Angular.hpp"
#include "../../Mathematics/Systems.hpp"

namespace ge {

    Angular::Angular(size_t size, Color const& color)
        : vertices{size, Vertex{{}, color}},
        Shadeable{"2DDefault"}
    {
        generateBuffers();
    }

    // move-only constructor
    Angular::Angular(Vertices vertices) noexcept
        : vertices{std::move(vertices)} {}

    Angular::Angular(Vertices vertices,
        std::string const& shader)
            : vertices{std::move(vertices)},
            Shadeable{shader}
    {
        generateBuffers();
    }

    Angular::Angular(Vertices vertices,
        ProgramPtr const& program)
            : vertices{std::move(vertices)},
            Shadeable{program}
    {
        generateBuffers();
    }

    Angular::Angular(Vertices vertices,
        ProgramPtr&& program) noexcept
            : vertices{std::move(vertices)},
            Shadeable{std::move(program)} {}

    void Angular::moveAngular(Angular&& shape) noexcept {
        vertexArrayObject = shape.vertexArrayObject;
        vertexBuffer = shape.vertexBuffer;
        shape.vertexArrayObject = shape.vertexBuffer = 0;
    }

    Angular::Angular(Angular&& shape) noexcept
        : Angular{std::move(shape.vertices),
            std::move(shape.shaderProgram)}
    {
        moveAngular(std::move(shape));
    }

    Angular& Angular::operator=(Angular&& shape) noexcept {
        this->~Angular();
        vertices = std::move(shape.vertices);
        shaderProgram = std::move(shape.shaderProgram);
        moveAngular(std::move(shape));
        return *this;
    }

    Angular& Angular::operator=(Angular const& shape) {
        vertices = shape.vertices;
        shaderProgram = shape.shaderProgram;
        return *this;
    }

    void Angular::generateBuffers(void) noexcept {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
    }

    void Angular::initialize(void) noexcept {
        generateBuffers();
        setShader("2DDefault");
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

    void Angular::onScreenTransformation(Vector2u const& oldDimmensions) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Angular::Vertex::position)) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * vectorCast<float32>(oldDimmensions)
                / vectorCast<float32>(context.windowDimmensions) - 1.f;
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

    Angular::~Angular(void) noexcept {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
