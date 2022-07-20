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

#include <MPGL/Traits/Concepts.hpp>

#include <iterator>
#include <memory>

namespace mpgl::any {

    /**
     * Checks whether the given type can be used in the
     * mpgl::any::InputRange
     *
     * @tparam Range the range's type
     * @tparam Tp the InputRange's element type
     */
    template <class Range, typename Tp>
    concept InputRangeCompatible =
        std::ranges::input_range<Range> &&
        std::same_as<Tp, std::ranges::range_value_t<Range>> &&
        std::copy_constructible<Range> &&
        std::is_copy_assignable_v<Range>;

    /**
     * Container and wrapper that stores ranges with at least input
     * iterator and gives an unified interface to use them. Input
     * range allows to store mutliple different types which only
     * has to obey the std::input_range concept and be copy
     * constructible and assignable. Thus it can be used in places
     * when templates cannot be used but the type generalization is
     * needed. Input range can be used as a wrapper to already
     * existing range - to do this only a reference to the range is
     * required; or it can store an object within itself - then the
     * desired object has to be moved inside. It cannot be defaultly
     * initialized. This class works heavely on virtual methods thus
     * it should be used wisely where performance is important
     *
     * @tparam Tp the type of range's elements
     */
    template <PureType Tp>
    class InputRange {
    public:
        typedef Tp&                         reference;
        typedef Tp const&                   const_reference;

        /**
         * Constructs a new Input Range object handling or owning
         * the given Input-Range-compatible range
         *
         * @param range the universal reference to the compatible
         * range (if it is a lvalue reference then InputRange holds a
         * reference - if it is a rvalue reference then InputRange
         * holds an object)
         */
        explicit InputRange(InputRangeCompatible<Tp> auto&& range);

        /**
         * Constructs a new Input Range object. Creates a copy of
         * the other's underlying object if the copied range manages
         * one. Otherwise copies a reference managed by other input
         * range
         *
         * @param inputRange the constant reference to the input
         * range object
         */
        InputRange(InputRange const& inputRange);

        InputRange(InputRange&&) = default;

        /**
         * Assigns an other Input Range object. Assigns a copy of
         * the other's underlying object if the copied range manages
         * one. Otherwise assigns a reference managed by other input
         * range
         *
         * @param inputRange the constant reference to the input
         * range object
         * @return the reference to this object
         */
        InputRange& operator=(InputRange const& inputRange);

        InputRange& operator=(InputRange&&) = default;

        /**
         * Assigns the given Input-Range-compatible range
         *
         * @param range the universal reference to the compatible
         * range (if it is a lvalue reference then InputRange holds a
         * reference - if it is a rvalue reference then InputRange
         * holds an object)
         * @return the reference to this object
         */
        InputRange& operator=(InputRangeCompatible<Tp> auto&& range);

        /**
         * Sentinel for the Input Range iterator
         */
        class sentinel {};

        /**
         * The input iterator that allows to interact with the
         * wrapped range. Acts as a facade to the underlying
         * type erasuring system. This class works heavely on virtual
         * methods thus it should be used wisely where performance
         * is important
         *
         * @tparam BaseTp the type used by the iterator
         */
        template <typename BaseTp>
        class Iterator {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = BaseTp;
            using difference_type = std::ptrdiff_t;
            using pointer = BaseTp*;
            using reference = BaseTp&;

            using UnderlyingIter =
                details::WrappedRangeIteratorBase<Tp>;
            using UnderlyingPtr = std::unique_ptr<UnderlyingIter>;

            /**
             * Constructs a new Iterator object
             *
             * @param pointer the rvalue reference to the smart
             * pointer managing the erasured iterator
             */
            explicit Iterator(UnderlyingPtr&& pointer) noexcept
                : iterPtr{std::move(pointer)} {}

            /**
             * Constructs a new Iterator object
             */
            explicit Iterator(void) noexcept :
                iterPtr{nullptr} {}

            /**
             * Constructs a new Iterator object from the other
             * Iterator object
             *
             * @param iterator the constant reference to the
             * Iterator object
             */
            Iterator(Iterator const& iterator);

            Iterator(Iterator&&) noexcept = default;

            /**
             * Assigns an other Iterator object
             *
             * @param iterator the constant reference to the
             * Iterator object
             * @return the reference to this object
             */
            Iterator& operator=(Iterator const& iterator);

            Iterator& operator=(Iterator&&) = default;

            /**
             * Returns the reference to the range's element
             *
             * @return the reference to the range's element
             */
            [[nodiscard]] reference operator*(void) const noexcept
                { return iterPtr->get(); }

            /**
             * Returns the pointer to the range's element
             *
             * @return the pointer to the range's element
             */
            [[nodiscard]] pointer operator->(void) const noexcept
                { return &(iterPtr->get()); }

            /**
             * Pre-increments an iterator
             *
             * @return the reference to this object
             */
            Iterator& operator++(void) noexcept
                { iterPtr->increment(); return *this; }

            /**
             * Post-increments an iterator
             *
             * @return a copy of the pre-incremented iterator
             */
            [[nodiscard]] Iterator operator++(int);

            /**
             * Swaps two iterators toghether
             *
             * @param other the reference to the other iterator
             * object
             */
            void swap(Iterator& other) noexcept
                { iterPtr.swap(other.iterPtr); }

            /**
             * Compares the iterator to the sentinel
             *
             * @param iter the constant reference to the iterator
             * @param sent the constant reference to the sentinel
             * @return true if the iterator has reached its end
             * otherwise returns false
             */
            friend [[nodiscard]] bool operator==(
                Iterator const& iter,
                [[maybe_unused]] sentinel const& sent) noexcept
                    { return !iter.iterPtr || !iter.iterPtr->hasNext(); }

            /**
             * Compares the iterator to the sentinel
             *
             * @param sent the constant reference to the sentinel
             * @param iter the constant reference to the iterator
             * @return true if the iterator has reached its end
             * otherwise returns false
             */
            friend [[nodiscard]] bool operator==(
                [[maybe_unused]] sentinel const& sent,
                Iterator const& iter) noexcept
                    { return iter == sent; }
        private:
            UnderlyingPtr                   iterPtr;
        };

        using iterator = Iterator<Tp>;
        using const_iterator = Iterator<Tp const>;

        /**
         * Returns an iterator to the begining of the range
         *
         * @return the iterator to the begining of the range
         */
        [[nodiscard]] iterator begin(void)
            { return iterator{rangePointer->iterator()}; }

        /**
         * Returns a constant iterator to the begining of the range
         *
         * @return the constant iterator to the begining of the range
         */
        [[nodiscard]] const_iterator begin(void) const
            { return const_iterator{rangePointer->iterator()}; }

        /**
         * Returns a constant iterator to the begining of the range
         *
         * @return the constant iterator to the begining of the range
         */
        [[nodiscard]] const_iterator cbegin(void) const
            { return const_iterator{rangePointer->iterator()}; }

        /**
         * Returns the sentinel (signalizes end of the range)
         *
         * @return the sentinel
         */
        [[nodiscard]] sentinel end(void) const noexcept
            { return {}; }

        /**
         * Returns the sentinel (signalizes end of the range)
         *
         * @return the sentinel
         */
        [[nodiscard]] sentinel cend(void) const noexcept
            { return {}; }

        /**
         * Swaps to Input Range objects
         *
         * @param other the reference to the other object
         */
        void swap(InputRange& other) noexcept
            { rangePointer.swap(other.rangePointer); }

        /**
         * Destroys the Input Range object
         */
        ~InputRange(void) noexcept = default;
    private:
        /**
         * An interface for the RangeWrapper. Allows to abstract
         * the ranges
         */
        class RangeInterface {
        public:
            /**
             * Constructs a new Range Interface object
             */
            explicit RangeInterface(void) noexcept = default;

            /**
             * An interface for the RangeWrapper's iterator. Creates
             * an abstract platform for the different range's
             * iterators
             */
            class IteratorInterface {
            public:
                /**
                 * Constructs a new Iterator Interface object
                 */
                explicit IteratorInterface(void) noexcept = default;

                using reference = Tp&;

                /**
                 * Pure virtual method. Has to be overloaded. Checks
                 * whether there is a next element in the iterator
                 *
                 * @return if there is a next element in the iterator
                 */
                [[nodiscard]] virtual bool hasNext(
                    void) const noexcept = 0;

                /**
                 * Pure virtual method. Has to be overloaded.
                 * Increments an iterator
                 */
                virtual void increment(void) noexcept = 0;

                /**
                 * Pure virtual method. Has to be overloaded. Returns
                 * a reference to the element in the range
                 *
                 * @return the reference to the element in the range
                 */
                [[nodiscard]] virtual reference get(
                    void) const noexcept = 0;

                /**
                 * Pure virtual method. Has to be overloaded. Clones
                 * the iterator and returns a pointer to the cloned
                 * object casted to the interface pointer
                 *
                 * @return the pointer to the cloned object
                 */
                [[nodiscard]] virtual IteratorInterface*
                    clone(void) = 0;

                /**
                 * Virtual destructor. Destroys the Iterator
                 * Interface object
                 */
                virtual ~IteratorInterface(
                    void) noexcept = default;
            protected:
                IteratorInterface(
                    IteratorInterface const&) noexcept = default;
                IteratorInterface(
                    IteratorInterface&&) noexcept = default;

                IteratorInterface& operator=(
                    IteratorInterface const&) noexcept = default;
                IteratorInterface& operator=(
                    IteratorInterface&&) noexcept = default;
            };

            using IterPtr = std::unique_ptr<IteratorInterface>;

            /**
             * Pure virtual method. Has to be overloaded. Returns
             * a unique pointer with the iterator pointing begining
             * of the range
             *
             * @return the unique pointer with the iterator pointing
             * begining of the range
             */
            [[nodiscard]] virtual IterPtr iterator(void) noexcept = 0;

            /**
             * Pure virtual method. Has to be overloaded. Clones
             * the range and returns a pointer to the cloned
             * object casted to the interface pointer
             *
             * @return the pointer to the cloned object
             */
            [[nodiscard]] virtual RangeInterface* clone(void) = 0;

            /**
             * Virtual destructor. Destroys the Range Interface object
             */
            virtual ~RangeInterface(void) noexcept = default;
        protected:
            RangeInterface(
                RangeInterface const&) noexcept = default;

            RangeInterface(RangeInterface&&) noexcept = default;

            RangeInterface& operator=(
                RangeInterface const&) noexcept = default;

            RangeInterface& operator=(
                RangeInterface&&) noexcept = default;
        };

        /**
         * Wrapps the input range. Allows to use dynamic
         * polymorphism on the given range via the Range Interface
         *
         * @tparam Range the wrapped range type
         */
        template <std::ranges::input_range Range>
        class WrappedRange : public RangeInterface {
        public:
            /**
             * Constructs a new Wrapped Range object
             *
             * @param range the universal reference to the range
             * object
             */
            explicit WrappedRange(Range&& range)
                : range{std::forward<Range>(range)} {}

            WrappedRange(WrappedRange const&) = default;
            WrappedRange(WrappedRange&&) = default;

            WrappedRange& operator=(
                WrappedRange const&) = default;
            WrappedRange& operator=(
                WrappedRange&&) = default;

            using IIter = RangeInterface::IteratorInterface;

            /**
             * Iterator used by wrapped range. It allows to be used
             * in dynamic context via the iterator interface
             */
            class Iterator : public IIter {
            public:
                using RangeIter = std::ranges::iterator_t<Range>;
                using RangeSent = std::ranges::sentinel_t<Range>;
                using reference =
                    = typename IIter::reference;

                /**
                 * Constructs a new Iterator object from a range's
                 * iterator and sentinel pair
                 *
                 * @param iterator the constant reference to the
                 * iterator object
                 * @param sentinel the constant reference to the
                 * sentinel object
                 */
                explicit Iterator(
                    RangeIter const& iterator,
                    RangeSent const& sentinel)
                        : iter{iterator}, sent{sentinel} {}

                /**
                 * Constructs a new Iterator object
                 */
                explicit Iterator(void) = default;

                Iterator(Iterator const&) = default;
                Iterator(Iterator&&) = default;

                Iterator& operator=(Iterator const&) = default;
                Iterator& operator=(Iterator&&) = default;

                /**
                 * Checks whether there is a next element in the
                 * iterator
                 *
                 * @return if there is a next element in the iterator
                 */
                [[nodiscard]] bool hasNext(
                    void) const noexcept final
                        { return iter != sent; }

                /**
                 * Increments an iterator
                 */
                void increment(void) noexcept final
                    { ++iter; }

                /**
                 * Returns a reference to the element in the range
                 *
                 * @return the reference to the element in the range
                 */
                [[nodiscard]] reference get(void) const noexcept
                    { return *iter; }

                /**
                 * Clones the iterator and returns a pointer to the
                 * cloned object casted to the interface pointer
                 *
                 * @return the pointer to the cloned object
                 */
                [[nodiscard]] IIter* clone(void) final;

                /**
                 * Destroys the Iterator object
                 */
                ~Iterator(void) noexcept = default;
            private:
                RangeIter                       iter;
                RangeSent                       sent;
            };

            using IterPtr = typename RangeInterface::IterPtr;

            /**
             * Clones the range and returns a pointer to the cloned
             * object casted to the interface pointer
             *
             * @return the pointer to the cloned object
             */
            [[nodiscard]] RangeInterface* clone(void) final
                { return new WrappedRange{*this}; }

            /**
             * Returns a unique pointer with the iterator pointing
             * begining of the range
             *
             * @return the unique pointer with the iterator pointing
             * begining of the range
             */
            [[nodiscard]] IterPtr iterator(void) noexcept final;

            /**
             * Destroys the Wrapped Range object
             */
            ~WrappedRange(void) noexcept = default;
        private:
            Range                           range;
        };

        typedef std::unique_ptr<
            RangeInterface>                 InterfacePtr;

        InterfacePtr                        rangePointer;
    };

}

namespace std {

    /**
     * Swaps two mpgl::any::InputRange objects
     *
     * @tparam Tp the range element's type
     * @param left the reference to the left object
     * @param right the reference to the right object
     */
    template <mpgl::PureType Tp>
    void swap(
        mpgl::any::InputRange<Tp>& left,
        mpgl::any::InputRange<Tp>& right) noexcept
    {
        left.swap(right);
    }

    /**
     * Swaps two mpgl::any::InputRange::iterator objects
     *
     * @tparam Tp the range element's type
     * @param left the reference to the left object
     * @param right the reference to the right object
     */
    template <mpgl::PureType Tp>
    void swap(
        typename mpgl::any::InputRange<Tp>::iterator& left,
        typename mpgl::any::InputRange<Tp>::iterator& right
        ) noexcept
    {
        left.swap(right);
    }

    /**
     * Swaps two mpgl::any::InputRange::const_iterator objects
     *
     * @tparam Tp the range element's type
     * @param left the reference to the left object
     * @param right the reference to the right object
     */
    template <mpgl::PureType Tp>
    void swap(
        typename mpgl::any::InputRange<Tp>::const_iterator& left,
        typename mpgl::any::InputRange<Tp>::const_iterator& right
        ) noexcept
    {
        left.swap(right);
    }

}

#include <MPGL/Collections/Erasers/InputRange.tpp>
