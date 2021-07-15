#pragma once

#include "Vector.hpp"
#include "Ranges.hpp"

#include <ranges>
#include <optional>

namespace ge {

    template <typename T, std::size_t Rows>
    constexpr decltype(auto) tupleConstructor(void) noexcept {
        if constexpr (Rows > 1)
            return std::tuple_cat(tupleConstructor<T, Rows-1>(), std::tuple<T>{});
        else
            return std::tuple<T>{};
    }

    template <AllSame... Args>
    constexpr Vector<Args...> vectorConverter(const std::tuple<Args...>& tuple) noexcept;

    template <Arithmetic T, std::size_t Rows>
    using MatrixTuple = decltype(tupleConstructor<decltype(vectorConverter(tupleConstructor<T, Rows>())), Rows>());

    template <typename T>
    concept Constant = std::is_const_v<std::remove_reference_t<T>>;

    template <typename T>
    concept NotConstant = !Constant<T>;

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    class Matrix : public MatrixTuple<T, Rows> {
    public:
        using column_value_type = decltype(vectorConverter(tupleConstructor<T, Rows>()));

        template <AllSame<column_value_type>... Args>
            requires (sizeof...(Args) == Rows)
        constexpr Matrix(const Args&... args) noexcept : MatrixTuple<T, Rows>{ args... } { std::reverse(columnBegin(), columnEnd()); transpose(); }
        constexpr Matrix(void) noexcept = default;

        static Matrix<T, Rows> identityMatrix(void) noexcept;

        constexpr std::size_t size(void) const noexcept { return Rows; }

        template <class BaseType>
        class Row {
        public:
            using value_type = std::conditional_t<std::is_const_v<BaseType>, const T, T>;

            template <Arithmetic Iter>
            class Iterator : public std::iterator<std::random_access_iterator_tag, Iter, std::ptrdiff_t, Iter*, Iter&> {
            public:
                using value_type = Iter;
                using pointer = value_type*;
                using reference = value_type&;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::random_access_iterator_tag;

                constexpr explicit Iterator(pointer iter) noexcept : iter{iter} {}
                constexpr explicit Iterator(void) noexcept = default;

                constexpr Iterator& operator++(void) noexcept { iter += Rows; return *this; }
                constexpr Iterator& operator++(int)  noexcept { auto tmp = *this; iter += Rows; return tmp; }

                constexpr Iterator& operator--(void) noexcept { iter -= Rows; return *this; }
                constexpr Iterator& operator--(int)  noexcept { auto tmp = *this; iter -= Rows; return tmp; }

                constexpr reference operator*(void) const noexcept { return *iter; }
                constexpr pointer operator->(void)  const noexcept { return iter; }

                constexpr Iterator& operator+= (std::size_t offset) noexcept { iter += offset * Rows; return *this; }
                constexpr Iterator& operator-= (std::size_t offset) noexcept { iter -= offset * Rows; return *this; }
                constexpr reference operator[] (std::size_t offset) const noexcept { return *(iter + offset * Rows); }

                friend constexpr bool operator== (const Iterator& left, const Iterator& right) noexcept { return left.iter == right.iter; }
                friend constexpr bool operator!= (const Iterator& left, const Iterator& right) noexcept { return left.iter != right.iter; }
                friend constexpr bool operator>  (const Iterator& left, const Iterator& right) noexcept { return left.iter > right.iter; }
                friend constexpr bool operator<  (const Iterator& left, const Iterator& right) noexcept { return left.iter < right.iter; }
                friend constexpr bool operator>= (const Iterator& left, const Iterator& right) noexcept { return left.iter >= right.iter; }
                friend constexpr bool operator<= (const Iterator& left, const Iterator& right) noexcept { return left.iter <= right.iter; }

                friend constexpr Iterator operator+ (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter += left * Rows; return tmp; }
                friend constexpr Iterator operator+ (std::size_t right, const Iterator& left) noexcept { auto tmp = left; tmp.iter += right * Rows; return tmp; }
                friend constexpr Iterator operator- (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter -= left * Rows; return tmp; }
                friend constexpr difference_type operator- (const Iterator& right, const Iterator& left) noexcept { return (right.iter - left.iter) / Rows; }
            private:
                pointer iter;
            };

            using iterator = Iterator<T>;
            using const_iterator = Iterator<const T>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            template <NotConstant Type = BaseType>
            constexpr T& operator[] (std::size_t index) noexcept { return *(reinterpret_cast<T*>(&reference.get()) + row + index * Rows); }
            constexpr const T& operator[] (std::size_t index) const noexcept { return *(reinterpret_cast<const T*>(&reference.get()) + row + index * Rows); }

            template <NotConstant Type = BaseType>
            constexpr iterator begin(void) noexcept { return iterator{ reinterpret_cast<T*>(&reference.get()) + row }; }
            constexpr const_iterator begin(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(&reference.get()) + row }; }

            template <NotConstant Type = BaseType>
            constexpr iterator end(void) noexcept { return iterator{ reinterpret_cast<T*>(&reference.get()) + row + Rows * Rows }; }
            constexpr const_iterator end(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(&reference.get()) + row + Rows * Rows }; }

            constexpr const_iterator cbegin(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(&reference.get()) + row }; }
            constexpr const_iterator cend(void) const noexcept { return const_iterator{ reinterpret_cast<const T*>(&reference.get()) + row + Rows * Rows }; }

            template <NotConstant Type = BaseType>
            constexpr reverse_iterator rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
            constexpr const_reverse_iterator rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }

            template <NotConstant Type = BaseType>
            constexpr reverse_iterator rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }
            constexpr const_reverse_iterator rend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

            constexpr const_reverse_iterator crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
            constexpr const_reverse_iterator crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

            constexpr Row& operator+= (const Row& left) noexcept;
            constexpr Row& operator-= (const Row& left) noexcept;
            constexpr Row& operator*= (const Row& left) noexcept;
            constexpr Row& operator/= (const Row& left);

            constexpr Row& operator+= (const column_value_type& left) noexcept;
            constexpr Row& operator-= (const column_value_type& left) noexcept;
            constexpr Row& operator*= (const column_value_type& left) noexcept;
            constexpr Row& operator/= (const column_value_type& left);

            constexpr Row& operator+= (const T& left) noexcept;
            constexpr Row& operator-= (const T& left) noexcept;
            constexpr Row& operator*= (const T& left) noexcept;
            constexpr Row& operator/= (const T& left);

            friend class Matrix<T, Rows>;
        private:
            constexpr explicit Row(const std::reference_wrapper<BaseType>& reference, std::size_t row) noexcept : row{row}, reference{reference} {}

            std::size_t row;
            std::reference_wrapper<BaseType> reference;
        };

        Row<Matrix<T, Rows>> operator[](std::size_t index) noexcept { return Row<Matrix<T, Rows>>{std::ref(*this), index}; }
        Row<const Matrix<T, Rows>> operator[] (std::size_t index) const noexcept { return Row<const Matrix<T, Rows>>{std::cref(*this), index}; }

        template <Arithmetic U>
        constexpr operator Matrix<U, Rows>() const noexcept;

        template <std::size_t URows>
            requires (URows > Rows)
        constexpr operator Matrix<T, URows>() const noexcept;

        template <typename BaseType>
        class Iterator {
        public:
            using value_type = Row<BaseType>;
            using reference = value_type;
            using difference_type = std::size_t;

            constexpr explicit Iterator(std::reference_wrapper<BaseType> ref, std::size_t row = 0) noexcept : row{row}, ref{ref} {}

            constexpr Iterator& operator++(void) noexcept { ++row; return *this; }
            constexpr Iterator& operator++(int) noexcept { auto tmp = *this; ++row; return tmp; }

            constexpr Iterator& operator--(void) noexcept { --row; return *this; }
            constexpr Iterator& operator--(int) noexcept { auto tmp = *this; --row; return tmp; }

            constexpr reference operator*(void) const noexcept { return value_type{ref, row}; }

            constexpr Iterator& operator+= (std::size_t offset) noexcept { row += offset; return *this; }
            constexpr Iterator& operator-= (std::size_t offset) noexcept { row -= offset; return *this; }
            constexpr reference operator[] (std::size_t offset) const noexcept { return Row{ref, row + offset}; }

            friend constexpr bool operator== (const Iterator& left, const Iterator& right) noexcept { return left.row == right.row; }
            friend constexpr bool operator!= (const Iterator& left, const Iterator& right) noexcept { return left.row != right.row; }
            friend constexpr bool operator>  (const Iterator& left, const Iterator& right) noexcept { return left.row > right.row; }
            friend constexpr bool operator<  (const Iterator& left, const Iterator& right) noexcept { return left.row < right.row; }
            friend constexpr bool operator>= (const Iterator& left, const Iterator& right) noexcept { return left.row >= right.row; }
            friend constexpr bool operator<= (const Iterator& left, const Iterator& right) noexcept { return left.row <= right.row; }

            friend constexpr Iterator operator+ (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.row += left; return tmp; }
            friend constexpr Iterator operator+ (std::size_t right, const Iterator& left) noexcept { auto tmp = left; tmp.row += right; return tmp; }
            friend constexpr Iterator operator- (const Iterator& right, std::size_t left) noexcept { auto tmp = right; tmp.row -= left; return tmp; }
            friend constexpr difference_type operator- (const Iterator& right, const Iterator& left) noexcept { return right.row - left.row; }
        private:
            std::size_t row;
            std::reference_wrapper<BaseType> ref;
        };

        using iterator = Iterator<Matrix<T, Rows>>;
        using const_iterator = Iterator<const Matrix<T, Rows>>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr iterator begin(void) noexcept { return iterator{ std::ref(*this) }; }
        constexpr iterator end(void) noexcept { return iterator{ std::ref(*this), Rows}; }

        constexpr const_iterator begin(void) const noexcept { return const_iterator{ std::cref(*this) }; }
        constexpr const_iterator end(void) const noexcept { return const_iterator{ std::cref(*this), Rows}; }

        constexpr const_iterator cbegin(void) const noexcept { return const_iterator{ std::cref(*this) }; }
        constexpr const_iterator cend(void) const noexcept { return const_iterator{ std::cref(*this), Rows}; }

        constexpr reverse_iterator rbegin(void) noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr reverse_iterator rend(void) noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr const_reverse_iterator rbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr const_reverse_iterator rend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        constexpr const_reverse_iterator crbegin(void) const noexcept { return std::reverse_iterator{ end() - 1 }; }
        constexpr const_reverse_iterator crend(void) const noexcept { return std::reverse_iterator{ begin() - 1 }; }

        template <class Iter>
        class ColumnIterator : public std::iterator<std::contiguous_iterator_tag, Iter, std::ptrdiff_t, Iter*, Iter&> {
        public:
            using value_type = Iter;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::contiguous_iterator_tag;

            constexpr explicit ColumnIterator(pointer iter) noexcept : iter{iter} {}
            constexpr explicit ColumnIterator(void) noexcept = default;

            constexpr ColumnIterator& operator++(void) noexcept { ++iter; return *this; }
            constexpr ColumnIterator& operator++(int)  noexcept { auto tmp = *this; ++iter; return tmp; }

            constexpr ColumnIterator& operator--(void) noexcept { --iter; return *this; }
            constexpr ColumnIterator& operator--(int)  noexcept { auto tmp = *this; --iter; return tmp; }

            constexpr reference operator*(void) const noexcept { return *iter; }
            constexpr pointer operator->(void)  const noexcept { return iter; }

            constexpr ColumnIterator& operator+= (std::size_t offset) noexcept { iter += offset; return *this; }
            constexpr ColumnIterator& operator-= (std::size_t offset) noexcept { iter -= offset; return *this; }
            constexpr reference operator[] (std::size_t offset) const noexcept { return *(iter + offset); }

            friend constexpr bool operator== (const ColumnIterator& left, const ColumnIterator& right) noexcept { return left.iter == right.iter; }
            friend constexpr bool operator!= (const ColumnIterator& left, const ColumnIterator& right) noexcept { return left.iter != right.iter; }
            friend constexpr bool operator>  (const ColumnIterator& left, const ColumnIterator& right) noexcept { return left.iter > right.iter; }
            friend constexpr bool operator<  (const ColumnIterator& left, const ColumnIterator& right) noexcept { return left.iter < right.iter; }
            friend constexpr bool operator>= (const ColumnIterator& left, const ColumnIterator& right) noexcept { return left.iter >= right.iter; }
            friend constexpr bool operator<= (const ColumnIterator& left, const ColumnIterator& right) noexcept { return left.iter <= right.iter; }

            friend constexpr ColumnIterator operator+ (const ColumnIterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter += left; return tmp; }
            friend constexpr ColumnIterator operator+ (std::size_t right, const ColumnIterator& left) noexcept { auto tmp = left; tmp.iter += right; return tmp; }
            friend constexpr ColumnIterator operator- (const ColumnIterator& right, std::size_t left) noexcept { auto tmp = right; tmp.iter -= left; return tmp; }
            friend constexpr difference_type operator- (const ColumnIterator& right, const ColumnIterator& left) noexcept { return right.iter - left.iter; }
        private:
            pointer iter;
        };

        using column_iterator = ColumnIterator<column_value_type>;
        using const_column_iterator = ColumnIterator<const column_value_type>;
        using reverse_column_iterator = std::reverse_iterator<column_iterator>;
        using const_reverse_column_iterator = std::reverse_iterator<const_column_iterator>;

        constexpr column_iterator columnBegin(void) noexcept { return column_iterator{ reinterpret_cast<column_value_type*>(this) }; }
        constexpr column_iterator columnEnd(void) noexcept { return column_iterator{ reinterpret_cast<column_value_type*>(this) + Rows }; }

        constexpr const_column_iterator columnBegin(void) const noexcept { return const_column_iterator{ reinterpret_cast<const column_value_type*>(this) }; }
        constexpr const_column_iterator columnEnd(void) const noexcept { return const_column_iterator{ reinterpret_cast<const column_value_type*>(this) + Rows }; }

        constexpr const_column_iterator columnCBegin(void) const noexcept { return const_column_iterator{ reinterpret_cast<const column_value_type*>(this) }; }
        constexpr const_column_iterator columnCEnd(void) const noexcept { return const_column_iterator{ reinterpret_cast<const column_value_type*>(this) + Rows }; }

        constexpr reverse_column_iterator columnRBegin(void) noexcept { return std::reverse_iterator{ columnEnd() - 1 }; }
        constexpr reverse_column_iterator columnREnd(void) noexcept { return std::reverse_iterator{ columnBegin() - 1 }; }

        constexpr const_reverse_column_iterator columnRBegin(void) const noexcept { return std::reverse_iterator{ columnEnd() - 1 }; }
        constexpr const_reverse_column_iterator columnREnd(void) const noexcept { return std::reverse_iterator{ columnBegin() - 1 }; }

        constexpr const_reverse_column_iterator columnCRBegin(void) const noexcept { return std::reverse_iterator{ columnEnd() - 1 }; }
        constexpr const_reverse_column_iterator columnCREnd(void) const noexcept { return std::reverse_iterator{ columnBegin() - 1 }; }

        constexpr column_value_type& getColumn(std::size_t index) noexcept { return *(reinterpret_cast<column_value_type*>(this) + index); }
        constexpr const column_value_type& getColumn(std::size_t index) const noexcept { return *(reinterpret_cast<const column_value_type*>(this) + index); }

        template <typename U = T, std::size_t MinorRows = Rows - 1>
            requires (MinorRows > 1)
        constexpr Matrix<U, Rows - 1> minor(std::size_t row, std::size_t column) const noexcept;

        constexpr Matrix& transpose(void) noexcept;

        template <typename U = T>
        constexpr U determinent(U init = {0}, U positive = {1}, U negative = {-1}) const noexcept;

        template <typename U = T>
        std::optional<Matrix<U, Rows>> inverse(void) const noexcept;

        constexpr Matrix& operator+= (const Matrix& right) noexcept;
        constexpr Matrix& operator-= (const Matrix& right) noexcept;
        constexpr Matrix& operator*= (const Matrix& right) noexcept;

    };

    template <Arithmetic T, std::size_t Rows>
    using MatVector = typename Matrix<T, Rows>::column_value_type;

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    std::optional<MatVector<T, Rows>> luDecomposition(Matrix<T, Rows>& matrix) noexcept {
        MatVector<T, Rows> permutations;
        std::iota(permutations.begin(), permutations.end(), 0);
        for (std::size_t k = 0, kp; k < Rows; ++k) {
            T p{0};
            for (std::size_t i = k; i < Rows; ++i) {
                if (T v = std::abs(matrix.getColumn(k)[i]); v > p) {
                    p = v;
                    kp = i;
                }
            }
            if (!p) return {};
            std::swap(permutations[k], permutations[kp]);
            for (std::size_t i = 0;i < Rows; ++i)
                std::swap(matrix.getColumn(i)[k], matrix.getColumn(i)[kp]);
            for (std::size_t i = k + 1; i < Rows; ++i) {
                matrix.getColumn(k)[i] /= matrix.getColumn(k)[k];
                for (std::size_t j = k + 1; j < Rows; ++j)
                    matrix.getColumn(j)[i] -= matrix.getColumn(k)[i] * matrix.getColumn(j)[k];
            }
        }
        return { permutations };
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    MatVector<T, Rows> lupSolve(const Matrix<T, Rows>& luMatrix, const MatVector<T, Rows>& permutations, const MatVector<T, Rows>& vector) noexcept {
        MatVector<T, Rows> result;
        MatVector<T, Rows> helper;
        for (std::size_t i = 0; i < Rows; ++i) {
            T sum{0};
            for (std::size_t j = 0;j < i - 1; ++j)
                sum += luMatrix.getColumn(j)[i] * helper[j];
            helper[i] = vector[permutations[i]] - sum;
        }
        for (std::size_t i = Rows - 1; i < Rows; --i) {
            T sum{0};
            for (std::size_t j = i + 1; j < Rows; ++j)
                sum += luMatrix.getColumn(j)[i] * result[j];
            result[i] = (helper[i] - sum) / luMatrix.getColumn(i)[i];
        }
        return result;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <typename U>
    std::optional<Matrix<U, Rows>> Matrix<T, Rows>::inverse(void) const noexcept {
        Matrix<U, Rows> luMatrix = *this;
        auto idMatrix = Matrix<U, Rows>::identityMatrix();
        if (auto permutations = luDecomposition(luMatrix)) {
            Matrix<U, Rows> inverseMatrix;
            for (std::size_t i = 0;i < Rows; ++i)
                inverseMatrix.getColumn(i) = lupSolve(luMatrix, *permutations, idMatrix.getColumn(i));
            return { inverseMatrix };
        }
        return {};
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    Matrix<T, Rows> Matrix<T, Rows>::identityMatrix(void) noexcept {
        Matrix<T, Rows> matrix;
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < Rows; ++j)
                matrix.getColumn(i)[j] = i == j ? T{1} : T{0};
        return matrix;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <Arithmetic U>
    constexpr Matrix<T, Rows>::operator Matrix<U, Rows>() const noexcept {
        Matrix<U, Rows> base;
        std::ranges::copy(*this, base);
        return base;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <std::size_t URows>
        requires (URows > Rows)
    constexpr Matrix<T, Rows>::operator Matrix<T, URows>() const noexcept {
        Matrix<T, URows> base;
        std::ranges::copy(*this, base);
        return base;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::operator+= (const Row& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left + right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::operator-= (const Row& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left - right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::operator*= (const Row& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left * right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::operator/= (const Row& right) {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left / right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::operator+= (const column_value_type& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left + right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::operator-= (const column_value_type& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left - right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::Row::operator*= (const column_value_type& right) noexcept {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left * right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::Row::operator/= (const column_value_type& right) {
        std::ranges::transform(*this, right, begin(), [](const T& left, const T& right)->T{ return left / right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::Row::operator+= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value += right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::Row::operator-= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value -= right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::Row::operator*= (const T& right) noexcept {
        std::ranges::for_each(*this, [&right](T& value) -> void { value *= right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <class BaseType>
    constexpr Matrix<T, Rows>::Row<BaseType>& Matrix<T, Rows>::Row<BaseType>::Row::operator/= (const T& right) {
        std::ranges::for_each(*this, [&right](T& value) -> void { value /= right; });
        return *this;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator+ (const typename Matrix<T, Rows>::Row<BaseType>& left, const typename Matrix<T, Rows>::column_value_type& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left + right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator- (const typename Matrix<T, Rows>::Row<BaseType>& left, const typename Matrix<T, Rows>::column_value_type& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left - right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator* (const typename Matrix<T, Rows>::Row<BaseType>& left, const typename Matrix<T, Rows>::column_value_type& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left * right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator/ (const typename Matrix<T, Rows>::Row<BaseType>& left, const typename Matrix<T, Rows>::column_value_type& right) {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left / right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator+ (const typename Matrix<T, Rows>::column_value_type& left, const typename Matrix<T, Rows>::Row<BaseType>& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left + right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator- (const typename Matrix<T, Rows>::column_value_type& left, const typename Matrix<T, Rows>::Row<BaseType>& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left - right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator* (const typename Matrix<T, Rows>::column_value_type& left, const typename Matrix<T, Rows>::Row<BaseType>& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left * right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator/ (const typename Matrix<T, Rows>::column_value_type& left, const typename Matrix<T, Rows>::Row<BaseType>& right) {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, right, vector.begin(), [](const T& left, const T& right)->T{ return left / right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator+ (const typename Matrix<T, Rows>::Row<BaseType>& left, const T& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, vector.begin(), [&right](const T& left) -> T { return left + right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator- (const typename Matrix<T, Rows>::Row<BaseType>& left, const T& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, vector.begin(), [&right](const T& left) -> T { return left - right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator* (const typename Matrix<T, Rows>::Row<BaseType>& left, const T& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, vector.begin(), [&right](const T& left) -> T { return left * right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator/ (const typename Matrix<T, Rows>::Row<BaseType>& left, const T& right) {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(left, vector.begin(), [&right](const T& left) -> T { return left / right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator+ (const T& left, const typename Matrix<T, Rows>::Row<BaseType>& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(right, vector.begin(), [&left](const T& right) -> T { return left + right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator- (const T& left, const typename Matrix<T, Rows>::Row<BaseType>& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(right, vector.begin(), [&left](const T& right) -> T { return left - right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows, class BaseType>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator* (const T& left, const typename Matrix<T, Rows>::Row<BaseType>& right) noexcept {
        typename Matrix<T, Rows>::column_value_type vector;
        std::ranges::transform(right, vector.begin(), [&left](const T& right) -> T { return left * right; });
        return vector;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>& Matrix<T, Rows>::transpose(void) noexcept {
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < i; ++j)
                std::swap((*this)[i][j], (*this)[j][i]);
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>& Matrix<T, Rows>::operator+= (const Matrix& right) noexcept {
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < i; ++j)
                this->getColumn(i)[j] += right.getColumn(i)[j];
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>& Matrix<T, Rows>::operator-= (const Matrix& right) noexcept {
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < i; ++j)
                this->getColumn(i)[j] -= right.getColumn(i)[j];
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>& Matrix<T, Rows>::operator*= (const Matrix& right) noexcept {
        Matrix<T, Rows> result;
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < i; ++j)
                result[i][j] = innerProduct((*this)[i], right.getColumn(j), T{0});
        *this = result;
        return *this;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    constexpr Matrix<T, Rows> operator* (const Matrix<T, Rows>& leftMatrix, const Matrix<T, Rows>& rightMatrix) noexcept {
        Matrix<T, Rows> result;
        for (std::size_t i = 0;i < Rows; ++i)
            for (std::size_t j = 0;j < i; ++j)
                result[i][j] = innerProduct(leftMatrix[i], rightMatrix.getColumn(j), T{0});
        return result;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    constexpr Matrix<T, Rows>::column_value_type operator*(const Matrix<T, Rows>& matrix, const typename Matrix<T, Rows>::column_value_type& vector) noexcept {
        typename Matrix<T, Rows>::column_value_type result;
        std::transform(matrix.begin(), matrix.end(), result.begin(), [&vector](const auto& row)->T{ return innerProduct(row, vector, T{0}); });
        return result;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <typename U, std::size_t MinorRows>
        requires (MinorRows > 1)
    constexpr Matrix<U, Rows - 1> Matrix<T, Rows>::minor(std::size_t row, std::size_t column) const noexcept {
        Matrix<U, Rows - 1> result;
        for (std::size_t i = 0, ri = 0; i < Rows; ++i) {
            if (i != row) {
                for (std::size_t j = 0, rj = 0;j < Rows; ++j) {
                    if (j != column) {
                        result.getColumn(rj)[ri] = static_cast<U>(getColumn(j)[i]);
                        ++rj;
                    }
                }
                ++ri;
            }
        }
        return result;
    }

    template <Arithmetic T, std::size_t Rows>
        requires (Rows > 1)
    template <typename U>
    constexpr U Matrix<T, Rows>::determinent(U init, U positive, U negative) const noexcept {
        if constexpr (Rows == 2)
            init = static_cast<U>(getColumn(0)[0] * getColumn(1)[1]) - static_cast<U>(getColumn(0)[1] * getColumn(1)[0]);
        else {
            for (std::size_t i = 0;i < Rows; ++i)
                init += (i % 2 ? negative : positive) * static_cast<U>(getColumn(i)[0]) * minor<U>(0, i).determinent();
        }
        return init;
    }


}