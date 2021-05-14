#pragma once

#include "FigureType.hpp"
#include "Drawable.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

namespace ge {

    class VertexComponent : public Drawable {
    private:
        using Vertex = std::pair<Vector2f, float>;
    public:
        explicit VertexComponent(const Vector2i& scene, FigureTypes::PrimitveType auto type) noexcept;
        explicit VertexComponent(const Vector2i& scene, FigureTypes::LineType auto type) noexcept;

        virtual void setShaders(const ShaderLibrary& shaderLibrary) noexcept final;
        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        template <std::contiguous_iterator _Iter, typename _Tp>
        class Iterator : std::iterator<std::forward_iterator_tag, _Tp, void, void, void> {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = _Tp;
            using reference = value_type&;
            using pointer = value_type*;

            explicit Iterator(void) noexcept = default;
            explicit Iterator(const _Iter& __iter) noexcept : __iter(__iter) {}

            reference operator* (void) const noexcept { return __iter->first; }
            pointer operator-> (void) noexcept { return &__iter->first; }
            Iterator& operator++ (void) noexcept { ++__iter; return *this; }
            Iterator operator++ (int) noexcept { auto temp = *this; ++__iter; return temp; }

            friend bool operator== (const Iterator& left, const Iterator& right) noexcept { return left.__iter == right.__iter; }
            friend bool operator!= (const Iterator& left, const Iterator& right) noexcept { return left.__iter != right.__iter; }
        private:
            _Iter __iter;
        };

        using iterator = Iterator<std::vector<Vertex>::iterator, Vector2f>;
        using const_iterator = Iterator<std::vector<Vertex>::const_iterator, const Vector2f>;

        iterator begin(void) noexcept { return iterator{vertices.begin()}; }
        iterator end(void) noexcept { return iterator{vertices.end()}; }
        const_iterator begin(void) const noexcept { return const_iterator{vertices.begin()}; }
        const_iterator end(void) const noexcept { return const_iterator{vertices.end()}; }

        ~VertexComponent(void) noexcept;
    private:
        explicit VertexComponent(const Vector2i& scene, size_t size) noexcept;

        std::vector<Vertex> vertices;
        uint32_t verticesBuffer;
        uint32_t vertexArrayObject;
    };

    VertexComponent::VertexComponent(const Vector2i& scene, FigureTypes::PrimitveType auto type) noexcept : VertexComponent(scene, type.verticiesCount()) {

    }

    VertexComponent::VertexComponent(const Vector2i& scene, FigureTypes::LineType auto type) noexcept : VertexComponent(scene, type.verticiesCount()) {

    }

}
