#pragma once

#include <vector>
#include <concepts>
#include <algorithm>

#include "../Transformations/Transformable2D.hpp"
#include "../../Mathematics/Systems.hpp"
#include "../Drawable.hpp"
#include "Views.hpp"

namespace ge {

    template <class Tp>
    concept DrawableType = std::derived_from<Tp, Drawable>
        && std::derived_from<Tp, ScreenTransformationEvent>;

    template <class Tp>
    concept TransformableDrawable = DrawableType<Tp>
        && std::derived_from<Tp, Transformable2D>;

    template <DrawableType Base,
        std::ranges::input_range Sequence = std::vector<Base>>
    class DrawableCollectionBase : public Sequence, public Drawable,
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

        template <typename Signature, typename... Args>
            requires std::invocable<Signature, Base, Args...>
        void call(Signature signature, Args&&... args) noexcept(
            std::is_nothrow_invocable_v<Signature, Base, Args...>);

        void copyToGPU(void) const noexcept final;
        void draw(void) const noexcept final;

        void onScreenTransformation(Vector2ui const& oldDimmensions
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
        void scale(Vector2f const& center, float factor) noexcept final;
        void rotate(Vector2f const& center, float angle) noexcept final;
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
    void DrawableCollectionBase<Base, Sequence>::copyToGPU(void) const noexcept {
        std::ranges::for_each(*this, [](auto const& drawable)
            { drawable.copyToGPU(); });
    }

    template <DrawableType Base, std::ranges::input_range Sequence>
    void DrawableCollectionBase<Base, Sequence>::draw(void) const noexcept {
        std::ranges::for_each(*this, [](auto const& drawable)
            { drawable.draw(); });
    }

    template <DrawableType Base, std::ranges::input_range Sequence>
    void DrawableCollectionBase<Base, Sequence>::onScreenTransformation(
        Vector2ui const& oldDimmensions) noexcept
    {
        std::ranges::for_each(*this, [&oldDimmensions](auto& drawable)
            { drawable.onScreenTransformation(oldDimmensions); });
    }

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    void DrawableCollection<Base, Sequence>::translate(Vector2f const& shift) noexcept {
        std::ranges::for_each(*this, [&shift](auto& drawable)
            { drawable.translate(shift); });
    }

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    void DrawableCollection<Base, Sequence>::scale(
        Vector2f const& center, float factor) noexcept
    {
        std::ranges::for_each(*this, [&center, &factor](auto& drawable)
            { drawable.scale(center, factor); });
    }

    template <TransformableDrawable Base, std::ranges::input_range Sequence>
    void DrawableCollection<Base, Sequence>::rotate(
        Vector2f const& center, float angle) noexcept
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
