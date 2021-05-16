#pragma once

#include "FigureType.hpp"
#include "Drawable.hpp"
#include "../Color.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

namespace ge {

    template <bool PolygonMode = false>
    class VertexComponent : public Drawable {
    public:
        struct Vertex {
            constexpr explicit Vertex(const Vector2f& position, const Color& color) noexcept : color(color), position(position) {}
            constexpr explicit Vertex(void) noexcept = default;

            Vector2f position;
            float depth;
            Color color;

            template<std::size_t Index>
            constexpr auto&& get() &  { return __getHelper<Index>(*this); }

            template<std::size_t Index>
            constexpr auto&& get() && { return __getHelper<Index>(*this); }

            template<std::size_t Index>
            constexpr auto&& get() const &  { return __getHelper<Index>(*this); }

            template<std::size_t Index>
            constexpr auto&& get() const && { return __getHelper<Index>(*this); }
        private:
            template<std::size_t Index, typename _This>
            constexpr auto&& __getHelper(_This&& __this) {
                static_assert(Index < 3, "Index out of Vertex bounds");
                if constexpr (Index == 0) return std::forward<_This>(__this).position;
                if constexpr (Index == 1) return std::forward<_This>(__this).color;
                if constexpr (Index == 2) return std::forward<_This>(__this).depth;
            }
        };

        explicit VertexComponent(void) noexcept = default;
        explicit VertexComponent(const Vector2i& scene, const FigureType& type) noexcept;

        virtual void setShaders(const ShaderLibrary& shaderLibrary) noexcept final;
        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        Vertex& operator[] (uint16_t index) noexcept { return vertices[index]; }

        using iterator = std::vector<Vertex>::iterator;
        using const_iterator = std::vector<Vertex>::const_iterator;

        iterator begin(void) noexcept { return iterator{ vertices.begin() }; }
        iterator end(void) noexcept { return iterator{ vertices.end() }; }
        const_iterator begin(void) const noexcept { return const_iterator{ vertices.begin() }; }
        const_iterator end(void) const noexcept { return const_iterator{ vertices.end() }; }

        ~VertexComponent(void) noexcept;
    protected:
        std::vector<Vertex> vertices;
        uint32_t verticesBuffer;
        uint32_t vertexArrayObject;
    };

    template class VertexComponent<true>;
    template class VertexComponent<false>;

    typedef VertexComponent<false> FilledComponent;
    typedef VertexComponent<true> PolygonComponent;

}
