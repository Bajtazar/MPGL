#pragma once

#include <vector>
#include <concepts>
#include <algorithm>

#include "../Transformations/Transformable2D.hpp"
#include "../../Mathematics/Systems.hpp"
#include "../Drawable.hpp"
#include "Views.hpp"

namespace ge {

    template <class T>
    concept DrawableType = std::is_base_of_v<Drawable, T>
        && std::is_base_of_v<Transformable2D, T>
        && std::is_constructible_v<T, std::shared_ptr<Vector2ui> const&>;

    template <class T, class Alloc>
    using DrawableVector = std::vector<std::unique_ptr<T>, Alloc>;

    template <DrawableType Base,
        class Allocator = std::allocator<std::unique_ptr<Base>>>
    class DrawableArray : private DrawableVector<Base, Allocator>,
        public Drawable, public Transformable2D
    {
    public:
        typedef std::shared_ptr<Vector2ui>                  ScenePtr;

        explicit DrawableArray(ScenePtr const& scene) noexcept;
        explicit DrawableArray(ScenePtr const& scene, std::size_t size,
            Base const& base) noexcept;

        DrawableArray(const DrawableArray& drawableArray) noexcept = default;
        DrawableArray(DrawableArray&& drawableArray) noexcept = default;

        DrawableArray& operator= (const DrawableArray& drawableArray) noexcept = default;
        DrawableArray& operator= (DrawableArray&& drawableArray) noexcept = default;

        Base& operator[] (std::size_t index) noexcept
            { return *DrawableVector<Base, Allocator>::operator[](index); }
        Base const& operator[] (std::size_t index) const noexcept
            { return *DrawableVector<Base, Allocator>::operator[](index); }

        std::size_t size(void) const noexcept
            { return DrawableVector<Base, Allocator>::size(); }
        void reserve(std::size_t size) noexcept
            { DrawableVector<Base, Allocator>::reserve(size); }
        void clear(void) noexcept
            { DrawableVector<Base, Allocator>::clear(); }

        void pushBack(const Base& drawable) noexcept;
        void pushBack(Base&& drawable) noexcept;
        template <typename... Args>
            requires std::is_constructible_v<Base, ScenePtr const&, Args...>
        void emplaceBack(Args&&... args) noexcept;

        template <class InnerIterator, typename value_type>
        class Iterator : public std::iterator<std::contiguous_iterator_tag,
            value_type, std::ptrdiff_t, value_type*, value_type&>
        {
        public:
            using reference = value_type&;
            using pointer = value_type*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::contiguous_iterator_tag;

            explicit Iterator(void) noexcept = default;
            explicit Iterator(InnerIterator const& iter) noexcept : iter{iter} {}

            Iterator& operator++(void) noexcept
                { ++iter; return *this; }
            Iterator& operator++(int)  noexcept
                { auto temp = *this; ++iter; return temp; }
            Iterator& operator--(void) noexcept
                { --iter; return *this; }
            Iterator& operator--(int)  noexcept
                { auto temp = *this; --iter; return temp; }
            reference operator* (void) const noexcept
                { return **iter; }
            pointer   operator->(void) noexcept
                { return *iter; }

            friend bool operator== (Iterator const& right, Iterator const& left) noexcept
                { return right.iter == left.iter; }

            friend Iterator operator+ (Iterator const& right, std::size_t left) noexcept
                { auto temp = right; temp.iter += left; return temp; }
            friend Iterator operator+ (std::size_t right, Iterator const& left) noexcept
                { auto temp = left; temp.iter += right; return temp; }
            friend Iterator operator- (Iterator const& right, std::size_t left) noexcept
                { auto temp = right; temp.iter -= left; return temp; }
            friend difference_type operator- (Iterator const& right, Iterator const& left) noexcept
                { return right.iter - left.iter; }

            friend auto operator<=> (Iterator const& right, Iterator const& left) noexcept
                {  left.iter < right.iter ? std::weak_ordering::less : right.iter < left.iter ?
                    std::weak_ordering::greater : std::weak_ordering::equivalent; }
        private:
            InnerIterator iter;
        };

        using iterator = Iterator<typename DrawableVector<Base,
            Allocator>::iterator, Base>;
        using const_iterator = Iterator<typename DrawableVector<Base,
            Allocator>::const_iterator, const Base>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using value_type = Base;

        iterator begin(void) noexcept
            { return iterator{ DrawableVector<Base, Allocator>::begin() }; }
        iterator end(void) noexcept
            { return iterator{ DrawableVector<Base, Allocator>::end() }; }

        const_iterator begin(void) const noexcept
            { return const_iterator{ DrawableVector<Base, Allocator>::begin() }; }
        const_iterator end(void) const noexcept
            { return const_iterator{ DrawableVector<Base, Allocator>::end() }; }

        const_iterator cbegin(void) const noexcept
            { return const_iterator{ DrawableVector<Base, Allocator>::cbegin() }; }
        const_iterator cend(void) const noexcept
            { return const_iterator{ DrawableVector<Base, Allocator>::cend() }; }

        reverse_iterator rbegin(void) noexcept
            { return reverse_iterator{ --end() }; }
        reverse_iterator rend(void) noexcept
            { return reverse_iterator{ --begin() }; }

        const_reverse_iterator rbegin(void) const noexcept
            { return const_reverse_iterator{ --end() }; }
        const_reverse_iterator rend(void) const noexcept
            { return const_reverse_iterator{ --begin() }; }

        const_reverse_iterator crbegin(void) const noexcept
            { return const_reverse_iterator{ --cend() }; }
        const_reverse_iterator crend(void) const noexcept
            { return const_reverse_iterator{ --cbegin() }; }

        void setShaders(ShaderLibrary const& shaderLibrary) noexcept final;
        void copyToGPU(void) const noexcept final;
        void draw(void) const noexcept final;

        void onScreenTransformation(const Vector2ui& oldDimmensions) noexcept final;
        void translate(Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center, float factor) noexcept final;
        void rotate(Vector2f const& center, float angle) noexcept final;
        void rotate(Vector2f const& center, Matrix2f const& rotation) noexcept final;

        ~DrawableArray(void) noexcept = default;
    private:
        ShaderLibrary const*                    library;

        void setShadersIfLibrary(void) noexcept;
    };

    // templates

    template <DrawableType Base, class Allocator>
    DrawableArray<Base, Allocator>::DrawableArray(ScenePtr const& scene) noexcept
        : DrawableVector<Base, Allocator>{}, Drawable{scene}, library{nullptr} {}

    template <DrawableType Base, class Allocator>
    DrawableArray<Base, Allocator>::DrawableArray(ScenePtr const& scene,
        std::size_t size, const Base& base) noexcept
            : DrawableVector<Base, Allocator>{}, Drawable{scene}, library{nullptr}
    {
        DrawableVector<Base, Allocator>::reserve(size);
        for (std::size_t i = 0;i < size; ++i)
            this->push_back(std::move(std::make_unique<Base>(base)));
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::setShadersIfLibrary(void) noexcept {
        if (library)
            this->back()->setShaders(*library);
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::pushBack(Base const& drawable) noexcept {
        this->push_back(std::move(std::make_unique<Base>(drawable)));
        setShadersIfLibrary();
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::pushBack(Base&& drawable) noexcept {
        this->push_back(std::move(std::make_unique<Base>(std::move(drawable))));
        setShadersIfLibrary();
    }

    template <DrawableType Base, class Allocator>
    template <typename... Args>
        requires std::is_constructible_v<Base, const std::shared_ptr<Vector2ui>&, Args...>
    void DrawableArray<Base, Allocator>::emplaceBack(Args&&... args) noexcept {
        this->emplace_back(std::move(std::make_unique<Base>(scene,
            std::forward<Args>(args)...)));
        setShadersIfLibrary();
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::setShaders(const ShaderLibrary& shaderLibrary) noexcept {
        library = &shaderLibrary;
        std::ranges::for_each(*this, [&shaderLibrary](auto& drawable)
            { drawable.setShaders(shaderLibrary); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::copyToGPU(void) const noexcept {
        std::ranges::for_each(*this, [](const auto& drawable){ drawable.copyToGPU(); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::draw(void) const noexcept {
        std::ranges::for_each(*this, [](const auto& drawable){ drawable.draw(); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::onScreenTransformation(
        Vector2ui const& oldDimmensions) noexcept
    {
        std::ranges::for_each(*this, [&oldDimmensions](auto& transformable)
            { transformable.onScreenTransformation(oldDimmensions); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::translate(const Vector2f& shift) noexcept {
        std::ranges::for_each(*this, [&shift](auto& transformable)
            { transformable.translate(shift); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::scale(Vector2f const& center, float factor) noexcept {
        std::ranges::for_each(*this, [&center, &factor](auto& transformable)
            { transformable.scale(center, factor); });
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::rotate(Vector2f const& center, float angle) noexcept {
        rotate(center, rotationMatrix<float>(angle));
    }

    template <DrawableType Base, class Allocator>
    void DrawableArray<Base, Allocator>::rotate(Vector2f const& center,
        Matrix2f const& rotation) noexcept
    {
        std::ranges::for_each(*this, [&center, &rotation](auto& transformable)
            { transformable.rotate(center, rotation); });
    }

}
