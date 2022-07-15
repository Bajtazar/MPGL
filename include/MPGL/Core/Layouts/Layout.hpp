/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include <MPGL/Core/Figures/Figure.hpp>

#include <unordered_set>

namespace mpgl {

    template <std::derived_from<Transformable2D> Tp>
    class Layout : public ScreenTransformationEvent {
    public:
        typedef Tp*                             pointer;

        struct LayoutPair {
            Vector2u                            hook;
            pointer                             transformable;
        };

        explicit Layout(void) = noexcept;

        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent>
                requires std::convertible_to<std::iter_value_t<Iter>,
                    LayoutPair>
        explicit Layout(Iter first, Sent const& last);

        template <std::ranges::input_range Range>
            requires std::convertible_to<std::ranges::range_value_t<
                Range>, LayoutPair>
        explicit Layout(Range&& range);

        Layout(std::initializer_list<LayoutPair> init);

        virtual void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept = 0;

        [[nodiscard]] bool empty(void) const noexcept
            { return shapes.empty(); }

        [[nodiscard]] std::size_t size(void) const noexcept
            { return shapes.size(); }

        [[nodiscard]] std::size_t maxSize(void) const noexcept
            { return shapes.max_size(); }

        void clear(void)
            { shapes.clear(); }
    protected:
        struct Hash {
            [[nodiscard]] size_t operator() (
                LayoutPair const& layoutPair) const noexcept;
        };

        struct EqualTo {
            [[nodiscard]] bool operator() (
                LayoutPair const& left,
                LayoutPair const& right) const noexcept;
        };

        typedef std::unordered_set<
            LayoutPair, Hash, EqualTo>          UnorderedSet;
    public:
        typedef UnorderedSet::iterator          iterator;
        typedef UnorderedSet::const_iterator    const_iterator;

        [[nodiscard]] iterator begin(void) noexcept
            { return shapes.begin(); }

        [[nodiscard]] iterator end(void) noexcept
            { return shapes.end(); }

        [[nodiscard]] const_iterator begin(void) const noexcept
            { return shapes.begin(); }

        [[nodiscard]] const_iterator end(void) const noexcept
            { return shapes.end(); }

        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return shapes.cbegin(); }

        [[nodiscard]] const_iterator cend(void) const noexcept
            { return shapes.cend(); }

        typedef std::pair<iterator, bool>       InsertResult;

        InsertResult insert(LayoutPair const& pair)
            { return shapes.insert(pair); }

        InsertResult insert(LayoutPair&& pair)
            { return shapes.insert(std::move(pair)); }

        InsertResult insert(Vector2u const& hook, pointer pointer)
            { return shapes.insert(LayoutPair{hook, pointer}); }

        iterator insert(const_iterator hint, LayoutPair const& pair)
            { return shapes.insert(hint, pair); }

        iterator insert(const_iterator hint, LayoutPair&& pair)
            { return shapes.insert(hint, std::mov(pair)); }

        iterator insert(
            const_iterator hint,
            Vector2u const& hook,
            pointer pointer);

        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent>
                requires std::convertible_to<std::iter_value_t<Iter>,
                    LayoutPair>
        iterator insert(Iter first, Sent const& last);

        template <std::ranges::input_range Range>
            requires std::convertible_to<std::ranges::range_value_t<
                Range>, LayoutPair>
        iterator insert(Range&& range);

        template <typename... Args>
            requires std::constructible_from<LayoutPair, Args...>
        InsertResult emplace(Args&&... args);

        template <typename... Args>
        iterator emplaceHint(const_iterator hint, Args&&... args);

        iterator erase(iterator position)
            { return shapes.erase(position); }

        iterator erase(const_iterator position)
            { return shapes.erase(position); }

        iterator erase(const_iterator first, const_iterator last)
            { return shapes.erase(first, last); }

        std::size_t erase(pointer pointer)
            { return shapes.erase(LayoutPair{{}, pointer}); }

        [[nodiscard]] iterator find(pointer pointer)
            { return shapes.find(LayoutPair{{}, pointer}); }

        [[nodiscard]] const_iterator find(pointer pointer) const
            { return shapes.find(LayoutPair{{}, pointer}); }

        [[nodiscard]] bool contains(pointer pointer) const
            { return shapes.contains(LayoutPair{{}, pointer}); }

        [[nodiscard]] float32 loadFactor(void) const noexcept
            { return shapes.load_factor(); }

        [[nodiscard]] float32 maxLoadFactor(void) const noexcept
            { return shapes.max_load_factor(); }

        void maxLoadFactor(float32 factor)
            { shapes.max_load_factor(factor); }

        void rehash(std::size_t count)
            { shapes.rehash(count); }

        void reserve(std::size_t count)
            { shapes.reserve(count); }

        virtual ~Layout(void) noexcept = default;
    protected:
        Layout(Layout const&) = default;
        Layout(Layout&&) noexcept = default;

        Layout& operator=(Layout const&) = default;
        Layout& operator=(Layout&&) noexcept = default;

        UnorderedSet                            shapes;
    };

    typedef Layout<Figure>                      FigureLayout;

}
