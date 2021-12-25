#pragma once

#include <iterator>
#include <optional>
#include <cstring>
#include <compare>
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

    template <Absolute Tp>
    using CanvaRowBase = std::pair<Tp*, std::reference_wrapper<
        std::size_t>>;

    template <DefaultBaseType Base,
        UnderlyingRange<Base> Range = std::vector<Base>,
        UnderlyingRange<CanvaRowBase<Base>> RowRange
            = std::vector<CanvaRowBase<Base>>>
    class Canva {
    public:
        typedef std::size_t                         size_type;
        typedef Vector2<size_type>                  size_vector;
    private:
        typedef CanvaRowBase<Base>                  BaseTuple;
        typedef typename RowRange::iterator         RowIter;
        typedef typename RowRange::const_iterator   RowConstIter;
    public:

        VEC_CONSTEXPR explicit Canva(size_type width, size_type height);
        VEC_CONSTEXPR explicit Canva(size_vector const& dimensions);
        VEC_CONSTEXPR Canva(void) = default;

        VEC_CONSTEXPR Canva(Canva const& canva);
        VEC_CONSTEXPR Canva(Canva&& canva) noexcept;

        VEC_CONSTEXPR Canva& operator=(Canva const& canva);
        VEC_CONSTEXPR Canva& operator=(Canva&& canva) noexcept;

        class Row : private CanvaRowBase<Base> {
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
                { return static_cast<pointer>(*iter); }
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

        VEC_CONSTEXPR void const* getMemoryPtr(void) const noexcept
            { return memoryMap.data(); }

        VEC_CONSTEXPR void* getMemoryPtr(void) noexcept
            { return memoryMap.data(); }

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        VEC_CONSTEXPR Canva<Base, URange, URowRange>
            extract(size_type x, size_type y,
                    size_type width, size_type height) const noexcept;

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        VEC_CONSTEXPR Canva<Base, URange, URowRange>
            extract(size_vector coords, size_vector dimensions) const noexcept;

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        VEC_CONSTEXPR std::optional<Canva<Base, URange, URowRange>>
            safe_extract(size_type x, size_type y,
                size_type width, size_type height) const noexcept;

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        VEC_CONSTEXPR std::optional<Canva<Base, URange, URowRange>>
            safe_extract(size_vector coords,
                size_vector dimensions) const noexcept;

        VEC_CONSTEXPR ~Canva(void) noexcept = default;
    private:
        Range                                       memoryMap;
        RowRange                                    rows;
        size_vector                                 dimensions;

        VEC_CONSTEXPR void createRows(void);
        VEC_CONSTEXPR void reassignRows(void) noexcept;
    };

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>::Row&
        Canva<Base, Range, RowRange>::Row::operator= (
            Row const& row) noexcept
    {
        static_cast<BaseTuple&>(*this) =
            static_cast<BaseTuple const&>(row);
        return *this;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>::Row&
        Canva<Base, Range, RowRange>::Row::operator= (Row&& row) noexcept
    {
        static_cast<BaseTuple&>(*this) = static_cast<BaseTuple&&>(row);
        return *this;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>::Canva(
        size_type width, size_type height) : Canva{{width, height}} {}

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>::Canva(
        size_vector const& dimensions)
            : memoryMap(dimensions[0] * dimensions[1], Base{}),
            dimensions{dimensions}
    {
        createRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>::Canva(
        Canva const& canva)
            : memoryMap{canva.memoryMap.begin(),
            canva.memoryMap.end()}, dimensions{canva.dimensions}
    {
        createRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>::Canva(
        Canva&& canva) noexcept
            : memoryMap{std::move(canva.memoryMap)},
            rows{std::move(canva.rows)},
            dimensions{canva.dimensions}
    {
        reassignRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>&
        Canva<Base, Range, RowRange>::operator=(Canva const& canva)
    {
        memoryMap = canva.memoryMap;
        dimensions = canva.dimensions;
        rows.clear();
        createRows();
        return *this;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR Canva<Base, Range, RowRange>&
        Canva<Base, Range, RowRange>::operator=(
            Canva&& canva) noexcept
    {
        memoryMap = std::move(canva.memoryMap);
        dimensions = std::move(canva.dimensions);
        rows = std::move(canva.rows);
        reassignRows();
        return *this;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR void
        Canva<Base, Range, RowRange>::reassignRows(void) noexcept
    {
        auto width = std::ref(dimensions[0]);
        for (auto& ref : rows | std::views::transform(&BaseTuple::second))
            ref = width;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR void Canva<Base, Range, RowRange>::resize(
        size_vector const& dimensions)
    {
        this->dimensions = dimensions;
        memoryMap.resize(dimensions[1] * dimensions[0]);
        rows.clear();
        createRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    VEC_CONSTEXPR void Canva<Base, Range, RowRange>::createRows(void)
    {
        Base* iter = memoryMap.data();
        auto width = std::ref(dimensions[0]);
        rows.reserve(dimensions[1]);
        for (size_type i = 0; i != dimensions[1]; ++i,
            iter += dimensions[0])
                rows.emplace_back(iter, width);
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    template <UnderlyingRange<Base> URange,
        UnderlyingRange<CanvaRowBase<Base>> URowRange>
    VEC_CONSTEXPR Canva<Base, URange, URowRange>
        Canva<Base, Range, RowRange>::extract(size_type x,
            size_type y, size_type width,
            size_type height) const noexcept
    {
        Canva<Base, Range, RowRange> image{width, height};
        Base* imem = image.memoryMap.data();
        Base const* tmem = memoryMap.data() +
            y * this->dimensions[0] + x;
        for (size_type i = 0; i < height; ++i,
            imem += width, tmem += this->dimensions[0])
                std::memcpy(imem, tmem, sizeof(Base) * width);
        return image;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    template <UnderlyingRange<Base> URange,
        UnderlyingRange<CanvaRowBase<Base>> URowRange>
    VEC_CONSTEXPR Canva<Base, URange, URowRange>
        Canva<Base, Range, RowRange>::extract(
            size_vector coords,
            size_vector dimensions) const noexcept
    {
        return extract<URange, URowRange>(
            coords[0], coords[1], dimensions[0], dimensions[1]);
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    template <UnderlyingRange<Base> URange,
        UnderlyingRange<CanvaRowBase<Base>> URowRange>
    VEC_CONSTEXPR std::optional<Canva<Base, URange, URowRange>>
        Canva<Base, Range, RowRange>::safe_extract(
            size_type x, size_type y,
            size_type width, size_type height) const noexcept
    {
        if (x + width > dimensions[0] || y + height > dimensions[1])
            return {};
        return { extract<URange, URowRange>(x, y, width, height) };
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    template <UnderlyingRange<Base> URange,
        UnderlyingRange<CanvaRowBase<Base>> URowRange>
    VEC_CONSTEXPR std::optional<Canva<Base, URange, URowRange>>
        Canva<Base, Range, RowRange>::safe_extract(
            size_vector coords,
            size_vector dimensions) const noexcept
    {
        // std::partial_ordering::unordered concidered as false
        if (!(totalCompare(coords + dimensions, this->dimensions) <= 0))
            return {};
        return { extract<URange, URowRange>(
            coords[0], coords[1], dimensions[0], dimensions[1]) };
    }

}
