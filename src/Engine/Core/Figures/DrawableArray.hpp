#pragma once

#include <vector>
#include <concepts>
#include <algorithm>
#include <memory>

#include "Drawable.hpp"

namespace ge {

    template <class T>
    concept DrawableType = std::is_base_of_v<Drawable, T> && std::is_constructible_v<T, const Vector2i&>;

    template <class T, class Alloc>
    using DrawableVector = std::vector<std::unique_ptr<T>, Alloc>;

    template <DrawableType Base, class Allocator = std::allocator<std::unique_ptr<Base>>>
    class DrawableArray : private DrawableVector<Base, Allocator>, public Drawable {
    public:
        explicit DrawableArray(const Vector2i& scene) noexcept;
        explicit DrawableArray(const Vector2i& scene, std::size_t size, const Base& base) noexcept;

        DrawableArray(const DrawableArray& drawableArray) noexcept = default;
        DrawableArray(DrawableArray&& drawableArray) noexcept = default;

        DrawableArray& operator= (const DrawableArray& drawableArray) noexcept = default;
        DrawableArray& operator= (DrawableArray&& drawableArray) noexcept = default;

        Base& operator[] (std::size_t index) noexcept { return *DrawableVector<Base, Allocator>::operator[](index); }
        const Base& operator[] (std::size_t index) const noexcept { return *DrawableVector<Base, Allocator>::operator[](index); }

        std::size_t size(void) const noexcept { return DrawableVector<Base, Allocator>::size(); }
        void reserve(std::size_t size) noexcept { DrawableVector<Base, Allocator>::reserve(size); }
        void clear(void) noexcept { DrawableVector<Base, Allocator>::clear(); }

        void pushBack(const Base& drawable) noexcept;
        void pushBack(Base&& drawable) noexcept;
        template <typename... Args>
            requires std::is_constructible_v<Base, const Vector2f&, Args...>
        void emplaceBack(Args&&... args) noexcept;

        using iterator = DrawableVector<Base, Allocator>::iterator;
        using const_iterator = DrawableVector<Base, Allocator>::const_iterator;
        using reverse_iterator = DrawableVector<Base, Allocator>::reverse_iterator;
        using const_reverse_iterator = DrawableVector<Base, Allocator>::const_reverse_iterator;

        using value_type = Base;

        iterator begin(void) noexcept { return DrawableVector<Base, Allocator>::begin(); }
        iterator end(void) noexcept { return DrawableVector<Base, Allocator>::end(); }

        const_iterator begin(void) const noexcept { return DrawableVector<Base, Allocator>::begin(); }
        const_iterator end(void) const noexcept { return DrawableVector<Base, Allocator>::end(); }

        const_iterator cbegin(void) const noexcept { return DrawableVector<Base, Allocator>::begin(); }
        const_iterator cend(void) const noexcept { return DrawableVector<Base, Allocator>::end(); }

        reverse_iterator rbegin(void) noexcept { return DrawableVector<Base, Allocator>::rbegin(); }
        reverse_iterator rend(void) noexcept { return DrawableVector<Base, Allocator>::rend(); }

        const_reverse_iterator rbegin(void) const noexcept { return DrawableVector<Base, Allocator>::rbegin(); }
        const_reverse_iterator rend(void) const noexcept { return DrawableVector<Base, Allocator>::rend(); }

        const_reverse_iterator crbegin(void) const noexcept { return DrawableVector<Base, Allocator>::crbegin(); }
        const_reverse_iterator crend(void) const noexcept { return DrawableVector<Base, Allocator>::crend(); }

        virtual void setShaders(const ShaderLibrary& shaderLibrary) noexcept final;
        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        ~DrawableArray(void) noexcept = default;
    };

    // templates

    template <DrawableType Base, class Allocator>
    DrawableArray<Base, Allocator>::DrawableArray(const Vector2i& scene) noexcept : DrawableVector<Base, Allocator>{}, Drawable{scene} {}

    template <DrawableType Base, class Allocator>
    DrawableArray<Base, Allocator>::DrawableArray(const Vector2i& scene, std::size_t size, const Base& base) noexcept : DrawableVector<Base, Allocator>{}, Drawable{scene} {
        DrawableVector<Base, Allocator>::reserve(size);
        for (std::size_t i = 0;i < size; ++i)
            this->push_back(std::move(std::make_unique<Base>(base)));
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::pushBack(const Base& drawable) noexcept {
        this->push_back(std::move(std::make_unique<Base>(drawable)));
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::pushBack(Base&& drawable) noexcept {
        this->push_back(std::move(std::make_unique<Base>(std::move(drawable))));
    }

    template <DrawableType Base, class Allocator>
    template <typename... Args>
        requires std::is_constructible_v<Base, const Vector2f&, Args...>
    void DrawableArray<Base, Allocator>::emplaceBack(Args&&... args) noexcept {
        this->emplace_back(std::move(std::make_unique<Base>(scene, std::forward<Args>(args)...)));
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::setShaders(const ShaderLibrary& shaderLibrary) noexcept {
        std::ranges::for_each(*this, [&shaderLibrary](auto& drawable){ drawable->setShaders(shaderLibrary); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::copyToGPU(void) noexcept {
        std::ranges::for_each(*this, [](auto& drawable){ drawable->copyToGPU(); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::draw(void) const noexcept {
        std::ranges::for_each(*this, [](const auto& drawable){ drawable->draw(); });
    }
}
