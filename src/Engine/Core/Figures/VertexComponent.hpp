#pragma once

#include "FigureType.hpp"
#include "Drawable.hpp"
#include "../Color.hpp"
#include "../../Utility/Adapter.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

namespace ge {

    template <bool PolygonMode = false>
    class VertexComponent : public Drawable {
    public:
        struct Vertex {
            constexpr explicit Vertex(const Vector2f& position, const Color& color, const Vector2i& scene) noexcept : position{position, Vector2f(scene)}, color{color} {}

            Adapter<Vector2f> position;
            Color color;

            template<std::size_t Index>
            constexpr auto&& get(void) &  { return getHelper<Index>(*this); }

            template<std::size_t Index>
            constexpr auto&& get(void) && { return getHelper<Index>(*this); }

            template<std::size_t Index>
            constexpr auto&& get(void) const &  { return getHelper<Index>(*this); }

            template<std::size_t Index>
            constexpr auto&& get(void) const && { return getHelper<Index>(*this); }
        private:
            template<std::size_t Index, typename This>
            constexpr auto&& getHelper(This&& __this) {
                static_assert(Index < 2, "Index out of Vertex bounds");
                if constexpr (Index == 0) return std::forward<This>(__this).position;
                if constexpr (Index == 1) return std::forward<This>(__this).color;
            }
        };

        explicit VertexComponent(void) noexcept = default;
        explicit VertexComponent(const Vector2i& scene, const FigureType& type) noexcept : VertexComponent(scene, type.verticiesCount())  {}

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
        explicit VertexComponent(const Vector2i& scene, size_t size) noexcept;

        std::vector<Vertex> vertices;
        uint32_t verticesBuffer;
        uint32_t vertexArrayObject;
    };

    template class VertexComponent<true>;
    template class VertexComponent<false>;

    typedef VertexComponent<false> FilledComponent;
    typedef VertexComponent<true> PolygonComponent;

}
