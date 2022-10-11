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

#include <algorithm>

namespace mpgl {

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>::ColumnView(
        MatrixTp&& matrix,
        std::size_t columnID) noexcept
            : matrix{std::forward<MatrixTp>(matrix)},
            columnID{columnID} {}

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator=(
            vector_form const& vector) noexcept
    {
        std::ranges::copy(vector, begin());
        return *this;
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::operator
        vector_form() const noexcept
    {
        vector_form vector;
        std::ranges::copy(*this, vector.begin());
        return vector;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>::iterator::iterator(
        MatrixTp&& matrix,
        std::size_t columnID,
        std::size_t rowID) noexcept
            : matrix{std::forward<MatrixTp>(matrix)},
            columnID{columnID}, rowID{rowID} {}

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>::iterator&
        ColumnView<MatrixTp>::iterator::operator++(void) noexcept
    {
        ++rowID;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::iterator
        ColumnView<MatrixTp>::iterator::operator++(int) noexcept
    {
        auto temp = *this;
        ++rowID;
        return temp;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>::iterator&
        ColumnView<MatrixTp>::iterator::operator--(void) noexcept
    {
        --rowID;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::iterator
        ColumnView<MatrixTp>::iterator::operator--(int) noexcept
    {
        auto temp = *this;
        --rowID;
        return temp;
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::iterator::reference
        ColumnView<MatrixTp>::iterator::operator*(
            void) const noexcept
    {
        return matrix.base()[rowID][columnID];
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::iterator::pointer
        ColumnView<MatrixTp>::iterator::operator->(
            void) const noexcept
    {
        return std::addressof(matrix.base()[rowID][columnID]);
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>::iterator&
        ColumnView<MatrixTp>::iterator::operator+=(
            difference_type offset) noexcept
    {
        rowID += offset;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>::iterator&
        ColumnView<MatrixTp>::iterator::operator-=(
            difference_type offset) noexcept
    {
        rowID -= offset;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::iterator::reference
        ColumnView<MatrixTp>::iterator::operator[](
            difference_type offset) const noexcept
    {
        return matrix.base()[rowID + offset][columnID];
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::reference
        ColumnView<MatrixTp>::operator[] (
            std::size_t index) const noexcept
    {
        return matrix.base()[index][columnID];
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator+=(
            ColumnView const& right) noexcept
    {
        for (size_t i = 0; i < size(); ++i)
            (*this)[i] += right[i];
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator-=(
            ColumnView const& right) noexcept
    {
        for (size_t i = 0; i < size(); ++i)
            (*this)[i] -= right[i];
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator*=(
            ColumnView const& right) noexcept
    {
        for (size_t i = 0; i < size(); ++i)
            (*this)[i] *= right[i];
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator/=(
            ColumnView const& right) noexcept
    {
        for (size_t i = 0; i < size(); ++i)
            (*this)[i] /= right[i];
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator+=(
            value_type const& right) noexcept
    {
        for (auto& element : *this)
            element += right;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator-=(
            value_type const& right) noexcept
    {
        for (auto& element : *this)
            element -= right;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator*=(
            value_type const& right) noexcept
    {
        for (auto& element : *this)
            element *= right;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    constexpr ColumnView<MatrixTp>&
        ColumnView<MatrixTp>::operator/=(
            value_type const& right) noexcept
    {
        for (auto& element : *this)
            element /= right;
        return *this;
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::iterator
        ColumnView<MatrixTp>::begin(void) const noexcept
            { return iterator{ matrix.base(), columnID }; }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::iterator
        ColumnView<MatrixTp>::end(void) const noexcept
    {
        return iterator{
            matrix.base(),
            columnID,
            matrix.base().size()
        };
    }

    template <details::MatrixView Tp>
    constexpr ColumnRangeView<Tp>::iterator::iterator(
        Tp&& matrix,
        std::size_t columnID) noexcept
            : matrix{std::forward<Tp>(matrix)}, columnID{columnID} {}

    template <details::MatrixView Tp>
    constexpr ColumnRangeView<Tp>::iterator&
        ColumnRangeView<Tp>::iterator::operator++(void) noexcept
    {
        ++columnID;
        return *this;
    }

    template <details::MatrixView Tp>
    [[nodiscard]] constexpr ColumnRangeView<Tp>::iterator
        ColumnRangeView<Tp>::iterator::operator++(int) noexcept
    {
        auto temp = *this;
        ++columnID;
        return temp;
    }

    template <details::MatrixView Tp>
    [[nodiscard]] constexpr ColumnRangeView<Tp>::iterator::value_type
        ColumnRangeView<Tp>::iterator::operator*(
            void) const noexcept
    {
        return value_type{matrix.base(), columnID};
    }

    template <details::MatrixView Tp>
    [[nodiscard]] constexpr ColumnRangeView<Tp>::iterator
        ColumnRangeView<Tp>::begin(void) const noexcept
    {
        return iterator{matrix.base()};
    }

    template <details::MatrixView Tp>
    [[nodiscard]] constexpr ColumnRangeView<Tp>::iterator
        ColumnRangeView<Tp>::end(void) const noexcept
    {
        return iterator{matrix.base(), matrix.base().width()};
    }

    template <details::MatrixView MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left + right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left - right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left * right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left / right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
                requires mpgl_Operable(
                    typename ColumnView<MatrixTp>::value_type, %)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left % right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
                requires mpgl_Operable(
                    typename ColumnView<MatrixTp>::value_type, ^)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left ^ right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
                requires mpgl_Operable(
                    typename ColumnView<MatrixTp>::value_type, &)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left & right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator|(
            typename ColumnView<MatrixTp>::vector_form const& left,
            ColumnView<MatrixTp> const& right) noexcept
                requires mpgl_Operable(
                    typename ColumnView<MatrixTp>::value_type, |)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left | right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator+(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left + right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator-(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left - right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator*(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left * right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator/(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right) noexcept
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left / right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator%(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, %)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left % right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator^(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, ^)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left ^ right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator&(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, &)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left & right;
            }
        );
        return vector;
    }

    template <MatrixType MatrixTp>
    [[nodiscard]] constexpr ColumnView<MatrixTp>::vector_form
        operator|(
            ColumnView<MatrixTp> const& left,
            typename ColumnView<MatrixTp>::vector_form const& right
            ) noexcept requires mpgl_Operable(
                typename ColumnView<MatrixTp>::value_type, |)
    {
        typename ColumnView<MatrixTp>::vector_form vector;
        std::ranges::transform(left, right, vector.begin(),
            [](auto const& left, auto const& right) {
                return left | right;
            }
        );
        return vector;
    }

}

namespace mpgl::details {

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto
        ColumnViewAdaptorClosure::operator() (
            Tp&& matrix) const noexcept
    {
        return ColumnView{std::forward<Tp>(matrix), rowID};
    }

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto ColumnViewAdaptor::operator() (
        Tp&& matrix,
        std::size_t rowID) const noexcept
    {
        return ColumnView{std::forward<Tp>(matrix), rowID};
    }

    [[nodiscard]] constexpr ColumnViewAdaptorClosure
        ColumnViewAdaptor::operator() (
            std::size_t rowID) const noexcept
    {
        return ColumnViewAdaptorClosure{rowID};
    }

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto
        ColumnRangeViewAdaptorClosure::operator() (
            Tp&& matrix) const noexcept
    {
        return ColumnRangeView{std::forward<Tp>(matrix)};
    }

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto
        ColumnRangeViewAdaptor::operator() (
            Tp&& matrix) const noexcept
    {
        return ColumnRangeView{std::forward<Tp>(matrix)};
    }

    [[nodiscard]] constexpr ColumnRangeViewAdaptorClosure
        ColumnRangeViewAdaptor::operator() (
            void) const noexcept
    {
        return ColumnRangeViewAdaptorClosure{};
    }

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto operator | (
        Tp&& matrix,
        ColumnViewAdaptorClosure const& closure) noexcept
    {
        return closure(std::forward<Tp>(matrix));
    }

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto operator | (
        Tp&& matrix,
        ColumnViewAdaptor const& adaptor) noexcept
    {
        return adaptor(std::forward<Tp>(matrix));
    }

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto operator | (
        Tp&& matrix,
        ColumnRangeViewAdaptorClosure const& closure) noexcept
    {
        return closure(std::forward<Tp>(matrix));
    }

    template <MatrixType Tp>
    [[nodiscard]] constexpr auto operator | (
        Tp&& matrix,
        ColumnRangeViewAdaptor const& adaptor) noexcept
    {
        return adaptor(std::forward<Tp>(matrix));
    }

}
