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

#include <MPGL/Exceptions/CircularListEmptyException.hpp>
#include <MPGL/Traits/Concepts.hpp>

#include <optional>
#include <memory>

namespace mpgl {

    /**
     * The list with last node pointing the first node and vice
     * versa
     *
     * @tparam Tp the element type
     * @tparam Alloc the allocator type
     */
    template <PureType Tp, Allocator<Tp> Alloc = std::allocator<Tp>>
    class CircularList {
    public:
        typedef Tp                                  value_type;
        typedef Tp*                                 pointer;
        typedef Tp const*                           const_pointer;
        typedef Tp&                                 reference;
        typedef Tp const&                           const_reference;
        typedef std::optional<Tp>                   optional;
    private:
        /**
         * Represents the node in the list
         */
        struct Node {
            typedef Node*                           NodePtr;

            value_type                              value;
            NodePtr                                 nextNode;
            NodePtr                                 lastNode;

            /**
             * Constructs a new node object from the given
             * pointers to last and next node and the stored
             * value's initialization arguments
             *
             * @tparam Args the arguments types
             * @param lastNode the pointer to the last node
             * @param nextNode the pointer to the next node
             * @param args the value's constructor arguments
             */
            template <typename... Args>
                requires std::constructible_from<value_type, Args...>
            constexpr explicit Node(
                NodePtr lastNode,
                NodePtr nextNode,
                Args&&... args)
                    : value{std::forward<Args>(args)...},
                    nextNode{nextNode}, lastNode{lastNode} {}

            /**
             * Constructs a new node object from the given
             * stored value's initialization arguments
             *
             * @tparam Args the arguments types
             * @param args the value's constructor arguments
             */
            template <typename... Args>
                requires std::constructible_from<value_type, Args...>
            constexpr explicit Node(Args&&... args)
                    : value{std::forward<Args>(args)...},
                    nextNode{nullptr}, lastNode{nullptr} {}

            /**
             * Destroys the node object
             */
            constexpr ~Node(void) noexcept = default;
        };
    public:
        /**
         * Constructs a new circular list object with the given
         * allocator
         *
         * @param alloc the constant reference to the allocator
         * object
         */
        constexpr CircularList(Alloc const& alloc = {}) noexcept
            : alloc{alloc}, sentinel{nullptr} {}

        /**
         * Constructs a new circular list object with the given
         * allocator. Copies the given range elements into the
         * circular list object
         *
         * @tparam Range the range type
         * @param range the universal reference to the range object
         * @param alloc the constant reference to the allocator
         * object
         */
        template <std::ranges::input_range Range>
        constexpr CircularList(
            Range&& range,
            Alloc const& alloc = {});

        /**
         * Constructs a new circular list object with the given
         * allocator. Copies the given range elements into the
         * circular list object
         *
         * @tparam Iter the iterator type
         * @tparam Sent the sentinel type
         * @param iter the iterator to the begining of the range
         * @param end the sentinel to the end of the range
         * @param alloc the constant reference to the allocator
         * object
         */
        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent>
        constexpr CircularList(
            Iter iter,
            Sent const& end,
            Alloc const& alloc = {});

        /**
         * Constructs a new circular list object from the given
         * constant reference to the circular list object
         *
         * @param list the constant reference to the circular list
         * object
         */
        constexpr CircularList(CircularList const& list);

        /**
         * Constructs a new circular list object from the given
         * rvalue reference to the circular list object
         *
         * @param list the rvalue reference to the circular list
         * object
         */
        constexpr CircularList(CircularList&& list) noexcept;

        /**
         * Assigns the given circular list object to this object
         *
         * @param list the constant reference to the circular
         * list object
         * @return the reference to this object
         */
        constexpr CircularList& operator= (
            CircularList const& list);

        /**
         * Assigns the given circular list object to this object
         *
         * @param list the rvalue reference to the circular
         * list object
         * @return the reference to this object
         */
        constexpr CircularList& operator= (
            CircularList&& list) noexcept;

        /**
         * Iterates through the circular list
         *
         * @tparam IterTp the iterator's value type
         */
        template <typename IterTp>
        class Iterator {
        public:
            using value_type                      = IterTp;
            using reference                       = IterTp&;
            using pointer                         = IterTp*;
            using difference_type                 = std::ptrdiff_t;
            using iterator_category               = std::bidirectional_iterator_tag;

            /**
             * Constructs a new iterator object from the given
             * pointer to node
             *
             * @param iter the pointer to the node
             */
            constexpr explicit Iterator(Node* iter) noexcept
                : iter{iter} {}

            /**
             * Constructs a new iterator object
             */
            constexpr explicit Iterator(void) noexcept = default;

            /**
             * Increments iterator by one
             *
             * @return reference to this object
             */
            constexpr Iterator& operator++(void) noexcept
                { iter = iter->nextNode; return *this; }

            /**
             * Post-increments iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr Iterator operator++(int) noexcept
                { auto temp = *this; ++(*this); return temp; }

            /**
             * Decrements iterator by one
             *
             * @return reference to this object
             */
            constexpr Iterator& operator--(void) noexcept
                { iter = iter->lastNode; return *this; }

            /**
             * Post-decrements iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr Iterator operator--(int) noexcept
                { auto temp = *this; --(*this); return temp; }

            /**
             * Returns a reference to the node's value
             *
             * @return the reference to the node's value
             */
            [[nodiscard]] constexpr reference operator*(
                void) const noexcept
                    { return iter->value; }

            /**
             * Returns a pointer to the node's value
             *
             * @return the pointer to the node's value
             */
            [[nodiscard]] constexpr pointer operator->(
                void) const noexcept
                    { return &iter->value; }

            /**
             * Checks whether two iterators are equal
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return whether two iterators are equal
             */
            [[nodiscard]] friend constexpr bool operator==(
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

        /**
         * Returns an iterator to the root node
         *
         * @return the iterator to the root node
         */
        [[nodiscard]] constexpr iterator attachment(void) noexcept
            { return iterator{sentinel}; }

        /**
         * Returns a constant iterator to the root node
         *
         * @return the constant iterator to the root node
         */
        [[nodiscard]] constexpr const_iterator attachment(
            void) const noexcept
                { return const_iterator{sentinel}; }

        /**
         * Returns a constant iterator to the root node
         *
         * @return the constant iterator to the root node
         */
        [[nodiscard]] constexpr const_iterator cattachment(
            void) const noexcept
                { return const_iterator{sentinel}; }

        /**
         * Returns a reverse iterator to the root node
         *
         * @return the reverse iterator to the root node
         */
        [[nodiscard]] constexpr reverse_iterator rattachment(
            void) noexcept
                { return reverse_iterator{sentinel}; }

        /**
         * Returns a reverse constant iterator to the root node
         *
         * @return the reverse constant iterator to the root node
         */
        [[nodiscard]] constexpr const_reverse_iterator rattachment(
            void) const noexcept
                { return const_reverse_iterator{sentinel}; }

        /**
         * Returns a reverse constant iterator to the root node
         *
         * @return the reverse constant iterator to the root node
         */
        [[nodiscard]] constexpr const_reverse_iterator crattachment (
            void) const noexcept
                { return const_reverse_iterator{sentinel}; }

        /**
         * Returns whether the circular list is empty
         *
         * @return if the circular list is empty
         */
        [[nodiscard]] constexpr bool empty(void) const noexcept
            { return sentinel == nullptr; }

        /**
         * Pushes a value into the circular list
         *
         * @param value the constant reference to the pushed object
         * @return the iterator to the newly created node
         */
        constexpr iterator push(const_reference value);

        /**
         * Pushes a value into the circular list
         *
         * @param value the rvalue reference to the pushed object
         * @return the iterator to the newly created node
         */
        constexpr iterator push(value_type&& value);

        /**
         * Emplaces a value into the circular list
         *
         * @tparam Args the arguments types
         * @param args the value's constructor arguments
         * @return the iterator to the newly created node
         */
        template <typename... Args>
            requires std::constructible_from<value_type, Args...>
        constexpr iterator emplace(Args&&... args);

        /**
         * Returns the value of the root node and pops it
         *
         * @throw CircularListEmptyException if the circular
         * list is empty
         * @return the value of the root node
         */
        [[nodiscard]] constexpr value_type pop(void);

        /**
         * Returns an optional with the value of the root node
         * and pops it. If the circular list is empty then
         * returns an empty optional
         *
         * @return the optional with the root node's value
         */
        [[nodiscard]] constexpr optional tryPop(void) noexcept;

        /**
         * Swaps the content of two circular lists
         *
         * @param list the reference to the circular list object
         */
        constexpr void swap(CircularList& list) noexcept
            { std::swap(sentinel, list.sentinel); }

        /**
         * Destroys the circular list
         */
        constexpr ~CircularList(void) noexcept;
    private:
        using TpAllocTraits = std::allocator_traits<Alloc>;
        using NodeAllocType = typename TpAllocTraits::template rebind_alloc<Node>;
        using NodeAllocTraits = typename TpAllocTraits::template rebind_traits<Node>;
        using NodePtr = typename Node::NodePtr;

        /**
         * Constructs a new node from the given arguments
         *
         * @tparam Args the arguments types
         * @param args the node's constructor arguments
         * @return the pointer to newly created node
         */
        template <typename... Args>
        constexpr NodePtr construct(Args&&... args);

        /**
         * Destroys the given node
         *
         * @param node the pointer to node
         */
        constexpr void destroy(NodePtr node) noexcept;

        /**
         * Adds the new node to the circular list
         *
         * @tparam Args the arguments types
         * @param args the node's constructor arguments
         */
        template <typename... Args>
        constexpr void addNode(Args&&... args);

        /**
         * Creates a new circular list object from the given
         * iterator to other circular list
         *
         * @param iter the iterator to other circular list object
         */
        constexpr void buildListFromList(const_iterator iter);

        /**
         * Pops the root node and returns its value
         *
         * @return the root node's value
         */
        constexpr value_type popRoot(void) noexcept;

        /**
         * Explicitly destroys inner circular list but does not
         * provide information to the compiler about end of the
         * object's lifetime. Object can safely be used after this
         * operation unlike the normal destructor call
         */
        constexpr void destroy(void) noexcept;

        [[no_unique_address]] NodeAllocType         alloc;
        NodePtr                                     sentinel;
    };

}

#include <MPGL/Collections/CircularList.tpp>
