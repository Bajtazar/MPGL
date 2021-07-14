#pragma once

#include "../../Utility/Adapter.hpp"
#include "../Color.hpp"
#include "../Texture.hpp"
#include "Drawable.hpp"
#include "Transformable.hpp"

#include <utility>

namespace ge {

    template <class Vertex>
    class Sprite : public Drawable, public Transformable {
    public:
        Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture) noexcept;
        // parallelogram
        Sprite(const std::shared_ptr<Vector2i>& scene, const Texture& texture,
               const Vector2f& firstVertex,            const Vector2f& secondVertex,
               const Vector2f& thirdVertex,            const Color& color) noexcept;
        // rectangle parallel to the x and y axis
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

        using iterator = std::vector<Vertex>::iterator;
        using const_iterator = std::vector<Vertex>::const_iterator;
        using reverse_iterator = std::vector<Vertex>::reverse_iterator;
        using const_reverse_iterator = std::vector<Vertex>::const_reverse_iterator;

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
        std::vector<Vertex> vertices;
        Texture texture;
        uint32_t elementArrayBuffer;
        uint32_t shaderProgram;
        uint32_t vertexBuffer;
        uint32_t vertexArrayObject;
    };

    struct ColorableVertex {
        constexpr explicit ColorableVertex(const Vector2f& position, const Color& color, const Vector2f& textureCoords, const std::shared_ptr<Vector2i>& scene) noexcept
            : position{position, scene}, color{color}, textureCoords{textureCoords} {}

        Adapter<Vector2f, Vector2i> position;
        Color color;
        Vector2f textureCoords;

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr auto& get(void) noexcept { return std::get<2 - Index>(reinterpret_cast<std::tuple<Vector2f, Color, Adapter<Vector2f, Vector2i>>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr const auto& get(void) const noexcept { return std::get<2 - Index>(reinterpret_cast<const std::tuple<Vector2f, Color, Adapter<Vector2f, Vector2i>>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr auto&& get(void) noexcept { return std::get<2 - Index>(reinterpret_cast<std::tuple<Vector2f, Color, Adapter<Vector2f, Vector2i>>&&>(*this)); }

        template <std::size_t Index>
            requires (Index < 3)
        inline constexpr const auto&& get(void) const noexcept { return std::get<2 - Index>(reinterpret_cast<const std::tuple<Vector2f, Color, Adapter<Vector2f, Vector2i>>&&>(*this)); }
    };

    struct DefaultVertex {
        constexpr explicit DefaultVertex([[maybe_unused]] const Vector2f& position, const Color& color, const Vector2f& textureCoords, [[maybe_unused]] const std::shared_ptr<Vector2i>& scene) noexcept
            : color{color}, textureCoords{textureCoords} {}

        Color color;
        Vector2f textureCoords;

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr auto& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Vector2f, Color>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr const auto& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Vector2f, Color>&>(*this)); }

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr auto&& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Vector2f, Color>&&>(*this)); }

        template <std::size_t Index>
            requires (Index < 2)
        inline constexpr const auto&& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Vector2f, Color>&&>(*this)); }
    };

    template class Sprite<ColorableVertex>;
    template class Sprite<DefaultVertex>;

    typedef Sprite<DefaultVertex> DefaultSprite;
    typedef Sprite<ColorableVertex> ColorableSprite;

}
