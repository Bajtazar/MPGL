#pragma once

#include <iterator>
#include <optional>
#include <cstring>
#include <compare>
#include <memory>
#include <vector>

#include "../Mathematics/Vector.hpp"
#include "../Traits/Concepts.hpp"

#ifndef friend_expr
#define friend_expr friend constexpr
#endif

namespace mpgl {

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
        typedef Base                                pixel;
    private:
        typedef CanvaRowBase<Base>                  BaseTuple;
        typedef typename RowRange::iterator         RowIter;
        typedef typename RowRange::const_iterator   RowConstIter;
    public:
        constexpr explicit Canva(size_type width, size_type height);
        constexpr explicit Canva(size_vector const& dimensions);
        constexpr Canva(void) = default;

        constexpr Canva(Canva const& canva);
        constexpr Canva(Canva&& canva) noexcept;

        constexpr Canva& operator=(Canva const& canva);
        constexpr Canva& operator=(Canva&& canva) noexcept;

        class Row : private CanvaRowBase<Base> {
        public:
            using value_type =                      Base;
            using reference =                       value_type&;
            using pointer =                         value_type*;
            using const_reference =                 value_type const&;
            using const_pointer =                   value_type* const;

            Row(void) noexcept = delete;

            constexpr Row(Row const& row) noexcept
                : BaseTuple{static_cast<BaseTuple const&>(row)} {}
            constexpr Row(Row&& row) noexcept
                : BaseTuple{static_cast<BaseTuple const&&>(row)} {}

            constexpr Row& operator= (Row const& row) noexcept;
            constexpr Row& operator= (Row&& row) noexcept;

            template <typename value_type>
            class Iterator : public std::iterator<
                std::random_access_iterator_tag, value_type>
            {
            public:
                using pointer =                     value_type*;
                using reference =                   value_type&;
                using difference_type =             std::ptrdiff_t;
                using iterator_category =           std::random_access_iterator_tag;
                using compare =
                    std::compare_three_way_result_t<pointer, pointer>;

                constexpr explicit Iterator(pointer iter) noexcept
                    : iter{iter} {}
                constexpr explicit Iterator(void) noexcept = default;

                constexpr Iterator& operator++(void) noexcept
                    { ++iter; return *this; }
                constexpr Iterator  operator++(int) noexcept
                    { auto temp = *this; ++iter; return *this; }
                constexpr Iterator& operator--(void) noexcept
                    { --iter; return *this; }
                constexpr Iterator  operator--(int) noexcept
                    { auto temp = *this; --iter; return *this; }

                constexpr reference operator*(void) const noexcept
                    { return *iter; }
                constexpr pointer operator->(void) noexcept
                    { return iter; }
                constexpr Iterator& operator+=(difference_type offset) noexcept
                    { iter += offset; return *this; }
                constexpr Iterator& operator-=(difference_type offset) noexcept
                    { iter -= offset; return *this; }
                constexpr reference operator[](difference_type offset) noexcept
                    { return *(iter + offset); }

                friend_expr bool operator== (Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter == right.iter; }

                friend_expr compare operator<=> (
                    Iterator const& left, Iterator const& right) noexcept
                        { return left.iter <=> right.iter; }

                friend_expr Iterator operator+ (Iterator const& left,
                    difference_type right) noexcept
                        { return left[right]; }
                friend_expr Iterator operator+ (difference_type left,
                    Iterator const& right) noexcept
                        { return right[left]; }
                friend_expr Iterator operator- (Iterator const& left,
                    difference_type right) noexcept
                        { auto temp = left; temp -= right; return temp; }
                friend_expr difference_type operator- (Iterator const& left,
                    Iterator const& right) noexcept
                        { return left.iter - right.iter; }
            private:
                pointer                             iter;
            };

            using iterator =                        Iterator<Base>;
            using const_iterator =                  Iterator<Base const>;
            using reverse_iterator =                std::reverse_iterator<iterator>;
            using const_reverse_iterator =          std::reverse_iterator<const_iterator>;

            constexpr size_type size(void) const noexcept
                { return this->second.get(); }

            constexpr iterator begin(void) noexcept
                { return iterator{memory()}; }
            constexpr iterator end(void) noexcept
                { return iterator{memory() + size()}; }
            constexpr const_iterator begin(void) const noexcept
                { return const_iterator{memory()}; }
            constexpr const_iterator end(void) const noexcept
                { return const_iterator{memory() + size()}; }

            constexpr const_iterator cbegin(void) const noexcept
                { return const_iterator{memory()}; }
            constexpr const_iterator cend(void) const noexcept
                { return const_iterator{memory() + size()}; }

            constexpr reverse_iterator rbegin(void) noexcept
                { return reverse_iterator{end() - 1u}; }
            constexpr reverse_iterator rend(void) noexcept
                { return reverse_iterator{begin() - 1u}; }
            constexpr const_reverse_iterator rbegin(void) const noexcept
                { return const_reverse_iterator{end() - 1u}; }
            constexpr const_reverse_iterator rend(void) const noexcept
                { return const_reverse_iterator{begin() - 1u}; }

            constexpr const_reverse_iterator crbegin(void) const noexcept
                { return const_reverse_iterator{end() - 1u}; }
            constexpr const_reverse_iterator crend(void) const noexcept
                { return const_reverse_iterator{begin() - 1u}; }

            constexpr reference operator[] (size_type index) noexcept
                { return *(memory() + index); }
            constexpr const_reference operator[] (size_type index) const noexcept
                { return *(memory() + index); }

            friend class Canva;

            constexpr ~Row(void) = default;
        private:
            constexpr const_pointer memory(void) const noexcept
                { return this->first; }
        };

        constexpr void resize(size_vector const& dimensions);
        constexpr void resize(size_type width, size_type height)
            { return resize({width, height}); }

        constexpr size_vector size(void) const noexcept
            { return dimensions; }
        constexpr size_type getWidth(void) const noexcept
            { return dimensions[0]; }
        constexpr size_type getHeight(void) const noexcept
            { return dimensions[1]; }

        template <std::integral T>
        constexpr Base& operator[](Vector2<T> const& coords) noexcept
            { return memoryMap[dimensions[0] * coords[1] + coords[0]]; }
        template <std::integral T>
        constexpr Base const& operator[](Vector2<T> const& coords) const noexcept
            { return memoryMap[dimensions[0] * coords[1] + coords[0]]; }

        constexpr Row& operator[] (size_type index) noexcept
            { return static_cast<Row&>(rows[index]); }
        constexpr Row const& operator[] (size_type index) const noexcept
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
            using compare =
                std::compare_three_way_result_t<pointer, pointer>;

            constexpr explicit Iterator(InnerIter const& iter) noexcept
                : iter{iter} {}
            constexpr explicit Iterator(void) noexcept = default;

            constexpr Iterator& operator++(void) noexcept
                { ++iter; return *this; }
            constexpr Iterator  operator++(int) noexcept
                { auto temp = *this; ++iter; return *this; }
            constexpr Iterator& operator--(void) noexcept
                { --iter; return *this; }
            constexpr Iterator  operator--(int) noexcept
                { auto temp = *this; --iter; return *this; }

            constexpr reference operator*(void) const noexcept
                { return static_cast<reference>(*iter); }
            constexpr pointer operator->(void) noexcept
                { return static_cast<pointer>(*iter); }
            constexpr Iterator& operator+=(difference_type offset) noexcept
                { iter += offset; return *this; }
            constexpr Iterator& operator-=(difference_type offset) noexcept
                { iter -= offset; return *this; }
            constexpr Iterator  operator[](difference_type offset) noexcept
                { auto temp = *this; temp += offset; return temp; }

            friend_expr bool operator== (Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter == right.iter; }

            friend_expr compare operator<=> (
                Iterator const& left, Iterator const& right) noexcept
                    { return left.iter <=> right.iter; }

            friend_expr Iterator operator+ (Iterator const& left,
                difference_type right) noexcept
                    { return left[right]; }
            friend_expr Iterator operator+ (difference_type left,
                Iterator const& right) noexcept
                    { return right[left]; }
            friend_expr Iterator operator- (Iterator const& left,
                difference_type right) noexcept
                    { auto temp = left; temp -= right; return temp; }
            friend_expr difference_type operator- (Iterator const& left,
                Iterator const& right) noexcept
                    { return left.iter - right.iter; }
        private:
            InnerIter                           iter;
        };

        using iterator =                            Iterator<Row, RowIter>;
        using const_iterator =                      Iterator<Row const, RowConstIter>;
        using reverse_iterator =                    std::reverse_iterator<iterator>;
        using const_reverse_iterator =              std::reverse_iterator<const_iterator>;

        constexpr iterator begin(void) noexcept
            { return iterator{rows.begin()}; }
        constexpr iterator end(void) noexcept
            { return iterator{rows.end()}; }
        constexpr const_iterator begin(void) const noexcept
            { return const_iterator{rows.begin()}; }
        constexpr const_iterator end(void) const noexcept
            { return const_iterator{rows.end()}; }

        constexpr const_iterator cbegin(void) const noexcept
            { return const_iterator{rows.cbegin()}; }
        constexpr const_iterator cend(void) const noexcept
            { return const_iterator{rows.cend()}; }

        constexpr reverse_iterator rbegin(void) noexcept
            { return reverse_iterator{end() - 1u}; }
        constexpr reverse_iterator rend(void) noexcept
            { return reverse_iterator{begin() - 1u}; }
        constexpr const_reverse_iterator rbegin(void) const noexcept
            { return const_reverse_iterator{end() - 1u}; }
        constexpr const_reverse_iterator rend(void) const noexcept
            { return const_reverse_iterator{begin() - 1u}; }

        constexpr const_reverse_iterator crbegin(void) const noexcept
            { return const_reverse_iterator{end() - 1u}; }
        constexpr const_reverse_iterator crend(void) const noexcept
            { return const_reverse_iterator{begin() - 1u}; }

        constexpr void const* getMemoryPtr(void) const noexcept
            { return memoryMap.data(); }

        constexpr void* getMemoryPtr(void) noexcept
            { return memoryMap.data(); }

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        constexpr Canva<Base, URange, URowRange>
            extract(size_type x, size_type y,
                    size_type width, size_type height) const noexcept;

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        constexpr Canva<Base, URange, URowRange>
            extract(size_vector coords, size_vector dimensions) const noexcept;

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        constexpr std::optional<Canva<Base, URange, URowRange>>
            safe_extract(size_type x, size_type y,
                size_type width, size_type height) const noexcept;

        template <UnderlyingRange<Base> URange = Range,
            UnderlyingRange<CanvaRowBase<Base>> URowRange = RowRange>
        constexpr std::optional<Canva<Base, URange, URowRange>>
            safe_extract(size_vector coords,
                size_vector dimensions) const noexcept;

        constexpr ~Canva(void) noexcept = default;
    private:
        Range                                       memoryMap;
        RowRange                                    rows;
        size_vector                                 dimensions;

        constexpr void createRows(void);
        constexpr void reassignRows(void) noexcept;
    };

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr Canva<Base, Range, RowRange>::Row&
        Canva<Base, Range, RowRange>::Row::operator= (
            Row const& row) noexcept
    {
        static_cast<BaseTuple&>(*this) =
            static_cast<BaseTuple const&>(row);
        return *this;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr Canva<Base, Range, RowRange>::Row&
        Canva<Base, Range, RowRange>::Row::operator= (Row&& row) noexcept
    {
        static_cast<BaseTuple&>(*this) = static_cast<BaseTuple&&>(row);
        return *this;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr Canva<Base, Range, RowRange>::Canva(
        size_type width, size_type height) : Canva{{width, height}} {}

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr Canva<Base, Range, RowRange>::Canva(
        size_vector const& dimensions)
            : memoryMap(dimensions[0] * dimensions[1], Base{}),
            dimensions{dimensions}
    {
        createRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr Canva<Base, Range, RowRange>::Canva(
        Canva const& canva)
            : memoryMap{canva.memoryMap.begin(),
            canva.memoryMap.end()}, dimensions{canva.dimensions}
    {
        createRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr Canva<Base, Range, RowRange>::Canva(
        Canva&& canva) noexcept
            : memoryMap{std::move(canva.memoryMap)},
            rows{std::move(canva.rows)},
            dimensions{canva.dimensions}
    {
        reassignRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr Canva<Base, Range, RowRange>&
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
    constexpr Canva<Base, Range, RowRange>&
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
    constexpr void
        Canva<Base, Range, RowRange>::reassignRows(void) noexcept
    {
        auto width = std::ref(dimensions[0]);
        for (auto& ref : rows | std::views::transform(&BaseTuple::second))
            ref = width;
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr void Canva<Base, Range, RowRange>::resize(
        size_vector const& dimensions)
    {
        this->dimensions = dimensions;
        memoryMap.resize(dimensions[1] * dimensions[0]);
        rows.clear();
        createRows();
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    constexpr void Canva<Base, Range, RowRange>::createRows(void)
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
    constexpr Canva<Base, URange, URowRange>
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
    constexpr Canva<Base, URange, URowRange>
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
    constexpr std::optional<Canva<Base, URange, URowRange>>
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
    constexpr std::optional<Canva<Base, URange, URowRange>>
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
