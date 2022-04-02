#pragma once

#include <vector>
#include <concepts>
#include <algorithm>

#include "Transformations/Transformable2D.hpp"
#include "../Mathematics/Systems.hpp"
#include "Drawable.hpp"

namespace mpgl {

    template <class Tp>
    concept DrawableType = std::derived_from<Tp, Drawable2D>
        && std::derived_from<Tp, ScreenTransformationEvent>;

    template <class Tp>
    concept TransformableDrawable = DrawableType<Tp>
        && std::derived_from<Tp, Transformable2D>;

    template <DrawableType Base,
        std::ranges::input_range Sequence = std::vector<Base>>
    class DrawableCollectionBase : public Sequence,
        public Drawable2D,
        public virtual ScreenTransformationEvent
    {
    public:
        using Sequence::Sequence;

        DrawableCollectionBase(DrawableCollectionBase const& base
            ) noexcept = default;
        DrawableCollectionBase(DrawableCollectionBase&& base
            ) noexcept = default;

        DrawableCollectionBase& operator=(DrawableCollectionBase const& base
            ) noexcept = default;
        DrawableCollectionBase& operator=(DrawableCollectionBase&& base
            ) noexcept = default;

        using iterator = typename Sequence::iterator;
        using const_iterator = typename Sequence::const_iterator;
        using reverse_iterator = typename Sequence::reverse_iterator;
        using const_reverse_iterator = typename Sequence::const_reverse_iterator;

        using value_type = Base;
        using size_type = typename Sequence::size_type;

        template <typename Signature, typename... Args>
            requires std::invocable<Signature, Base, Args...>
        void call(Signature signature, Args&&... args) noexcept(
            std::is_nothrow_invocable_v<Signature, Base, Args...>);

        void draw(void) const noexcept final;
        void draw(size_type begin, size_type end) const noexcept
            requires std::ranges::random_access_range<Sequence>;

        void onScreenTransformation(Vector2u const& oldDimensions
            ) noexcept final;

        ~DrawableCollectionBase(void) noexcept = default;
    };

    template <DrawableType Base,
        std::ranges::input_range Sequence = std::vector<Base>>
    class DrawableCollection : public DrawableCollectionBase<Base, Sequence> {
    public:
        using DrawableCollectionBase<Base, Sequence>::DrawableCollectionBase;

        ~DrawableCollection(void) noexcept = default;
    };

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    class DrawableCollection<Base, Sequence>
        : public DrawableCollectionBase<Base, Sequence>, public Transformable2D
    {
    public:
        using DrawableCollectionBase<Base, Sequence>::DrawableCollectionBase;

        void translate(Vector2f const& shift) noexcept final;
        void scale(Vector2f const& center, float32 factor) noexcept final;
        void rotate(Vector2f const& center, float32 angle) noexcept final;
        void rotate(Vector2f const& center, Matrix2f const& rotation) noexcept final;

        ~DrawableCollection(void) noexcept = default;
    };

    // templates

    template <DrawableType Base, std::ranges::input_range Sequence>
    template <typename Signature, typename... Args>
            requires std::invocable<Signature, Base, Args...>
    void DrawableCollectionBase<Base, Sequence>::call(Signature signature,
        Args&&... args) noexcept(std::is_nothrow_invocable_v<Signature, Base, Args...>)
    {
        std::ranges::for_each(*this, [&](auto& drawable)
            { (drawable->signature)(std::forward<Args>(args)...); });
    }

    template <DrawableType Base, std::ranges::input_range Sequence>
    void DrawableCollectionBase<Base, Sequence>::draw(void) const noexcept {
        std::ranges::for_each(*this, [](auto const& drawable)
            { drawable.draw(); });
    }

    template <DrawableType Base, std::ranges::input_range Sequence>
    void DrawableCollectionBase<Base, Sequence>::draw(
        size_type begin, size_type end) const noexcept
            requires std::ranges::random_access_range<Sequence>
    {
        for (size_type i = begin; i != end; ++i)
            (*this)[i].draw();
    }

    template <DrawableType Base, std::ranges::input_range Sequence>
    void DrawableCollectionBase<Base, Sequence>::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        std::ranges::for_each(*this, [&oldDimensions](auto& drawable)
            { drawable.onScreenTransformation(oldDimensions); });
    }

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    void DrawableCollection<Base, Sequence>::translate(Vector2f const& shift) noexcept {
        std::ranges::for_each(*this, [&shift](auto& drawable)
            { drawable.translate(shift); });
    }

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    void DrawableCollection<Base, Sequence>::scale(
        Vector2f const& center, float32 factor) noexcept
    {
        std::ranges::for_each(*this, [&center, &factor](auto& drawable)
            { drawable.scale(center, factor); });
    }

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    void DrawableCollection<Base, Sequence>::rotate(
        Vector2f const& center, float32 angle) noexcept
    {
        std::ranges::for_each(*this, [&center, &angle](auto& drawable)
            { drawable.rotate(center, angle); });
    }

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    void DrawableCollection<Base, Sequence>::rotate(
        Vector2f const& center, Matrix2f const& rotation) noexcept
    {
        std::ranges::for_each(*this, [&center, &rotation](auto& drawable)
            { drawable.rotate(center, rotation); });
    }

}
