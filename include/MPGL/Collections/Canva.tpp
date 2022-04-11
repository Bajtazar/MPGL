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

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    template <std::integral Tp>
    [[nodiscard]] constexpr Base&
        Canva<Base, Range, RowRange>::operator[](
            Vector2<Tp> const& coords) noexcept
    {
        return memoryMap[dimensions[0] * coords[1] + coords[0]];
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    template <std::integral Tp>
    [[nodiscard]] constexpr Base const&
        Canva<Base, Range, RowRange>::operator[](
            Vector2<Tp> const& coords) const noexcept
    {
        return memoryMap[dimensions[0] * coords[1] + coords[0]];
    }

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
        Canva<Base, Range, RowRange>::Row::operator= (
            Row&& row) noexcept
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
        for (auto& ref : rows | std::views::transform(
            &BaseTuple::second))
        {
            ref = width;
        }
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
    [[nodiscard]] constexpr Canva<Base, URange, URowRange>
        Canva<Base, Range, RowRange>::extract(
            size_type x,
            size_type y,
            size_type width,
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
    [[nodiscard]] constexpr Canva<Base, URange, URowRange>
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
    [[nodiscard]] constexpr std::optional<
        Canva<Base, URange, URowRange>>
            Canva<Base, Range, RowRange>::safe_extract(
                size_type x,
                size_type y,
                size_type width,
                size_type height) const noexcept
    {
        if (x + width > dimensions[0] || y + height > dimensions[1])
            return {};
        return { extract<URange, URowRange>(x, y, width, height) };
    }

    template <DefaultBaseType Base, UnderlyingRange<Base> Range,
        UnderlyingRange<CanvaRowBase<Base>> RowRange>
    template <UnderlyingRange<Base> URange,
        UnderlyingRange<CanvaRowBase<Base>> URowRange>
    [[nodiscard]] constexpr std::optional<
        Canva<Base, URange, URowRange>>
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
