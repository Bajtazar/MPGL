#include "GlyphSprite.hpp"

namespace ge {

    template <bool IsMonochromatic>
        GlyphSprite<IsMonochromatic>::Vertices
    GlyphSprite<IsMonochromatic>::makeVertexArray(Color const& color) noexcept
    {
        if constexpr (IsMonochromatic) {
            GlyphBase<IsMonochromatic>::color = color;
            return {Vertex{{}, {0.f, 0.f}}, Vertex{{}, {0.f, 1.f}},
                Vertex{{}, {1.f, 1.f}}, Vertex{{}, {1.f, 0.f}}};
        } else
            return {Vertex{{}, color, {0.f, 0.f}}, Vertex{{}, color, {0.f, 1.f}},
                Vertex{{}, color, {1.f, 1.f}}, Vertex{{}, color, {1.f, 0.f}}};
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>::GlyphSprite(GlyphTexture const& texture,
        Color const& color)
        : vertices{std::move(makeVertexArray(color))}, texture{texture}
    {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementArrayBuffer);
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>::GlyphSprite(GlyphTexture const& texture,
        Vector2f const& firstVertex, Vector2f const& secondVertex,
        Vector2f const& thirdVertex,Color const& color)
        : GlyphSprite{texture, color}
    {
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[2].position = thirdVertex + secondVertex - firstVertex;
        vertices[3].position = thirdVertex;
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>::GlyphSprite(GlyphTexture const& texture,
        Vector2f const& firstVertex, Vector2f const& dimmensions,
        Color const& color)
        : GlyphSprite{texture, color}
    {
        vertices[0].position = firstVertex;
        vertices[1].position = firstVertex + Vector2f{0.f, dimmensions[1]};
        vertices[2].position = firstVertex + dimmensions;
        vertices[3].position = firstVertex + Vector2f{dimmensions[0], 0.f};
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>::GlyphSprite(GlyphSprite const& sprite) noexcept
        :  GlyphSprite{sprite.texture}
    {
        shaderProgram = sprite.shaderProgram;
        std::ranges::copy(sprite, begin());
        if constexpr (IsMonochromatic)
            GlyphBase<IsMonochromatic>::color = sprite.color;
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>::GlyphSprite(GlyphSprite&& sprite) noexcept
        : vertices{std::move(sprite.vertices)},
        texture{sprite.texture}
    {
        vertexArrayObject = sprite.vertexArrayObject;
        vertexBuffer = sprite.vertexBuffer;
        elementArrayBuffer = sprite.elementArrayBuffer;
        shaderProgram = std::move(sprite.shaderProgram);
        sprite.vertexArrayObject = 0;
        sprite.vertexBuffer = 0;
        sprite.elementArrayBuffer = 0;
        if constexpr (IsMonochromatic)
            GlyphBase<IsMonochromatic>::color = sprite.color;
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>& GlyphSprite<IsMonochromatic>::operator=(
        GlyphSprite const& sprite) noexcept
    {
        shaderProgram = sprite.shaderProgram;
        std::ranges::copy(sprite, begin());
        if constexpr (IsMonochromatic)
            GlyphBase<IsMonochromatic>::color = sprite.color;
        return *this;
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>& GlyphSprite<IsMonochromatic>::operator=(
        GlyphSprite&& sprite) noexcept
    {
        this->~GlyphSprite();
        vertexArrayObject = sprite.vertexArrayObject;
        vertexBuffer = sprite.vertexBuffer;
        shaderProgram = std::move(sprite.shaderProgram);
        elementArrayBuffer = sprite.elementArrayBuffer;
        sprite.vertexArrayObject = 0;
        sprite.vertexBuffer = 0;
        sprite.elementArrayBuffer = 0;
        if constexpr (IsMonochromatic)
            GlyphBase<IsMonochromatic>::color = sprite.color;
        return *this;
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::setColor(Color const& color) noexcept
    {
        if constexpr (IsMonochromatic) {
            GlyphBase<IsMonochromatic>::color = color;
            if (shaderProgram)
                shaderProgram.setUniform("color", color);
        } else {
            std::ranges::for_each(vertices, [&color](auto& vertex){ vertex.color = color; });
            copyToGPU();
        }
    }


    template <bool IsMonochromatic>
    Color const& GlyphSprite<IsMonochromatic>::getColor(void) const noexcept
        requires IsMonochromatic
    {
        return GlyphBase<IsMonochromatic>::color;
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::setShaders(ShaderLibrary const& library) noexcept {
        if constexpr (IsMonochromatic)
            shaderProgram = library["2DMonoGlyph"];
        else
            shaderProgram = library["2DPoliGlyph"];
        shaderProgram.use();
        shaderProgram.setUniform("tex", 0);
        if constexpr (IsMonochromatic)
            shaderProgram.setUniform("color", GlyphBase<IsMonochromatic>::color);
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::bindBuffers(void) const noexcept {
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(uint32_t), indexes.data(), GL_STATIC_DRAW);
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::copyBuffersToGPU(void) const noexcept {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if constexpr (!IsMonochromatic) {
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(4 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::unbindBuffers(void) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::copyToGPU(void) const noexcept {
        bindBuffers();
        copyBuffersToGPU();
        unbindBuffers();
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::draw(void) const noexcept {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shaderProgram.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getTexture());
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::onScreenTransformation(
        Vector2ui const& oldDimmensions) noexcept
    {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position)) {
            Vector2f& position = vertexPosition.get();
            position = (position + 1.f) * static_cast<Vector2f>(oldDimmensions)
                / static_cast<Vector2f>(context.windowDimmensions) - 1.f;
        }
        copyToGPU();
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::translate(
        Vector2f const& shift) noexcept
    {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position))
            vertexPosition = Vector2f(vertexPosition) + shift;
        copyToGPU();
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::scale(Vector2f const& center, float factor) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position))
            vertexPosition = (static_cast<Vector2f>(vertexPosition) - center) * factor + center;
        copyToGPU();
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::rotate(Vector2f const& center, float angle) noexcept {
        rotate(center, rotationMatrix<float>(angle));
    }

    template <bool IsMonochromatic>
    void GlyphSprite<IsMonochromatic>::rotate(Vector2f const& center, Matrix2f const& rotation) noexcept {
        for (auto& vertexPosition : vertices | std::views::transform(&Vertex::position)) {
            Vector2f position = vertexPosition;
            Vector2f radius = position - center;
            vertexPosition = rotation * radius + center;
        }
        copyToGPU();
    }

    template <bool IsMonochromatic>
    GlyphSprite<IsMonochromatic>::~GlyphSprite(void) noexcept {
        glDeleteBuffers(1, &elementArrayBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

}
