#include "Sprite.hpp"
#include "Views.hpp"
#include "../../Mathematics/Systems.hpp"

#include <algorithm>

namespace ge {

    template <bool IsColorable>
    Sprite<IsColorable>::VertexArray
        Sprite<IsColorable>::makeVertexArray(
            Color const& color) noexcept requires IsColorable
    {
        return {
            Vertex{{}, color, {0.f, 0.f}},
            Vertex{{}, color, {0.f, 1.f}},
            Vertex{{}, color, {1.f, 1.f}},
            Vertex{{}, color, {1.f, 0.f}}
        };
    }

    template <bool IsColorable>
    Sprite<IsColorable>::VertexArray
        Sprite<IsColorable>::makeVertexArray(void) noexcept
    {
        if constexpr (IsColorable)
            return {Vertex{{}, {}, {0.f, 0.f}},
                Vertex{{}, {}, {0.f, 1.f}},
                Vertex{{}, {}, {1.f, 1.f}},
                Vertex{{}, {}, {1.f, 0.f}}};
        else
            return {Vertex{{}, {0.f, 0.f}},
                Vertex{{}, {0.f, 1.f}},
                Vertex{{}, {1.f, 1.f}},
                Vertex{{}, {1.f, 0.f}}};
    }

    template <bool IsColorable>
    const Sprite<IsColorable>::Executable
        Sprite<IsColorable>::shaderExec = [](ProgramPtr& ptr) -> void
            { ptr->use(); ptr->setUniform("tex", 0); };

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Texture const& texture) noexcept
        : Shadeable{shaderType(), shaderExec},
        vertices{makeVertexArray()}, texture{texture}
    {
        generateBuffers();
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Texture const& texture,
        Color const& color) noexcept requires (IsColorable)
            : Shadeable{shaderType(), shaderExec},
            vertices{makeVertexArray(color)}, texture{texture}
    {
        generateBuffers();
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Texture const& texture,
        Vector2f const& firstVertex, Vector2f const& secondVertex,
        Vector2f const& thirdVertex) noexcept
            : Sprite{texture}
    {
        setVerticesPoisition(firstVertex, secondVertex, thirdVertex);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Texture const& texture,
        Vector2f const& firstVertex, Vector2f const& secondVertex,
        Vector2f const& thirdVertex,
        Color const& color) noexcept requires (IsColorable)
            : Sprite{texture, color}
    {
        setVerticesPoisition(firstVertex, secondVertex, thirdVertex);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Texture const& texture,
        Vector2f const& firstVertex,
        Vector2f const& dimmensions) noexcept
            : Sprite{texture}
    {
        setVerticesPoisition(firstVertex, dimmensions);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Texture const& texture,
        Vector2f const& firstVertex, Vector2f const& dimmensions,
        Color const& color) noexcept requires (IsColorable)
            : Sprite{texture, color}
    {
        setVerticesPoisition(firstVertex, dimmensions);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(
        Sprite const& sprite) noexcept : Sprite{sprite.texture}
    {
        shaderProgram = sprite.shaderProgram;
        std::ranges::copy(sprite, begin());
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Sprite&& sprite) noexcept
        : vertices{std::move(sprite.vertices)},
        texture{std::move(sprite.texture)}
    {
        vertexArrayObject = sprite.vertexArrayObject;
        vertexBuffer = sprite.vertexBuffer;
        elementArrayBuffer = sprite.elementArrayBuffer;
        shaderProgram = std::move(sprite.shaderProgram);
        sprite.vertexArrayObject = sprite.vertexBuffer =
            sprite.elementArrayBuffer = 0;
    }

    template <bool IsColorable>
    Sprite<IsColorable>& Sprite<IsColorable>::operator= (
        Sprite const& sprite) noexcept
    {
        shaderProgram = sprite.shaderProgram;
        std::ranges::copy(sprite, begin());
        return *this;
    }

    template <bool IsColorable>
    Sprite<IsColorable>& Sprite<IsColorable>::operator= (
        Sprite&& sprite) noexcept
    {
        this->~Sprite();
        texture = std::move(sprite.texture);
        vertexArrayObject = sprite.vertexArrayObject;
        vertexBuffer = sprite.vertexBuffer;
        shaderProgram = std::move(sprite.shaderProgram);
        elementArrayBuffer = sprite.elementArrayBuffer;
        sprite.vertexArrayObject = sprite.vertexBuffer =
            sprite.elementArrayBuffer = 0;
        return *this;
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::bindBuffers(void) const noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4,
            vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() *
            sizeof(uint32), indexes.data(), GL_STATIC_DRAW);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::copyBuffersToGPU(void) const noexcept {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            reinterpret_cast<void*>(2 * sizeof(float32)));
        glEnableVertexAttribArray(1);
        if constexpr (IsColorable) {
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                sizeof(Vertex),
                reinterpret_cast<void*>(4 * sizeof(float32)));
            glEnableVertexAttribArray(2);
        }
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::unbindBuffers(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::copyToGPU(void) const noexcept {
        bindBuffers();
        copyBuffersToGPU();
        unbindBuffers();
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::draw(void) const noexcept {
        shaderProgram->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getTexture());
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::onScreenTransformation(
        Vector2u const& oldDimmensions) noexcept
    {
        for (auto& vertexPosition : vertices |
            std::views::transform(&Vertex::position))
        {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * vectorCast<float32>(
                oldDimmensions) / vectorCast<float32>(
                    context.windowDimmensions) - 1.f;
        }
        copyToGPU();
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::translate(
        Vector2f const& shift) noexcept
    {
        for (auto& vertexPosition : vertices |
            std::views::transform(&Vertex::position))
                vertexPosition = Vector2f(vertexPosition) + shift;
        copyToGPU();
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::scale(Vector2f const& center,
        float32 factor) noexcept
    {
        for (auto& vertexPosition : vertices |
            std::views::transform(&Vertex::position))
        {
            vertexPosition = (Vector2f(vertexPosition) -
                center) * factor + center;
        }
        copyToGPU();
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::rotate(Vector2f const& center,
        float32 angle) noexcept
    {
        rotate(center, rotationMatrix<float32>(angle));
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::rotate(Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        for (auto& vertexPosition : vertices |
            std::views::transform(&Vertex::position))
        {
            Vector2f radius = Vector2f(vertexPosition) - center;
            vertexPosition = rotation * radius + center;
        }
        copyToGPU();
    }

    template <bool IsColorable>
    Sprite<IsColorable>::~Sprite(void) noexcept {
        glDeleteBuffers(1, &elementArrayBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::replaceTexture(
        Texture const& texture)
    {
        this->texture = texture;
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setVerticesPoisition(
        Vector2f const& firstVertex,
        Vector2f const& dimmensions) noexcept
    {
        vertices[0].position = firstVertex;
        vertices[1].position = firstVertex +
            Vector2f{0.f, dimmensions[1]};
        vertices[2].position = firstVertex + dimmensions;
        vertices[3].position = firstVertex +
            Vector2f{dimmensions[0], 0.f};
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setVerticesPoisition(
        Vector2f const& firstVertex,
        Vector2f const& secondVertex,
        Vector2f const& thirdVertex) noexcept
    {
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[2].position = thirdVertex +
            secondVertex - thirdVertex;
        vertices[3].position = firstVertex;
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::generateBuffers(void) noexcept {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementArrayBuffer);
    }

}
