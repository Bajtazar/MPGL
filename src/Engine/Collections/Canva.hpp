#pragma once

#include <iterator>
#include <memory>
#include <vector>

#include "../Mathematics/Vector.hpp"
#include "../Traits/Concepts.hpp"

#if __cplusplus > 202002L
#define VEC_CONSTEXPR constexpr
#else
#define VEC_CONSTEXPR
#endif

namespace ge {

    template <Absolute Tp, Absolute St>
    using CanvaRowBase = std::pair<Tp*, std::reference_wrapper<St>>;

    template <DefaultBaseType Base, template<class, class> class Range
            = std::vector, template<class> class Alloc = std::allocator>
        requires (UnderlyiesTypes<Range, Base, CanvaRowBase<Base, std::size_t>>)
    class Canva {
    public:
        typedef std::size_t                         size_type;
        typedef Vector2<size_type>                  size_vector;
    private:
        typedef CanvaRowBase<Base, size_type>       BaseTuple;
        typedef Range<BaseTuple, Alloc<BaseTuple>>  RowsRange;
        typedef typename RowsRange::iterator        RowIter;
        typedef typename RowsRange::const_iterator  RowConstIter;
    public:

        VEC_CONSTEXPR explicit Canva(size_type width, size_type height);
        VEC_CONSTEXPR explicit Canva(size_vector const& dimensions);
        VEC_CONSTEXPR Canva(void) = default;

        class Row : private CanvaRowBase<Base, size_type> {
        public:
            using value_type =                      Base;
            using reference =                       value_type&;
            using pointer =                         value_type*;
            using const_reference =                 value_type const&;
            using const_pointer =                   value_type* const;

            Row(void) noexcept = delete;

            VEC_CONSTEXPR Row(Row const& row) noexcept
                : BaseTuple{static_cast<BaseTuple const&>(row)} {}
            VEC_CONSTEXPR Row(Row&& row) noexcept
                : BaseTuple{static_cast<BaseTuple const&&>(row)} {}

            VEC_CONSTEXPR Row& operator= (Row const& row) noexcept;
            VEC_CONSTEXPR Row& operator= (Row&& row) noexcept;

            template <typename value_type>
            class Iterator : public std::iterator<
                std::random_access_iterator_tag, value_type>
            {
            public:
                using pointer =                     value_type*;
                using reference =                   value_type&;
                using difference_type =             std::ptrdiff_t;
                using iterator_category =           std::random_access_iterator_tag;

                VEC_CONSTEXPR explicit Iterator(pointer iter) noexcept
                    : iter{iter} {}
                VEC_CONSTEXPR explicit Iterator(void) noexcept = default;

                VEC_CONSTEXPR Iterator& operator++(void) noexcept
                    { ++iter; return *this; }
                VEC_CONSTEXPR Iterator  operator++(int) noexcept
                    { auto temp = *this; ++iter; return *this; }
                VEC_CONSTEXPR Iterator& operator--(void) noexcept
                    { --iter; return *this; }
                VEC_CONSTEXPR Iterator  operator--(int) noexcept
                    { auto temp = *this; --iter; return *this; }

                VEC_CONSTEXPR reference operator*(void) const noexcept
                    { return *iter; }
                VEC_CONSTEXPR pointer operator->(void) noexcept
                    { return iter; }
                VEC_CONSTEXPR Iterator& operator+=(difference_type offset) noexcept
                    { iter += offset; return *this; }
                VEC_CONSTEXPR Iterator& operator-=(difference_type offset) noexcept
                    { iter -= offset; return *this; }
                VEC_CONSTEXPR reference operator[](difference_type offset) noexcept
                    { return *(iter + offset); }

                friend VEC_CONSTEXPR bool operator== (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter == right.iter; }
                friend VEC_CONSTEXPR bool operator>= (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter >= right.iter; }
                friend VEC_CONSTEXPR bool operator<= (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter <= right.iter; }
                friend VEC_CONSTEXPR bool operator> (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter > right.iter; }
                friend VEC_CONSTEXPR bool operator< (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter < right.iter; }

                friend VEC_CONSTEXPR Iterator operator+ (Iterator const& left,
                    difference_type right) noexcept
                { return left[right]; }
                friend VEC_CONSTEXPR Iterator operator+ (difference_type left,
                    Iterator const& right) noexcept
                { return right[left]; }
                friend VEC_CONSTEXPR Iterator operator- (Iterator const& left,
                    difference_type right) noexcept
                { auto temp = left; temp -= right; return temp; }
                friend VEC_CONSTEXPR difference_type operator- (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter - right.iter; }
            private:
                pointer                             iter;
            };

            using iterator =                        Iterator<Base>;
            using const_iterator =                  Iterator<Base const>;
            using reverse_iterator =                std::reverse_iterator<iterator>;
            using const_reverse_iterator =          std::reverse_iterator<const_iterator>;

            VEC_CONSTEXPR size_type size(void) const noexcept
                { return this->second.get(); }

            VEC_CONSTEXPR iterator begin(void) noexcept
                { return iterator{memory()}; }
            VEC_CONSTEXPR iterator end(void) noexcept
                { return iterator{memory() + size()}; }
            VEC_CONSTEXPR const_iterator begin(void) const noexcept
                { return const_iterator{memory()}; }
            VEC_CONSTEXPR const_iterator end(void) const noexcept
                { return const_iterator{memory() + size()}; }

            VEC_CONSTEXPR const_iterator cbegin(void) const noexcept
                { return const_iterator{memory()}; }
            VEC_CONSTEXPR const_iterator cend(void) const noexcept
                { return const_iterator{memory() + size()}; }

            VEC_CONSTEXPR reverse_iterator rbegin(void) noexcept
                { return reverse_iterator{end() - 1u}; }
            VEC_CONSTEXPR reverse_iterator rend(void) noexcept
                { return reverse_iterator{begin() - 1u}; }
            VEC_CONSTEXPR const_reverse_iterator rbegin(void) const noexcept
                { return const_reverse_iterator{end() - 1u}; }
            VEC_CONSTEXPR const_reverse_iterator rend(void) const noexcept
                { return const_reverse_iterator{begin() - 1u}; }

            VEC_CONSTEXPR const_reverse_iterator crbegin(void) const noexcept
                { return const_reverse_iterator{end() - 1u}; }
            VEC_CONSTEXPR const_reverse_iterator crend(void) const noexcept
                { return const_reverse_iterator{begin() - 1u}; }

            VEC_CONSTEXPR reference operator[] (size_type index) noexcept
                { return *(memory() + index); }
            VEC_CONSTEXPR const_reference operator[] (size_type index) const noexcept
                { return *(memory() + index); }

            friend class Canva;

            VEC_CONSTEXPR ~Row(void) = default;
        private:
            VEC_CONSTEXPR const_pointer memory(void) const noexcept
                { return this->first; }
        };

        VEC_CONSTEXPR void resize(size_vector const& dimensions);
        VEC_CONSTEXPR void resize(size_type width, size_type height)
            { return resize({width, height}); }

        VEC_CONSTEXPR size_vector size(void) const noexcept
            { return dimensions; }
        VEC_CONSTEXPR size_type getWidth(void) const noexcept
            { return dimensions[0]; }
        VEC_CONSTEXPR size_type getHeight(void) const noexcept
            { return dimensions[1]; }

        template <std::integral T>
        VEC_CONSTEXPR Base& operator[](Vector2<T> const& coords) noexcept
            { return memoryMap[dimensions[0] * coords[1] + coords[0]]; }
        template <std::integral T>
        VEC_CONSTEXPR Base const& operator[](Vector2<T> const& coords) const noexcept
            { return memoryMap[dimensions[0] * coords[1] + coords[0]]; }

        VEC_CONSTEXPR Row& operator[] (size_type index) noexcept
            { return static_cast<Row&>(rows[index]); }
        VEC_CONSTEXPR Row const& operator[] (size_type index) const noexcept
            { return static_cast<Row const&>(rows[index]); }

        template <class value_type, class InnerIter>
        class Iterator : public std::iterator<
            std::random_access_iterator_tag, value_type>
        {
        public:
            using pointer =                     value_type*;
            using reference =                   value_type&;
            using difference_type =             std::ptrdiff_t;
            using iterator_category =           std::random_access_iterator_tag;

            VEC_CONSTEXPR explicit Iterator(InnerIter const& iter) noexcept
                : iter{iter} {}
            VEC_CONSTEXPR explicit Iterator(void) noexcept = default;

            VEC_CONSTEXPR Iterator& operator++(void) noexcept
                { ++iter; return *this; }
            VEC_CONSTEXPR Iterator  operator++(int) noexcept
                { auto temp = *this; ++iter; return *this; }
            VEC_CONSTEXPR Iterator& operator--(void) noexcept
                { --iter; return *this; }
            VEC_CONSTEXPR Iterator  operator--(int) noexcept
                { auto temp = *this; --iter; return *this; }

            VEC_CONSTEXPR reference operator*(void) const noexcept
                { return static_cast<reference>(*iter); }
            VEC_CONSTEXPR pointer operator->(void) noexcept
                { returnstatic_cast<pointer>(*iter); }
            VEC_CONSTEXPR Iterator& operator+=(difference_type offset) noexcept
                { iter += offset; return *this; }
            VEC_CONSTEXPR Iterator& operator-=(difference_type offset) noexcept
                { iter -= offset; return *this; }
            VEC_CONSTEXPR Iterator  operator[](difference_type offset) noexcept
                { auto temp = *this; temp += offset; return temp; }

            friend VEC_CONSTEXPR bool operator== (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter == right.iter; }
            friend VEC_CONSTEXPR bool operator>= (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter >= right.iter; }
            friend VEC_CONSTEXPR bool operator<= (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter <= right.iter; }
            friend VEC_CONSTEXPR bool operator> (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter > right.iter; }
            friend VEC_CONSTEXPR bool operator< (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter < right.iter; }

            friend VEC_CONSTEXPR Iterator operator+ (Iterator const& left,
                    difference_type right) noexcept
                { return left[right]; }
            friend VEC_CONSTEXPR Iterator operator+ (difference_type left,
                    Iterator const& right) noexcept
                { return right[left]; }
            friend VEC_CONSTEXPR Iterator operator- (Iterator const& left,
                    difference_type right) noexcept
                { auto temp = left; temp -= right; return temp; }
            friend VEC_CONSTEXPR difference_type operator- (Iterator const& left,
                    Iterator const& right) noexcept
                { return left.iter - right.iter; }
        private:
            InnerIter                           iter;
        };

        using iterator =                            Iterator<Row, RowIter>;
        using const_iterator =                      Iterator<Row const, RowConstIter>;
        using reverse_iterator =                    std::reverse_iterator<iterator>;
        using const_reverse_iterator =              std::reverse_iterator<const_iterator>;

        VEC_CONSTEXPR iterator begin(void) noexcept
            { return iterator{rows.begin()}; }
        VEC_CONSTEXPR iterator end(void) noexcept
            { return iterator{rows.end()}; }
        VEC_CONSTEXPR const_iterator begin(void) const noexcept
            { return const_iterator{rows.begin()}; }
        VEC_CONSTEXPR const_iterator end(void) const noexcept
            { return const_iterator{rows.end()}; }

        VEC_CONSTEXPR const_iterator cbegin(void) const noexcept
            { return const_iterator{rows.cbegin()}; }
        VEC_CONSTEXPR const_iterator cend(void) const noexcept
            { return const_iterator{rows.cend()}; }

        VEC_CONSTEXPR reverse_iterator rbegin(void) noexcept
            { return reverse_iterator{end() - 1u}; }
        VEC_CONSTEXPR reverse_iterator rend(void) noexcept
            { return reverse_iterator{begin() - 1u}; }
        VEC_CONSTEXPR const_reverse_iterator rbegin(void) const noexcept
            { return const_reverse_iterator{end() - 1u}; }
        VEC_CONSTEXPR const_reverse_iterator rend(void) const noexcept
            { return const_reverse_iterator{begin() - 1u}; }

        VEC_CONSTEXPR const_reverse_iterator crbegin(void) const noexcept
            { return const_reverse_iterator{end() - 1u}; }
        VEC_CONSTEXPR const_reverse_iterator crend(void) const noexcept
            { return const_reverse_iterator{begin() - 1u}; }

        VEC_CONSTEXPR const void* getMemoryPtr(void) const noexcept
            { return memoryMap.data(); }

        VEC_CONSTEXPR void* getMemoryPtr(void) noexcept
            { return memoryMap.data(); }
    private:
        Range<Base, Alloc<Base>>                    memoryMap;
        RowsRange                                   rows;
        size_vector                                 dimensions;

        VEC_CONSTEXPR void createRows(void);
    };

    template <DefaultBaseType Base, template<class, class> class Range,
            template<class> class Alloc>
        requires (UnderlyiesTypes<Range, Base, CanvaRowBase<Base, std::size_t>>)
    VEC_CONSTEXPR Canva<Base, Range, Alloc>::Row&
        Canva<Base, Range, Alloc>::Row::operator= (Row const& row) noexcept
    {
        static_cast<BaseTuple&>(*this) = static_cast<BaseTuple const&>(row);
        return *this;
    }

    template <DefaultBaseType Base, template<class, class> class Range,
            template<class> class Alloc>
        requires (UnderlyiesTypes<Range, Base, CanvaRowBase<Base, std::size_t>>)
    VEC_CONSTEXPR Canva<Base, Range, Alloc>::Row&
        Canva<Base, Range, Alloc>::Row::operator= (Row&& row) noexcept
    {
        static_cast<BaseTuple&>(*this) = static_cast<BaseTuple&&>(row);
        return *this;
    }

    template <DefaultBaseType Base, template<class, class> class Range,
            template<class> class Alloc>
        requires (UnderlyiesTypes<Range, Base, CanvaRowBase<Base, std::size_t>>)
    VEC_CONSTEXPR Canva<Base, Range, Alloc>::Canva(size_type width, size_type height)
        : Canva{{width, height}} {}

    template <DefaultBaseType Base, template<class, class> class Range,
            template<class> class Alloc>
        requires (UnderlyiesTypes<Range, Base, CanvaRowBase<Base, std::size_t>>)
    VEC_CONSTEXPR Canva<Base, Range, Alloc>::Canva(size_vector const& dimensions)
        : memoryMap(dimensions[0] * dimensions[1], Base{}), dimensions{dimensions}
    {
        createRows();
    }

    template <DefaultBaseType Base, template<class, class> class Range,
            template<class> class Alloc>
        requires (UnderlyiesTypes<Range, Base, CanvaRowBase<Base, std::size_t>>)
    VEC_CONSTEXPR void Canva<Base, Range, Alloc>::resize(size_vector const& dimensions)
    {
        this->dimensions = dimensions;
        size_type size = dimensions[1] * dimensions[0];
        memoryMap.resize(size);
        rows.clear();
        createRows();
    }

    template <DefaultBaseType Base, template<class, class> class Range,
            template<class> class Alloc>
        requires (UnderlyiesTypes<Range, Base, CanvaRowBase<Base, std::size_t>>)
    VEC_CONSTEXPR void Canva<Base, Range, Alloc>::createRows(void)
    {
        Base* iter = memoryMap.data();
        auto width = std::ref(dimensions[0]);
        rows.reserve(dimensions[1]);
        for (size_type i = 0; i != dimensions[1]; ++i, iter += dimensions[0])
            rows.emplace_back(iter, width);
    }

}
