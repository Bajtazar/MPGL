#pragma once

#include "../Color.hpp"
#include "../Drawable.hpp"
#include "../../Utility/Adapter.hpp"
#include "../Transformations/Transformable2D.hpp"

namespace ge {

    class Shape : public Drawable, public Transformable2D {
    public:
        struct Vertex {
            explicit Vertex(Vector2f const& position, Color const& color) noexcept
                : position{position}, color{color} {}

            Adapter<Vector2f>               position;
            Color                           color;

            template <std::size_t Index>
                requires (Index < 2)
            constexpr auto&& get(void) & noexcept
                { if constexpr(Index) return color; else return position; }

            template <std::size_t Index>
                requires (Index < 2)
            constexpr auto&& get(void) const& noexcept
                { if constexpr(Index) return color; else return position; }

            template <std::size_t Index>
                requires (Index < 2)
            constexpr auto&& get(void) && noexcept
                { if constexpr(Index) return std::move(color); else return std::move(position); }
        };

        void setShaders(ShaderLibrary const& shaderLibrary) noexcept final;
        void copyToGPU(void) const noexcept final;
        virtual void draw(void) const noexcept = 0;

        void onScreenTransformation(Vector2ui const& oldDimmensions) noexcept final;
        void translate(Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center, float factor) noexcept final;
        void rotate(Vector2f const& center, float angle) noexcept final;
        void rotate(Vector2f const& center, Matrix2f const& rotation) noexcept final;

        Vertex& operator[] (std::size_t index) noexcept
            { return vertices[index]; }
        Vertex const& operator[] (std::size_t index) const noexcept
            { return vertices[index]; }

        Vertex& front(void) noexcept
            { return vertices.front(); }
        Vertex const& front(void) const noexcept
            { return vertices.front(); }

        Vertex& back(void) noexcept
            { return vertices.back(); }
        Vertex const& back(void) const noexcept
            { return vertices.back(); }

        using iterator = std::vector<Vertex>::iterator;
        using const_iterator = std::vector<Vertex>::const_iterator;
        using reverse_iterator = std::vector<Vertex>::reverse_iterator;
        using const_reverse_iterator = std::vector<Vertex>::const_reverse_iterator;

        std::size_t size(void) const noexcept
            { return vertices.size(); }

        iterator begin(void) noexcept
            { return vertices.begin(); }
        iterator end(void) noexcept
            { return vertices.end(); }

        const_iterator begin(void) const noexcept
            { return vertices.begin(); }
        const_iterator end(void) const noexcept
            { return vertices.end(); }

        const_iterator cbegin(void) const noexcept
            { return vertices.begin(); }
        const_iterator cend(void) const noexcept
            { return vertices.end(); }

        reverse_iterator rbegin(void) noexcept
            { return vertices.rbegin(); }
        reverse_iterator rend(void) noexcept
            { return vertices.rend(); }

        const_reverse_iterator rbegin(void) const noexcept
            { return vertices.rbegin(); }
        const_reverse_iterator rend(void) const noexcept
            { return vertices.rend(); }

        const_reverse_iterator crbegin(void) const noexcept
            { return vertices.crbegin(); }
        const_reverse_iterator crend(void) const noexcept
            { return vertices.crend(); }

        virtual ~Shape(void) noexcept;
    protected:
        explicit Shape(size_t size) noexcept;
        explicit Shape(std::vector<Vertex> vertices) noexcept;

        virtual void bindBuffers(void) const noexcept;
        virtual void unbindBuffers(void) const noexcept;
        void copyBuffersToGPU(void) const noexcept;

        std::vector<Vertex>                 vertices;
        ShaderProgram                       shaderProgram;
        uint32_t                            vertexBuffer;
        uint32_t                            vertexArrayObject;
    };

}
