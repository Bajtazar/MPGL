#pragma once

#include "../Exceptions/CircularListEmptyException.hpp"
#include "../Traits/Concepts.hpp"

namespace ge {

    template <NotReference Tp, Allocator<Tp> Alloc = std::allocator<Tp>>
    class CircularList {
    public:
        typedef Tp                                  value_type;
        typedef Tp*                                 pointer;
        typedef Tp const*                           const_pointer;
        typedef Tp&                                 reference;
        typedef Tp const&                           const_reference;
    private:
        struct Node {
            typedef Node*                           NodePtr;

            value_type                              value;
            NodePtr                                 nextNode;
            NodePtr                                 lastNode;

            constexpr explicit Node(
                const_reference value) noexcept
                    : nextNode{nullptr}, lastNode{nullptr},
                    value{value} {}
            constexpr explicit Node(value_type&& value) noexcept
                : nextNode{nullptr}, lastNode{nullptr},
                value{value} {}
            constexpr explicit Node(const_reference value,
                NodePtr lastNode, NodePtr nextNode) noexcept
                    : nextNode{nextNode}, lastNode{lastNode},
                    value{value} {}
            constexpr explicit Node(value_type&& value,
                NodePtr lastNode, NodePtr nextNode) noexcept
                    : nextNode{nextNode}, lastNode{lastNode},
                    value{value} {}
            constexpr ~Node(void) noexcept = default;
        };
    public:
        constexpr CircularList(Alloc const& alloc = {}) noexcept
            : alloc{alloc}, sentinel{nullptr} {}

        template <std::ranges::input_range Range>
        constexpr CircularList(Range&& range,
            Alloc const& alloc = {});

        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent>
        constexpr CircularList(Iter iter,
            Sent const& end, Alloc const& alloc = {});

        constexpr CircularList(CircularList const& list);
        constexpr CircularList(CircularList&& list) noexcept;

        constexpr CircularList& operator= (
            CircularList const& list);
        constexpr CircularList& operator= (
            CircularList&& list) noexcept;

        template <typename value_type>
        class Iterator : std::iterator<
            std::forward_iterator_tag, value_type>
        {
        public:
            typedef std::forward_iterator_tag       iterator_category;
            typedef value_type&                     reference;
            typedef value_type*                     pointer;

            constexpr explicit Iterator(Node* iter) noexcept
                : iter{iter} {}
            constexpr explicit Iterator(void) noexcept = default;

            constexpr Iterator& operator++(void) noexcept
                { iter = iter->nextNode; return *this; }
            constexpr Iterator operator++(int) noexcept
                { auto temp = *this; ++(*this); return temp; }
            constexpr Iterator& operator--(void) noexcept
                { iter = iter->lastNode; return *this; }
            constexpr Iterator operator--(int) noexcept
                { auto temp = *this; --(*this); return temp; }

            constexpr reference operator*(void) const noexcept
                { return iter->value; }
            constexpr pointer operator->(void) const noexcept
                { return &iter->value; }

            friend constexpr bool operator==(
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter == right.iter; }

            friend class CircularList;
        private:
            Node*                                   iter;
        };

        using iterator =                            Iterator<value_type>;
        using const_iterator =                      Iterator<value_type const>;
        using reverse_iterator =                    std::reverse_iterator<iterator>;
        using const_reverse_iterator =              std::reverse_iterator<const_iterator>;

        constexpr iterator attachment (void) noexcept
            { return iterator{sentinel}; }
        constexpr const_iterator attachment (void) const noexcept
            { return const_iterator{sentinel}; }
        constexpr const_iterator cattachment (void) const noexcept
            { return const_iterator{sentinel}; }

        constexpr reverse_iterator rattachment (void) noexcept
            { return reverse_iterator{sentinel}; }
        constexpr const_reverse_iterator rattachment (
            void) const noexcept
                { return const_reverse_iterator{sentinel}; }
        constexpr const_reverse_iterator crattachment (
            void) const noexcept
                { return const_reverse_iterator{sentinel}; }

        constexpr bool empty(void) const noexcept
            { return sentinel == nullptr; }

        constexpr iterator push(const_reference value);
        constexpr iterator push(value_type&& value);

        template <typename... Args>
            requires std::constructible_from<value_type, Args...>
        constexpr void emplace(Args&&... args);

        constexpr value_type pop(void);

        constexpr void swap(CircularList& list) noexcept
            { std::swap(sentinel, list.sentinel); }

        constexpr ~CircularList(void) noexcept;
    private:
        typedef std::allocator_traits<Alloc>        TpAllocTraits;
        typedef typename
            TpAllocTraits::rebind_alloc<Node>       NodeAllocType;
        typedef typename
            TpAllocTraits::rebind_traits<Node>      NodeAllocTraits;
        typedef typename Node::NodePtr              NodePtr;

        template <typename... Args>
        constexpr NodePtr construct(Args&&... args);
        constexpr void destroy(NodePtr node) noexcept;

        constexpr void addNode(const_reference value);
        constexpr void addNode(value_type&& value);

        constexpr void buildListFromList(const_iterator iter);

        [[no_unique_address]] NodeAllocType         alloc;
        NodePtr                                     sentinel;
    };

    template <NotReference Tp, Allocator<Tp> Alloc>
    template <typename... Args>
    constexpr CircularList<Tp, Alloc>::NodePtr
        CircularList<Tp, Alloc>::construct(Args&&... args)
    {
        NodePtr node = NodeAllocTraits::allocate(alloc, 1);
        NodeAllocTraits::construct(alloc, node,
            std::forward<Args>(args)...);
        return node;
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr void CircularList<Tp, Alloc>::destroy(
        NodePtr node) noexcept
    {
        NodeAllocTraits::destroy(alloc, node);
        NodeAllocTraits::deallocate(alloc, node, 1);
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr void CircularList<Tp, Alloc>::addNode(
            const_reference value)
    {
        NodePtr node = construct(value, sentinel,
            sentinel->nextNode);
        sentinel = sentinel->nextNode =
            sentinel->nextNode->lastNode = node;
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr void CircularList<Tp, Alloc>::addNode(
            value_type&& value)
    {
        NodePtr node = construct(std::move(value),
            sentinel, sentinel->nextNode);
        sentinel = sentinel->nextNode =
            sentinel->nextNode->lastNode = node;
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
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

    template <NotReference Tp, Allocator<Tp> Alloc>
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

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::value_type
        CircularList<Tp, Alloc>::pop(void)
    {
        if (empty())
            throw CircularListEmptyException{};
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

    template <NotReference Tp, Allocator<Tp> Alloc>
    template <std::input_iterator Iter,
        std::sentinel_for<Iter> Sent>
    constexpr CircularList<Tp, Alloc>::CircularList(
        Iter iter, Sent const& end, Alloc const& alloc)
            : alloc{alloc}
    {
        if (iter == end)
            return;
        sentinel = construct(*iter++);
        sentinel->lastNode = sentinel->nextNode = sentinel;
        for (; iter != end; ++iter)
            addNode(*iter);
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
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

    template <NotReference Tp, Allocator<Tp> Alloc>
    template <std::ranges::input_range Range>
    constexpr CircularList<Tp, Alloc>::CircularList(
        Range&& range, Alloc const& alloc)
            : CircularList{std::ranges::begin(range),
            std::ranges::end(range), alloc} {}

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::CircularList(
        CircularList const& list) : sentinel{nullptr}
    {
        if (list.empty())
            return;
        buildListFromList(list.attachment());
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::CircularList(
        CircularList&& list) noexcept : sentinel{list.sentinel}
    {
        list.sentinel = nullptr;
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>&
        CircularList<Tp, Alloc>::operator=(
            CircularList const& list)
    {
        this->~CircularList();
        if (sentinel = nullptr; list.empty())
            return *this;
        buildListFromList(list.attachment());
        return *this;
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>&
        CircularList<Tp, Alloc>::operator=(
            CircularList&& list) noexcept
    {
        this->~CircularList();
        sentinel = list.sentinel;
        list.sentinel = nullptr;
        return *this;
    }

    template <NotReference Tp, Allocator<Tp> Alloc>
    constexpr CircularList<Tp, Alloc>::~CircularList(
        void) noexcept
    {
        if (!sentinel) return;
        auto iter = iterator{ sentinel->nextNode };
        auto const end = iterator{ sentinel };
        while (iter != end)
           destroy((iter++).iter);
        destroy(sentinel);
    }

}
