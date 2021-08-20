#include "Sprite.hpp"
#include "Views.hpp"
#include "../../Mathematics/Systems.hpp"

#include <algorithm>

namespace ge {

    template <bool IsColorable>
    std::array<typename Sprite<IsColorable>::Vertex, 4> Sprite<IsColorable>::makeVertexArray(const std::shared_ptr<Vector2i>& scene, const Color& color) noexcept requires IsColorable {
        return {Vertex{{}, color, {0.f, 0.f}, scene}, Vertex{{}, color, {0.f, 1.f}, scene}, Vertex{{}, color, {1.f, 1.f}, scene}, Vertex{{}, color, {1.f, 0.f}, scene}};
    }

    template <bool IsColorable>
    const std::array<uint32_t, 6> Sprite<IsColorable>::indexes {0, 1, 2, 0, 3, 2};

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture<>& texture) noexcept : Drawable{scene}, vertices{std::move(makeVertexArray(scene))}, texture{texture} {
        generateBuffers();
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture<>& texture, const Color& color) noexcept requires (IsColorable)
        : Drawable{scene}, vertices{std::move(makeVertexArray(scene, color))}, texture{texture}
    {
        generateBuffers();
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture<>& texture, const Vector2f& firstVertex, const Vector2f& secondVertex, const Vector2f& thirdVertex) noexcept : Sprite{scene, texture} {
        setVerticesPoisition(firstVertex, secondVertex, thirdVertex);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture<>& texture, const Vector2f& firstVertex, const Vector2f& secondVertex, const Vector2f& thirdVertex, const Color& color) noexcept
        requires (IsColorable) : Sprite{scene, texture, color}
    {
        setVerticesPoisition(firstVertex, secondVertex, thirdVertex);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture<>& texture, const Vector2f& firstVertex, const Vector2f& dimmensions) noexcept : Sprite{scene, texture} {
        setVerticesPoisition(firstVertex, dimmensions);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture<>& texture, const Vector2f& firstVertex, const Vector2f& dimmensions, const Color& color) noexcept
        requires (IsColorable) : Sprite{scene, texture, color}
    {
        setVerticesPoisition(firstVertex, dimmensions);
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(const Sprite& sprite) noexcept : Sprite{sprite.scene, sprite.texture} {
        shaderProgram = sprite.shaderProgram;
        std::ranges::copy(sprite, begin());
    }

    template <bool IsColorable>
    Sprite<IsColorable>::Sprite(Sprite&& sprite) noexcept : Drawable{std::move(sprite.scene)}, vertices{std::move(sprite.vertices)}, texture{sprite.texture} {
        vertexArrayObject = sprite.vertexArrayObject;
        vertexBuffer = sprite.vertexBuffer;
        elementArrayBuffer = sprite.elementArrayBuffer;
        shaderProgram = sprite.shaderProgram;
        sprite.vertexArrayObject = 0;
        sprite.vertexBuffer = 0;
        sprite.elementArrayBuffer = 0;
    }

    template <bool IsColorable>
    Sprite<IsColorable>& Sprite<IsColorable>::operator= (const Sprite& sprite) noexcept {
        shaderProgram = sprite.shaderProgram;
        std::ranges::copy(sprite, begin());
        return *this;
    }

    template <bool IsColorable>
    Sprite<IsColorable>& Sprite<IsColorable>::operator= (Sprite&& sprite) noexcept {
        this->~Sprite();
        vertexArrayObject = sprite.vertexArrayObject;
        vertexBuffer = sprite.vertexBuffer;
        shaderProgram = sprite.shaderProgram;
        elementArrayBuffer = sprite.elementArrayBuffer;
        sprite.vertexArrayObject = 0;
        sprite.vertexBuffer = 0;
        sprite.elementArrayBuffer = 0;
        return *this;
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setShaders(const ShaderLibrary& library) noexcept {
        if constexpr (IsColorable)
            shaderProgram = library["2DCTexture"];
        else
            shaderProgram = library["2DTexture"];

        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::copyToGPU(void) noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if constexpr (IsColorable) {
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(4 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getTexture());
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::onScreenTransformation(const Vector2i& oldDimmensions) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position)) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(oldDimmensions) / static_cast<Vector2f>(*scene) - 1.f;
        }
        copyToGPU();
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::translate(const Vector2f& shift) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position))
            vertexPosition = Vector2f(vertexPosition) + shift;
        copyToGPU();
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::scale(const Vector2f& center, float factor) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position))
            vertexPosition = (static_cast<Vector2f>(vertexPosition) - center) * factor + center;
        copyToGPU();
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::rotate(const Vector2f& center, float angle) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position)) {
            Vector2f position = vertexPosition;
            Vector2f radius = position - center;
            vertexPosition = rotationMatrix<float>(angle) * radius + center;
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
    std::array<typename Sprite<IsColorable>::Vertex, 4> Sprite<IsColorable>::makeVertexArray(const std::shared_ptr<Vector2i>& scene) noexcept {
        if constexpr (IsColorable)
            return {Vertex{{}, {}, {0.f, 0.f}, scene}, Vertex{{}, {}, {0.f, 1.f}, scene}, Vertex{{}, {}, {1.f, 1.f}, scene}, Vertex{{}, {}, {1.f, 0.f}, scene}};
        else
            return {Vertex{{}, {0.f, 0.f}, scene}, Vertex{{}, {0.f, 1.f}, scene}, Vertex{{}, {1.f, 1.f}, scene}, Vertex{{}, {1.f, 0.f}, scene}};
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::replaceTexture(const Texture<>& texture) {
        this->texture = texture;
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setVerticesPoisition(const Vector2f& firstVertex, const Vector2f& dimmensions) noexcept {
        vertices[0].position = firstVertex;
        vertices[1].position = firstVertex + Vector2f{0.f, dimmensions[1]};
        vertices[2].position = firstVertex + dimmensions;
        vertices[3].position = firstVertex + Vector2f{dimmensions[0], 0.f};
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::setVerticesPoisition(const Vector2f& firstVertex, const Vector2f& secondVertex, const Vector2f& thirdVertex) noexcept {
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[2].position = thirdVertex;
        vertices[3].position = firstVertex - secondVertex + thirdVertex;
    }

    template <bool IsColorable>
    void Sprite<IsColorable>::generateBuffers(void) noexcept {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementArrayBuffer);
    }

}
