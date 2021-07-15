#pragma once

#include "../../Utility/Adapter.hpp"
#include "../Color.hpp"
#include "../Texture.hpp"
#include "Drawable.hpp"
#include "Transformable.hpp"

#include <utility>
#include <array>

namespace ge {

    struct ColorableVertex {
        constexpr explicit ColorableVertex(const Vector2f& position, const Color& color, const Vector2f& textureCoords, const std::shared_ptr<Vector2i>& scene) noexcept
            : position{position, scene}, textureCoords{textureCoords}, color{color} {}

        Adapter<Vector2f, Vector2i> position;
        Vector2f textureCoords;
        Color color;

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr auto& get(void) noexcept { return std::get<2 - Index>(reinterpret_cast<std::tuple<Color, Vector2f, Adapter<Vector2f, Vector2i>>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr const auto& get(void) const noexcept { return std::get<2 - Index>(reinterpret_cast<const std::tuple<Color, Vector2f, Adapter<Vector2f, Vector2i>>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr auto&& get(void) noexcept { return std::get<2 - Index>(reinterpret_cast<std::tuple<Color, Vector2f, Adapter<Vector2f, Vector2i>>&&>(*this)); }

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr const auto&& get(void) const noexcept { return std::get<2 - Index>(reinterpret_cast<const std::tuple<Color, Vector2f, Adapter<Vector2f, Vector2i>>&&>(*this)); }
    };

    struct DefaultVertex {
        constexpr explicit DefaultVertex(const Vector2f& position, const Vector2f& textureCoords, const std::shared_ptr<Vector2i>& scene) noexcept : position{position, scene}, textureCoords{textureCoords} {}

        Adapter<Vector2f, Vector2i> position;
        Vector2f textureCoords;

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr auto& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Vector2f, Adapter<Vector2f, Vector2i>>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr const auto& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Vector2f, Adapter<Vector2f, Vector2i>>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr auto&& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Vector2f, Adapter<Vector2f, Vector2i>>&&>(*this)); }

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr const auto&& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Vector2f, Adapter<Vector2f, Vector2i>>&&>(*this)); }
    };

    template <bool IsColorable = false>
    class Sprite : public Drawable, public Transformable {
    public:
        using Vertex = std::conditional_t<IsColorable, ColorableVertex, DefaultVertex>;

        // Default constructor
        Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture) noexcept;
        // parallelogram
        Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture,
               const Vector2f& firstVertex,            const Vector2f& secondVertex,
               const Vector2f& thirdVertex) noexcept;
        // parallelogram for colorable version
        template <bool Colorable = IsColorable>
            requires Colorable
        Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture,
               const Vector2f& firstVertex,            const Vector2f& secondVertex,
               const Vector2f& thirdVertex,            const Color& color) noexcept;
        // rectangle parallel to the x and y axis
        Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture,
               const Vector2f& firstVertex,            const Vector2f& dimmensions) noexcept;
        // rectangle parallel to the x and y axis for colorable version
        template <bool Colorable = IsColorable>
            requires Colorable
        Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture,
               const Vector2f& firstVertex,            const Vector2f& dimmensions,
               const Color& color) noexcept;

        Sprite(const Sprite& sprite) noexcept;
        Sprite(Sprite&& sprite) noexcept;

        Sprite& operator= (const Sprite& sprite) noexcept;
        Sprite& operator= (Sprite&& sprite) noexcept;

        virtual void setShaders(const ShaderLibrary& library) noexcept final;
        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        virtual void onScreenTransformation(const Vector2i& oldDimmensions) noexcept final;
        virtual void translate(const Vector2f& shift) noexcept final;
        virtual void scale(const Vector2f& center, float factor) noexcept final;
        virtual void rotate(const Vector2f& center, float angle) noexcept final;

        std::size_t size(void) const noexcept { return vertices.size(); }

        using iterator = std::array<Vertex, 4>::iterator;
        using const_iterator = std::array<Vertex, 4>::const_iterator;
        using reverse_iterator = std::array<Vertex, 4>::reverse_iterator;
        using const_reverse_iterator = std::array<Vertex, 4>::const_reverse_iterator;

        iterator begin(void) noexcept { return vertices.begin(); }
        iterator end(void) noexcept { return vertices.end(); }

        const_iterator begin(void) const noexcept { return vertices.begin(); }
        const_iterator end(void) const noexcept { return vertices.end(); }

        const_iterator cbegin(void) const noexcept { return vertices.cbegin(); }
        const_iterator cend(void) const noexcept { return vertices.cend(); }

        reverse_iterator rbegin(void) noexcept { return vertices.rbegin(); }
        reverse_iterator rend(void) noexcept { return vertices.rend(); }

        const_reverse_iterator rbegin(void) const noexcept { return vertices.rbegin(); }
        const_reverse_iterator rend(void) const noexcept { return vertices.rend(); }

        const_reverse_iterator crbegin(void) const noexcept { return vertices.crbegin(); }
        const_reverse_iterator crend(void) const noexcept { return vertices.crend(); }

        ~Sprite(void) noexcept;
    private:
        static const std::array<uint32_t, 6> indexes;
        std::array<Vertex, 4> vertices;
        Texture texture;
        uint32_t elementArrayBuffer;
        uint32_t shaderProgram;
        uint32_t vertexBuffer;
        uint32_t vertexArrayObject;

        static std::array<Vertex, 4> makeVertexArray(const std::shared_ptr<Vector2i>& scene) noexcept;
        template <bool Colorable = IsColorable>
            requires Colorable
        static std::array<Vertex, 4> makeVertexArray(const std::shared_ptr<Vector2i>& scene, const Color& color) noexcept;
    };

    template class Sprite<true>;
    template class Sprite<false>;

    typedef Sprite<false> DefaultSprite;
    typedef Sprite<true> ColorableSprite;

    template <bool IsColorable>
    template <bool Colorable>
        requires Colorable
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture, const Vector2f& firstVertex, const Vector2f& dimmensions, const Color& color) noexcept
    :  Drawable{scene}, vertices{std::move(makeVertexArray(scene, color))}, texture{texture} {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementArrayBuffer);
        vertices[0].position = firstVertex;
        vertices[1].position = firstVertex + Vector2f{0.f, dimmensions[1]};
        vertices[2].position = firstVertex + dimmensions;
        vertices[3].position = firstVertex + Vector2f{dimmensions[0], 0.f};
    }

    template <bool IsColorable>
    template <bool Colorable>
        requires Colorable
    Sprite<IsColorable>::Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture, const Vector2f& firstVertex, const Vector2f& secondVertex, const Vector2f& thirdVertex, const Color& color) noexcept
    : Drawable{scene}, vertices{std::move(makeVertexArray(scene, color))}, texture{texture} {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementArrayBuffer);
        vertices[0].position = firstVertex;
        vertices[1].position = secondVertex;
        vertices[2].position = thirdVertex;
        vertices[3].position = firstVertex - secondVertex + thirdVertex;
    }

    template <bool IsColorable>
    template <bool Colorable>
        requires Colorable
    std::array<typename Sprite<IsColorable>::Vertex, 4> Sprite<IsColorable>::makeVertexArray(const std::shared_ptr<Vector2i>& scene, const Color& color) noexcept {
        return {Vertex{{}, color, {0.f, 0.f}, scene}, Vertex{{}, color, {0.f, 1.f}, scene}, Vertex{{}, color, {1.f, 1.f}, scene}, Vertex{{}, color, {1.f, 0.f}, scene}};
    }

}
