#pragma once

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "../Color.hpp"
#include "../../Utility/Adapter.hpp"


namespace ge {

    class Shape : public Drawable, public Transformable {
    public:
        struct Vertex {
            constexpr explicit Vertex(const Vector2f& position, const Color& color, const std::shared_ptr<Vector2i>& scene) noexcept : position{position, scene}, color{color} {}

            Adapter<Vector2f, Vector2i> position;
            Color color;

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr auto& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Color, Adapter<Vector2f, Vector2i>>&>(*this)); }

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr const auto& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Color, Adapter<Vector2f, Vector2i>>&>(*this)); }

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr auto&& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Color, Adapter<Vector2f, Vector2i>>&&>(*this)); }

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr const auto&& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Color, Adapter<Vector2f, Vector2i>>&&>(*this)); }
        };

        virtual void setShaders(const ShaderLibrary& shaderLibrary) noexcept final;
        virtual void copyToGPU(void) noexcept = 0;
        virtual void draw(void) const noexcept = 0;

        virtual void onScreenTransformation(const Vector2i& oldDimmensions) noexcept;
        virtual void translate(const Vector2f& shift) noexcept;
        virtual void scale(const Vector2f& center, float factor) noexcept;
        virtual void rotate(const Vector2f& center, float angle) noexcept;

        Vertex& operator[] (std::size_t index) noexcept { return vertices[index]; }
        const Vertex& operator[] (std::size_t index) const noexcept { return vertices[index]; }

        using iterator = std::vector<Vertex>::iterator;
        using const_iterator = std::vector<Vertex>::const_iterator;
        using reverse_iterator = std::vector<Vertex>::reverse_iterator;
        using const_reverse_iterator = std::vector<Vertex>::const_reverse_iterator;

        std::size_t size(void) const noexcept { return vertices.size(); }

        iterator begin(void) noexcept { return vertices.begin(); }
        iterator end(void) noexcept { return vertices.end(); }

        const_iterator begin(void) const noexcept { return vertices.begin(); }
        const_iterator end(void) const noexcept { return vertices.end(); }

        const_iterator cbegin(void) const noexcept { return vertices.begin(); }
        const_iterator cend(void) const noexcept { return vertices.end(); }

        reverse_iterator rbegin(void) noexcept { return vertices.rbegin(); }
        reverse_iterator rend(void) noexcept { return vertices.rend(); }

        const_reverse_iterator crbegin(void) const noexcept { return vertices.crbegin(); }
        const_reverse_iterator crend(void) const noexcept { return vertices.crend(); }

        virtual ~Shape(void) noexcept;
    protected:
        explicit Shape(const std::shared_ptr<Vector2i>& scene, size_t size) noexcept;
        explicit Shape(const std::shared_ptr<Vector2i>& scene, std::vector<Vertex>&& vertices) noexcept;

        std::vector<Vertex> vertices;
        uint32_t vertexBuffer;
        uint32_t vertexArrayObject;
    };

}
