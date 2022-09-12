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

#include <iterator>
#include <optional>
#include <cstring>
#include <compare>
#include <memory>
#include <vector>

#include <MPGL/Mathematics/Tensors/Vector.hpp>
#include <MPGL/Traits/Concepts.hpp>

namespace mpgl {

    /**
     * Base type for the canva's row base
     *
     * @tparam Tp the type of the canva's pixels
     */
    template <Absolute Tp>
    using CanvaRowBase = std::pair<Tp*, std::reference_wrapper<
        std::size_t>>;

    /**
     * Represents an image consisting of given pixels type
     *
     * @tparam Base the base pixel type
     * @tparam Range the image holding vector
     * @tparam RowRange the rows holding vector
     */
    template <DefaultBaseType Base,
        UnderlyingRange<Base> Range = std::vector<Base>,
        UnderlyingRange<CanvaRowBase<Base>> RowRange
            = std::vector<CanvaRowBase<Base>>>
    class Canva {
    public:
        typedef std::size_t                         size_type;
        typedef Vector2<size_type>                  size_vector;
        typedef Base                                pixel;
    private:
        typedef CanvaRowBase<Base>                  BaseTuple;
        typedef typename RowRange::iterator         RowIter;
        typedef typename RowRange::const_iterator   RowConstIter;
    public:
        /**
         * Constructs a new canva object with the given width
         * and height
         *
         * @param width the width of the canva
         * @param height the height of the canva
         */
        constexpr explicit Canva(size_type width, size_type height);

        /**
         * Constructs a new canva object with the given dimensions
         * vector
         *
         * @param dimensions the dimensions of the canva
         */
        constexpr explicit Canva(size_vector const& dimensions);

        /**
         * Constructs a new canva object
         */
        constexpr Canva(void) = default;

        /**
         * Constructs a new canva object from the given
         * constant reference to canva object
         *
         * @param canva the constant reference to canva object
         */
        constexpr Canva(Canva const& canva);

        /**
         * Constructs a new canva object from the given
         * rvalue reference to canva object
         *
         * @param canva the rvalue reference to canva object
         */
        constexpr Canva(Canva&& canva) noexcept;

        /**
         * Assings the given canva object to this object
         *
         * @param canva the constant reference to canva object
         * @return the reference to this object
         */
        constexpr Canva& operator=(Canva const& canva);

        /**
         * Assings the given rvalue canva object to this object
         *
         * @param canva the rvalue reference to canva object
         * @return the reference to this object
         */
        constexpr Canva& operator=(Canva&& canva) noexcept;

        /**
         * Represents the row of the image
         */
        class Row : private CanvaRowBase<Base> {
        public:
            using value_type =                      Base;
            using reference =                       value_type&;
            using pointer =                         value_type*;
            using const_reference =                 value_type const&;
            using const_pointer =                   value_type* const;

            /**
             * Constructs a new row object from the given constant
             * reference to row object
             *
             * @param row the constant reference to row obejct
             */
            constexpr Row(Row const& row) noexcept
                : BaseTuple{static_cast<BaseTuple const&>(row)} {}

            /**
             * Constructs a new row object from the given rvalue
             * reference to row object
             *
             * @param row the rvalue reference to row obejct
             */
            constexpr Row(Row&& row) noexcept
                : BaseTuple{static_cast<BaseTuple const&&>(row)} {}

            /**
             * Assings the given row object to this object
             *
             * @param row the constant reference to row object
             * @return the reference to this object
             */
            constexpr Row& operator= (Row const& row) noexcept;

            /**
             * Assings the given rvalue row object to this object
             *
             * @param row the rvalue reference to row object
             * @return the reference to this object
             */
            constexpr Row& operator= (Row&& row) noexcept;

            /**
             * Iterator that returns the pixels from the row
             *
             * @tparam IterTp the pixels' type
             */
            template <typename IterTp>
            class Iterator {
            public:
                using value_type                  = IterTp;
                using pointer                     = IterTp*;
                using reference                   = IterTp&;
                using difference_type             = std::ptrdiff_t;
                using iterator_category           = std::random_access_iterator_tag;
                using compare =
                    std::compare_three_way_result_t<pointer, pointer>;

                /**
                 * Constructs a new iterator object from thhe given
                 * pixel pointer
                 *
                 * @param iter the pointer to pixel
                 */
                constexpr explicit Iterator(pointer iter) noexcept
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
                    { ++iter; return *this; }

                /**
                 * Post-increments iterator by one and returns copy
                 * of the object
                 *
                 * @return the copied object
                 */
                [[nodiscard]] constexpr Iterator operator++(
                    int) noexcept
                        { auto temp = *this; ++iter; return *this; }

                /**
                 * Decrements iterator by one
                 *
                 * @return reference to this object
                 */
                constexpr Iterator& operator--(void) noexcept
                    { --iter; return *this; }

                /**
                 * Post-decrements iterator by one and returns copy
                 * of the object
                 *
                 * @return the copied object
                 */
                [[nodiscard]] constexpr Iterator operator--(
                    int) noexcept
                        { auto temp = *this; --iter; return *this; }

                /**
                 * Returns a reference to the pixel
                 *
                 * @return the reference to the pixel
                 */
                [[nodiscard]] constexpr reference operator*(
                    void) const noexcept
                        { return *iter; }

                /**
                 * Returns a pointer to the pixel
                 *
                 * @return the pointer to the pixel
                 */
                [[nodiscard]] constexpr pointer operator->(
                    void) noexcept
                        { return iter; }

                /**
                 * Increments iterator by the given distance
                 *
                 * @param offset the incremented distance
                 * @return reference to this object
                 */
                constexpr Iterator& operator+=(
                    difference_type offset) noexcept
                        { iter += offset; return *this; }

                /**
                 * Decrements iterator by the given distance
                 *
                 * @param offset the decremented distance
                 * @return reference to this object
                 */
                constexpr Iterator& operator-=(
                    difference_type offset) noexcept
                        { iter -= offset; return *this; }

                /**
                 * Returns a reference to the pixel shifted by
                 * the given offset
                 *
                 * @param offset the incremented distance
                 * @return the reference to the pixel
                 * shifted by the given offse
                 */
                [[nodiscard]] constexpr reference operator[](
                    difference_type offset) noexcept
                        { return *(iter + offset); }

                /**
                 * Checks whether two iterators are equal
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return whether two iterators are equal
                 */
                [[nodiscard]] constexpr friend bool operator== (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter == right.iter; }

                /**
                 * Compares two iterators to each other
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return the result of compare
                 */
                [[nodiscard]] constexpr friend compare operator<=> (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter <=> right.iter; }

                /**
                 * Adds given distance to an iterator
                 *
                 * @param left the iterator
                 * @param right the distance
                 * @return the shifted iterator
                 */
                [[nodiscard]] constexpr friend Iterator operator+ (
                    Iterator left,
                    difference_type right) noexcept
                        { return left += right; }

                /**
                 * Adds given distance to an iterator
                 *
                 * @param left the distance
                 * @param right the iterator
                 * @return the shifted iterator
                 */
                [[nodiscard]] constexpr friend Iterator operator+ (
                    difference_type left,
                    Iterator right) noexcept
                        { return right += left; }

                /**
                 * Subtracts given distance from iterator
                 *
                 * @param left the iterator
                 * @param right the distance
                 * @return the shifted operator
                 */
                [[nodiscard]] constexpr friend Iterator operator- (
                    Iterator left,
                    difference_type right) noexcept
                        { return left -= right; }

                /**
                 * Returns distance between iterators
                 *
                 * @param left the left iterator
                 * @param right the right iterator
                 * @return difference_type
                 */
                [[nodiscard]] constexpr friend difference_type
                    operator- (
                        Iterator const& left,
                        Iterator const& right) noexcept
                            { return left.iter - right.iter; }
            private:
                pointer                             iter;
            };

            using iterator =                        Iterator<Base>;
            using const_iterator =                  Iterator<Base const>;
            using reverse_iterator =                std::reverse_iterator<iterator>;
            using const_reverse_iterator =          std::reverse_iterator<const_iterator>;

            /**
             * Returns a size of the row
             *
             * @return the size of the row
             */
            [[nodiscard]] constexpr size_type size(
                void) const noexcept
                    { return this->second.get(); }

            /**
             * Returns an iterator to the begin of the row pixels
             *
             * @return the iterator to the begin of the row pixels
             */
            [[nodiscard]] constexpr iterator begin(void) noexcept
                { return iterator{memory()}; }

            /**
             * Returns an iterator to the end of the row pixels
             *
             * @return the iterator to the end of the row pixels
             */
            [[nodiscard]] constexpr iterator end(void) noexcept
                { return iterator{memory() + size()}; }

            /**
             * Returns a constant iterator to the begin of
             * the row pixels
             *
             * @return the constant iterator to the begin of
             * the row pixels
             */
            [[nodiscard]] constexpr const_iterator begin(
                void) const noexcept
                    { return const_iterator{memory()}; }

            /**
             * Returns a constant iterator to the end of
             * the row pixels
             *
             * @return the constant iterator to the end of
             * the row pixels
             */
            [[nodiscard]] constexpr const_iterator end(
                void) const noexcept
                    { return const_iterator{memory() + size()}; }

            /**
             * Returns a constant iterator to the begin of
             * the row pixels
             *
             * @return the constant iterator to the begin of
             * the row pixels
             */
            [[nodiscard]] constexpr const_iterator cbegin(
                void) const noexcept
                    { return const_iterator{memory()}; }

            /**
             * Returns a constant iterator to the end of
             * the row pixels
             *
             * @return the constant iterator to the end of
             * the row pixels
             */
            [[nodiscard]] constexpr const_iterator cend(
                void) const noexcept
                    { return const_iterator{memory() + size()}; }

            /**
             * Returns a reverse iterator to the end of the
             * row pixels
             *
             * @return the reverse iterator to the end of the
             * row pixels
             */
            [[nodiscard]] constexpr reverse_iterator rbegin(
                void) noexcept
                    { return reverse_iterator{end() - 1u}; }

            /**
             * Returns a reverse iterator to the begining of the
             * row pixels
             *
             * @return the reverse iterator to the begining of the
             * row pixels
             */
            [[nodiscard]] constexpr reverse_iterator rend(
                void) noexcept
                    { return reverse_iterator{begin() - 1u}; }

            /**
             * Returns a constant reverse iterator to the end
             * of the row pixels
             *
             * @return the constant reverse iterator to the end
             * of the row pixels
             */
            [[nodiscard]] constexpr const_reverse_iterator rbegin(
                void) const noexcept
                    { return const_reverse_iterator{end() - 1u}; }

            /**
             * Returns a constant reverse iterator to the begining
             * of the row pixels
             *
             * @return the constant reverse iterator to the begining
             * of the row pixels
             */
            [[nodiscard]] constexpr const_reverse_iterator rend(
                void) const noexcept
                    { return const_reverse_iterator{begin() - 1u}; }

            /**
             * Returns a constant reverse iterator to the end
             * of the row pixels
             *
             * @return the constant reverse iterator to the end
             * of the row pixels
             */
            [[nodiscard]] constexpr const_reverse_iterator crbegin(
                void) const noexcept
                    { return const_reverse_iterator{end() - 1u}; }

            /**
             * Returns a constant reverse iterator to the begining
             * of the row pixels
             *
             * @return the constant reverse iterator to the begining
             * of the row pixels
             */
            [[nodiscard]] constexpr const_reverse_iterator crend(
                void) const noexcept
                    { return const_reverse_iterator{begin() - 1u}; }

            /**
             * Returns a reference to the pixel with given index
             *
             * @param index the pixel's index
             * @return the reference to the pixel with given index
             */
            [[nodiscard]] constexpr reference operator[] (
                size_type index) noexcept
                    { return *(memory() + index); }

            /**
             * Returns a constant reference to the pixel with
             * given index
             *
             * @param index the pixel's index
             * @return the constant reference to the pixel
             * with given index
             */
            [[nodiscard]] constexpr const_reference operator[] (
                size_type index) const noexcept
                    { return *(memory() + index); }

            friend class Canva;

            /**
             * Destroys the row object
             */
            constexpr ~Row(void) = default;
        private:
            /**
             * Returns a constant pointer to the begining of the
             * row's memory
             *
             * @return the constant pointer to the begining of the
             * row's memory
             */
            [[nodiscard]] constexpr const_pointer memory(
                void) const noexcept
                    { return this->first; }
        };

        /**
         * Resizes the image to the new dimensions
         *
         * @param dimensions the image's new dimensions
         */
        constexpr void resize(size_vector const& dimensions);

        /**
         * Resizes the image to the new dimensions
         *
         * @param width the image's new width
         * @param height the image's new height
         */
        constexpr void resize(size_type width, size_type height)
            { return resize({width, height}); }

        /**
         * Return a dimensions of the image
         *
         * @return the dimensions of the image
         */
        [[nodiscard]] constexpr size_vector size(void) const noexcept
            { return dimensions; }

        /**
         * Returns a width of the image
         *
         * @return the width of the image
         */
        [[nodiscard]] constexpr size_type getWidth(void) const noexcept
            { return dimensions[0]; }

        /**
         * Returns a height of the image
         *
         * @return the height of the image
         */
        [[nodiscard]] constexpr size_type getHeight(
            void) const noexcept
                { return dimensions[1]; }

        /**
         * Returns a reference to the pixel lying under
         * given coordinates
         *
         * @tparam Tp the element type of the vector
         * @param coords the pixel coordinates
         * @return the reference to the pixel
         */
        template <std::integral Tp>
        [[nodiscard]] constexpr Base& operator[](
            Vector2<Tp> const& coords) noexcept;

        /**
         * Returns a constant reference to the pixel
         * lying under given coordinates
         *
         * @tparam Tp the element type of the vector
         * @param coords the pixel coordinates
         * @return the constant reference to the pixel
         */
        template <std::integral T>
        [[nodiscard]] constexpr Base const& operator[](
            Vector2<T> const& coords) const noexcept;

        /**
         * Returns a reference to the row with given index
         *
         * @param index the index of the row
         * @return the reference to the row
         */
        [[nodiscard]] constexpr Row& operator[] (
            size_type index) noexcept
                { return static_cast<Row&>(rows[index]); }

        /**
         * Returns a constant reference to the row with given index
         *
         * @param index the index of the row
         * @return the constant reference to the row
         */
        [[nodiscard]] constexpr Row const& operator[] (
            size_type index) const noexcept
                { return static_cast<Row const&>(rows[index]); }

        /**
         * Iterator that returns the references to the image
         * rows
         *
         * @tparam IterTp the type of the row
         * @tparam InnerIter the type of the underlying iterator
         */
        template <class IterTp, class InnerIter>
        class Iterator {
        public:
            using value_type                  = IterTp;
            using pointer                     = IterTp*;
            using reference                   = IterTp&;
            using difference_type             = std::ptrdiff_t;
            using iterator_category           = std::random_access_iterator_tag;
            using compare =
                std::compare_three_way_result_t<pointer, pointer>;

            /**
             * Constructs a new iterator object from the given
             * constant reference to the underlying iterator
             *
             * @param iter the constant reference to the
             * underlying iterator
             */
            constexpr explicit Iterator(
                InnerIter const& iter) noexcept
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
                { ++iter; return *this; }

            /**
             * Post-increments iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr Iterator operator++(int) noexcept
                { auto temp = *this; ++iter; return *this; }

            /**
             * Decrements iterator by one
             *
             * @return reference to this object
             */
            constexpr Iterator& operator--(void) noexcept
                { --iter; return *this; }

            /**
             * Post-decrements iterator by one and returns copy
             * of the object
             *
             * @return the copied object
             */
            [[nodiscard]] constexpr Iterator operator--(int) noexcept
                { auto temp = *this; --iter; return *this; }

            /**
             * Returns a reference to the row
             *
             * @return the reference to the row
             */
            [[nodiscard]] constexpr reference operator*(
                void) const noexcept
                    { return static_cast<reference>(*iter); }

            /**
             * Returns a pointer to the row
             *
             * @return the pointer to the row
             */
            [[nodiscard]] constexpr pointer operator->(void) noexcept
                { return static_cast<pointer>(&(*iter)); }

            /**
             * Increments iterator by the given distance
             *
             * @param offset the incremented distance
             * @return reference to this object
             */
            constexpr Iterator& operator+=
                (difference_type offset) noexcept
                    { iter += offset; return *this; }

            /**
             * Decrements iterator by the given distance
             *
             * @param offset the decremented distance
             * @return reference to this object
             */
            constexpr Iterator& operator-=(
                difference_type offset) noexcept
                    { iter -= offset; return *this; }

            /**
             * Returns a reference to the row shifted by
             * the given offset
             *
             * @param offset the incremented distance
             * @return the reference to the row
             * shifted by the given offse
             */
            [[nodiscard]] constexpr Iterator operator[](
                difference_type offset) noexcept
                    { auto temp = *this; temp += offset; return temp; }

            /**
             * Checks whether two iterators are equal
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return whether two iterators are equal
             */
            [[nodiscard]] friend constexpr bool operator== (
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter == right.iter; }

            /**
             * Compares two iterators to each other
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return the result of compare
             */
            [[nodiscard]] friend constexpr compare operator<=> (
                Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter <=> right.iter; }

            /**
             * Adds given distance to an iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted iterator
             */
            [[nodiscard]] friend constexpr Iterator operator+ (
                Iterator left,
                difference_type right) noexcept
                    { return left += right; }

            /**
             * Adds given distance to an iterator
             *
             * @param left the distance
             * @param right the iterator
             * @return the shifted iterator
             */
            [[nodiscard]] friend constexpr Iterator operator+ (
                difference_type left,
                Iterator right) noexcept
                    { return right += left; }

            /**
             * Subtracts given distance from iterator
             *
             * @param left the iterator
             * @param right the distance
             * @return the shifted operator
             */
            [[nodiscard]] friend constexpr Iterator operator- (
                Iterator left,
                difference_type right) noexcept
                    { return left -= right; }

            /**
             * Returns distance between iterators
             *
             * @param left the left iterator
             * @param right the right iterator
             * @return difference_type
             */
            [[nodiscard]] friend constexpr difference_type
                operator- (
                    Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter - right.iter; }
        private:
            InnerIter                           iter;
        };

        using iterator                            = Iterator<Row, RowIter>;
        using const_iterator                      = Iterator<Row const, RowConstIter>;
        using reverse_iterator                    = std::reverse_iterator<iterator>;
        using const_reverse_iterator              = std::reverse_iterator<const_iterator>;

        /**
         * Returns an iterator to the begining of the image rows
         *
         * @return the iterator to the begining of the image rows
         */
        [[nodiscard]] constexpr iterator begin(void) noexcept
            { return iterator{rows.begin()}; }

        /**
         * Returns an iterator to the end of the image rows
         *
         * @return the iterator to the end of the image rows
         */
        [[nodiscard]] constexpr iterator end(void) noexcept
            { return iterator{rows.end()}; }

        /**
         * Returns a constant iterator to the begining of
         * the image rows
         *
         * @return the constant iterator to the begining of
         * the image rows
         */
        [[nodiscard]] constexpr const_iterator begin(
            void) const noexcept
                { return const_iterator{rows.begin()}; }

        /**
         * Returns a constant iterator to the end of
         * the image rows
         *
         * @return the constant iterator to the end of
         * the image rows
         */
        [[nodiscard]] constexpr const_iterator end(
            void) const noexcept
                { return const_iterator{rows.end()}; }

        /**
         * Returns a constant iterator to the begining of
         * the image rows
         *
         * @return the constant iterator to the begining of
         * the image rows
         */
        [[nodiscard]] constexpr const_iterator cbegin(
            void) const noexcept
                { return const_iterator{rows.cbegin()}; }

        /**
         * Returns a constant iterator to the end of
         * the image rows
         *
         * @return the constant iterator to the end of
         * the image rows
         */
        [[nodiscard]] constexpr const_iterator cend(
            void) const noexcept
                { return const_iterator{rows.cend()}; }

        /**
         * Returns a reverse iterator to the end of
         * the image rows
         *
         * @return the reverse iterator to the end of
         * the image rows
         */
        [[nodiscard]] constexpr reverse_iterator rbegin(void) noexcept
            { return reverse_iterator{end() - 1u}; }

        /**
         * Returns a reverse iterator to the beginig of
         * the image rows
         *
         * @return the reverse iterator to the beginig of
         * the image rows
         */
        [[nodiscard]] constexpr reverse_iterator rend(void) noexcept
            { return reverse_iterator{begin() - 1u}; }

        /**
         * Returns a constant reverse iterator to the end of
         * the image rows
         *
         * @return the constant reverse iterator to the end of
         * the image rows
         */
        [[nodiscard ]]constexpr const_reverse_iterator
            rbegin(void) const noexcept
                { return const_reverse_iterator{end() - 1u}; }

        /**
         * Returns a constant reverse iterator to the begining of
         * the image rows
         *
         * @return the constant reverse iterator to the begining of
         * the image rows
         */
        [[nodiscard]] constexpr const_reverse_iterator
            rend(void) const noexcept
                { return const_reverse_iterator{begin() - 1u}; }

        /**
         * Returns a constant reverse iterator to the end of
         * the image rows
         *
         * @return the constant reverse iterator to the end of
         * the image rows
         */
        [[nodiscard]] constexpr const_reverse_iterator
            crbegin(void) const noexcept
                { return const_reverse_iterator{end() - 1u}; }

        /**
         * Returns a constant reverse iterator to the begining of
         * the image rows
         *
         * @return the constant reverse iterator to the begining of
         * the image rows
         */
        [[nodiscard]] constexpr const_reverse_iterator
            crend(void) const noexcept
                { return const_reverse_iterator{begin() - 1u}; }

        /**
         * Returns a constant pointer to the image data
         *
         * @return the constant pointer to the image data
         */
        [[nodiscard]] constexpr Base const* data(void) const noexcept
            { return memoryMap.data(); }

        /**
         * Returns a pointer to the image data
         *
         * @return the pointer to the image data
         */
        [[nodiscard]] constexpr Base* data(void) noexcept
            { return memoryMap.data(); }

        /**
         * Extracts an area starting with given coordinates with
         * given dimensions to the other canva
         *
         * @tparam URange the new canva base range
         * @tparam URowRange the new canva row range
         * @param x the start pixel x-coordinate
         * @param y the start pixel y-coordinate
         * @param width the width of the new canva
         * @param height the height of the new canva
         * @return the extracted canva
         */
        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        [[nodiscard]] constexpr Canva<Base, URange, URowRange>
            extract(
                size_type x,
                size_type y,
                size_type width,
                size_type height) const noexcept;

        /**
         * Extracts an area starting with given coordinates with
         * given dimensions to the other canva
         *
         * @tparam URange the new canva base range
         * @tparam URowRange the new canva row range
         * @param coords the start pixel coordinates
         * @param dimensions the new canva dimensions
         * @return the extracted canva
         */
        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        [[nodiscard]] constexpr Canva<Base, URange, URowRange>
            extract(
                size_vector coords,
                size_vector dimensions) const noexcept;

        /**
         * Extracts an area starting with given coordinates with
         * given dimensions to the other canva. If given coords
         * or dimensions are wrong then returns an empty optional
         *
         * @tparam URange the new canva base range
         * @tparam URowRange the new canva row range
         * @param x the start pixel x-coordinate
         * @param y the start pixel y-coordinate
         * @param width the width of the new canva
         * @param height the height of the new canva
         * @return the extracted canva
         */
        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        [[nodiscard]] constexpr std::optional<
            Canva<Base, URange, URowRange>>
                safe_extract(
                    size_type x,
                    size_type y,
                    size_type width,
                    size_type height) const noexcept;

        /**
         * Extracts an area starting with given coordinates with
         * given dimensions to the other canva. If given coords
         * or dimensions are wrong then returns an empty optional
         *
         * @tparam URange the new canva base range
         * @tparam URowRange the new canva row range
         * @param coords the start pixel coordinates
         * @param dimensions the new canva dimensions
         * @return the extracted canva
         */
        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        [[nodiscard]] constexpr std::optional<
            Canva<Base, URange, URowRange>>
                safe_extract(
                    size_vector coords,
                    size_vector dimensions) const noexcept;

        constexpr ~Canva(void) noexcept = default;
    private:
        Range                                       memoryMap;
        RowRange                                    rows;
        size_vector                                 dimensions;

        /**
         * Creates a new rows inside an image
         */
        constexpr void createRows(void);

        /**
         * Reassigns the image's rows
         */
        constexpr void reassignRows(void) noexcept;
    };

}

#include <MPGL/Collections/Canva.tpp>
