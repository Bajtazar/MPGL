#pragma once

#include "../Color.hpp"
#include "../Drawable.hpp"
#include "../../Utility/Adapter.hpp"
#include "../Transformations/Transformable2D.hpp"

namespace ge {

    class Shape : public Drawable, public Transformable2D {
    public:
        struct Vertex {
            explicit Vertex(const Vector2f& position, const Color& color) noexcept : position{position}, color{color} {}

            Adapter<Vector2f> position;
            Color color;

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr auto& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Color, Adapter<Vector2f>>&>(*this)); }

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr const auto& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Color, Adapter<Vector2f>>&>(*this)); }

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr auto&& get(void) noexcept { return std::get<1 - Index>(reinterpret_cast<std::tuple<Color, Adapter<Vector2f>>&&>(*this)); }

            template <std::size_t Index>
                requires (Index < 2)
            inline constexpr const auto&& get(void) const noexcept { return std::get<1 - Index>(reinterpret_cast<const std::tuple<Color, Adapter<Vector2f>>&&>(*this)); }
        };

        void setShaders(const ShaderLibrary& shaderLibrary) noexcept final;
        void copyToGPU(void) const noexcept final;
        virtual void draw(void) const noexcept = 0;

        void onScreenTransformation(const Vector2ui& oldDimmensions) noexcept final;
        void translate(const Vector2f& shift) noexcept final;
        void scale(const Vector2f& center, float factor) noexcept final;
        void rotate(const Vector2f& center, float angle) noexcept final;
        void rotate(const Vector2f& center, const Matrix2f& rotation) noexcept final;

        Vertex& operator[] (std::size_t index) noexcept { return vertices[index]; }
        const Vertex& operator[] (std::size_t index) const noexcept { return vertices[index]; }

        Vertex& front(void) noexcept { return vertices.front(); }
        const Vertex& front(void) const noexcept { return vertices.front(); }

        Vertex& back(void) noexcept { return vertices.back(); }
        const Vertex& back(void) const noexcept { return vertices.back(); }

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

        const_reverse_iterator rbegin(void) const noexcept { return vertices.rbegin(); }
        const_reverse_iterator rend(void) const noexcept { return vertices.rend(); }

        const_reverse_iterator crbegin(void) const noexcept { return vertices.crbegin(); }
        const_reverse_iterator crend(void) const noexcept { return vertices.crend(); }

        virtual ~Shape(void) noexcept;
    protected:
        explicit Shape(size_t size) noexcept;
        explicit Shape(std::vector<Vertex>&& vertices) noexcept;

        virtual void bindBuffers(void) const noexcept;
        virtual void unbindBuffers(void) const noexcept;
        void copyBuffersToGPU(void) const noexcept;

        std::vector<Vertex> vertices;
        uint32_t shaderProgram;
        uint32_t vertexBuffer;
        uint32_t vertexArrayObject;
    };

}
