#pragma once

#include <vector>
#include <concepts>
#include <algorithm>

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "Views.hpp"

namespace ge {

    template <class T>
    concept DrawableType = std::is_base_of_v<Drawable, T> && std::is_base_of_v<Transformable, T> && std::is_constructible_v<T, const std::shared_ptr<Vector2i>&>;

    template <class T, class Alloc>
    using DrawableVector = std::vector<std::unique_ptr<T>, Alloc>;

    template <DrawableType Base, class Allocator = std::allocator<std::unique_ptr<Base>>>
    class DrawableArray : private DrawableVector<Base, Allocator>, public Drawable, public Transformable {
    public:
        explicit DrawableArray(const std::shared_ptr<Vector2i>& scene) noexcept;
        explicit DrawableArray(const std::shared_ptr<Vector2i>& scene, std::size_t size, const Base& base) noexcept;

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
            requires std::is_constructible_v<Base, const std::shared_ptr<Vector2i>&, Args...>
        void emplaceBack(Args&&... args) noexcept;

        template <class InnerIterator, typename value_type>
        class Iterator : public std::iterator<std::contiguous_iterator_tag, value_type, std::ptrdiff_t, value_type*, value_type&> {
        public:
            using reference = value_type&;
            using pointer = value_type*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::contiguous_iterator_tag;

            explicit Iterator(void) noexcept = default;
            explicit Iterator(const InnerIterator& iter) noexcept : iter{iter} {}

            Iterator& operator++(void) noexcept { ++iter; return *this; }
            Iterator& operator++(int)  noexcept { auto tmp = *this; ++iter; return tmp; }
            Iterator& operator--(void) noexcept { --iter; return *this; }
            Iterator& operator--(int)  noexcept { auto tmp = *this; --iter; return tmp; }
            reference operator* (void) const noexcept { return **iter; }
            pointer   operator->(void) noexcept { return *iter; }

            friend bool operator== (const Iterator& right, const Iterator& left) noexcept { return right.iter == left.iter; }

            friend Iterator operator+ (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter += left; return tmp; }
            friend Iterator operator+ (std::size_t right, const Iterator& left) noexcept { auto tmp = left; tmp.iter += right; return tmp; }
            friend Iterator operator- (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter -= left; return tmp; }
            friend difference_type operator- (const Iterator& right, const Iterator& left) noexcept { return right.iter - left.iter; }

            friend auto operator<=> (const Iterator& right, const Iterator& left) noexcept {  left.iter < right.iter ? std::weak_ordering::less : right.iter < left.iter ? std::weak_ordering::greater : std::weak_ordering::equivalent; }
        private:
            InnerIterator iter;
        };

        using iterator = Iterator<typename DrawableVector<Base, Allocator>::iterator, Base>;
        using const_iterator = Iterator<typename DrawableVector<Base, Allocator>::const_iterator, const Base>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using value_type = Base;

        iterator begin(void) noexcept { return iterator{ DrawableVector<Base, Allocator>::begin() }; }
        iterator end(void) noexcept { return iterator{ DrawableVector<Base, Allocator>::end() }; }

        const_iterator begin(void) const noexcept { return const_iterator{ DrawableVector<Base, Allocator>::begin() }; }
        const_iterator end(void) const noexcept { return const_iterator{ DrawableVector<Base, Allocator>::end() }; }

        const_iterator cbegin(void) const noexcept { return const_iterator{ DrawableVector<Base, Allocator>::cbegin() }; }
        const_iterator cend(void) const noexcept { return const_iterator{ DrawableVector<Base, Allocator>::cend() }; }

        reverse_iterator rbegin(void) noexcept { return reverse_iterator{ --end() }; }
        reverse_iterator rend(void) noexcept { return reverse_iterator{ --begin() }; }

        const_reverse_iterator rbegin(void) const noexcept { return const_reverse_iterator{ --end() }; }
        const_reverse_iterator rend(void) const noexcept { return const_reverse_iterator{ --begin() }; }

        const_reverse_iterator crbegin(void) const noexcept { return const_reverse_iterator{ --cend() }; }
        const_reverse_iterator crend(void) const noexcept { return const_reverse_iterator{ --cbegin() }; }

        virtual void setShaders(const ShaderLibrary& shaderLibrary) noexcept final;
        virtual void copyToGPU(void) noexcept final;
        virtual void draw(void) const noexcept final;

        virtual void onScreenTransformation(const Vector2i& oldDimmensions) noexcept final;

        ~DrawableArray(void) noexcept = default;
    };

    // templates

    template <DrawableType Base, class Allocator>
    DrawableArray<Base, Allocator>::DrawableArray(const std::shared_ptr<Vector2i>& scene) noexcept : DrawableVector<Base, Allocator>{}, Drawable{scene} {}

    template <DrawableType Base, class Allocator>
    DrawableArray<Base, Allocator>::DrawableArray(const std::shared_ptr<Vector2i>& scene, std::size_t size, const Base& base) noexcept : DrawableVector<Base, Allocator>{}, Drawable{scene} {
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
        requires std::is_constructible_v<Base, const std::shared_ptr<Vector2i>&, Args...>
    void DrawableArray<Base, Allocator>::emplaceBack(Args&&... args) noexcept {
        this->emplace_back(std::move(std::make_unique<Base>(scene, std::forward<Args>(args)...)));
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::setShaders(const ShaderLibrary& shaderLibrary) noexcept {
        std::ranges::for_each(*this, [&shaderLibrary](auto& drawable){ drawable.setShaders(shaderLibrary); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::copyToGPU(void) noexcept {
        std::ranges::for_each(*this, [](auto& drawable){ drawable.copyToGPU(); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::draw(void) const noexcept {
        std::ranges::for_each(*this, [](const auto& drawable){ drawable.draw(); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::onScreenTransformation(const Vector2i& oldDimmensions) noexcept {
        std::ranges::for_each(*this, [&oldDimmensions](auto& transformable){ transformable.onScreenTransformation(oldDimmensions); });
    }
}
