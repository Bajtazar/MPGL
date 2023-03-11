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

namespace mpgl {

    template <PureType Tp, Allocator<Tp> Alloc>
    template <typename... Args>
    constexpr CircularList<Tp, Alloc>::NodePtr
        CircularList<Tp, Alloc>::construct(Args&&... args)
    {
        NodePtr node = NodeAllocTraits::allocate(alloc, 1);
        NodeAllocTraits::construct(alloc, node,
            std::forward<Args>(args)...);
        return node;
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr void CircularList<Tp, Alloc>::destroy(
        NodePtr node) noexcept
    {
        NodeAllocTraits::destroy(alloc, node);
        NodeAllocTraits::deallocate(alloc, node, 1);
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    template <typename... Args>
    constexpr void CircularList<Tp, Alloc>::addNode(
            Args&&... args)
    {
        NodePtr node = construct(sentinel, sentinel->nextNode,
            std::forward<Args>(args)...);
        sentinel = sentinel->nextNode =
            sentinel->nextNode->lastNode = node;
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::iterator
        CircularList<Tp, Alloc>::push(const_reference value)
    {
        if (empty()) {
            NodePtr node = construct(value);
            sentinel = node->lastNode =
                node->nextNode = node;
        } else
            addNode(value);
        return iterator{ sentinel };
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::iterator
        CircularList<Tp, Alloc>::push(value_type&& value)
    {
        if (empty()) {
            NodePtr node = construct(std::move(value));
            sentinel = node->lastNode =
                node->nextNode = node;
        } else
            addNode(std::move(value));
        return iterator{ sentinel };
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    template <typename... Args>
        requires std::constructible_from<Tp, Args...>
    constexpr CircularList<Tp, Alloc>::iterator
        CircularList<Tp, Alloc>::emplace(Args&&... args)
    {
        if (empty()) {
            NodePtr node = construct(std::forward<Args>(args)...);
            sentinel = node->lastNode =
                node->nextNode = node;
        } else
            addNode(std::forward<Args>(args)...);
        return iterator{ sentinel };
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::value_type
        CircularList<Tp, Alloc>::popRoot(void) noexcept
    {
        value_type value = std::move(sentinel->value);
        NodePtr node = sentinel;
        if (sentinel->nextNode != sentinel) {
            node->lastNode->nextNode = node->nextNode;
            node->nextNode->lastNode = node->lastNode;
            sentinel = node->lastNode;
        } else
            sentinel = nullptr;
        destroy(node);
        return value;
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    [[nodiscard]] constexpr CircularList<Tp, Alloc>::value_type
        CircularList<Tp, Alloc>::pop(void)
    {
        if (empty())
            throw CircularListEmptyException{};
        return popRoot();
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    [[nodiscard]] constexpr CircularList<Tp, Alloc>::optional
        CircularList<Tp, Alloc>::tryPop(void) noexcept
    {
        if (empty())
            return {};
        return { popRoot() };
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    template <std::input_iterator Iter,
        std::sentinel_for<Iter> Sent>
    constexpr CircularList<Tp, Alloc>::CircularList(
        Iter iter,
        Sent const& end,
        Alloc const& alloc)
            : alloc{alloc}
    {
        if (iter == end)
            return;
        sentinel = construct(*iter++);
        sentinel->lastNode = sentinel->nextNode = sentinel;
        for (; iter != end; ++iter)
            addNode(*iter);
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr void CircularList<Tp, Alloc>::buildListFromList(
        const_iterator iter)
    {
        auto const sent = ++iter;
        sentinel = construct(*iter++);
        sentinel->lastNode = sentinel->nextNode = sentinel;
        if (iter == sent)
            return;
        for (; iter != sent; ++iter)
            addNode(*iter);
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    template <std::ranges::input_range Range>
    constexpr CircularList<Tp, Alloc>::CircularList(
        Range&& range,
        Alloc const& alloc)
            : CircularList{std::ranges::begin(range),
            std::ranges::end(range), alloc} {}

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::CircularList(
        CircularList const& list) : sentinel{nullptr}
    {
        if (list.empty())
            return;
        buildListFromList(list.attachment());
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::CircularList(
        CircularList&& list) noexcept : sentinel{list.sentinel}
    {
        list.sentinel = nullptr;
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>&
        CircularList<Tp, Alloc>::operator=(
            CircularList const& list)
    {
        destroy();
        if (sentinel = nullptr; list.empty())
            return *this;
        buildListFromList(list.attachment());
        return *this;
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>&
        CircularList<Tp, Alloc>::operator=(
            CircularList&& list) noexcept
    {
        destroy();
        sentinel = list.sentinel;
        list.sentinel = nullptr;
        return *this;
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::~CircularList(
        void) noexcept
    {
        destroy();
    }

    template <PureType Tp, Allocator<Tp> Alloc>
    constexpr void CircularList<Tp, Alloc>::destroy(void) noexcept {
        if (!sentinel) return;
        auto iter = iterator{ sentinel->nextNode };
        auto const end = iterator{ sentinel };
        while (iter != end)
           destroy((iter++).iter);
        destroy(sentinel);
    }

}
