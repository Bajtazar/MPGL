#pragma once

#include "Vector.hpp"
#include "Ranges.hpp"

#include <optional>

namespace mpgl {

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    class Matrix {
    public:
        #pragma pack(push, 1)
        class Column {
        public:
            constexpr explicit Column(void) noexcept = delete;
            constexpr Column(Column const& column) noexcept = delete;
            constexpr Column(Column&& column) noexcept
                : seed{std::move(column.seed)} {}

            constexpr Column& operator=(
                Column const& column) noexcept = delete;
            constexpr Column& operator=(Column&& column) noexcept
                    { seed = std::move(column.seed); return *this; }

            constexpr Column& operator=(
                Vector<Tp, Rows> const& vec) noexcept;

            constexpr operator Vector<Tp, Rows>() const noexcept;

            static constexpr std::size_t size(void) noexcept
                { return Rows; }

            template <Arithmetic Up = Tp>
            constexpr Up length(Up init = {0}) const noexcept;

            constexpr Column& operator+=(Column const& right);
            constexpr Column& operator-=(Column const& right);
            constexpr Column& operator*=(Column const& right);
            constexpr Column& operator/=(Column const& right);

            constexpr Column& operator%=(Column const& right)
                requires mpgl_Operable(Tp, %);
            constexpr Column& operator^=(Column const& right)
                requires mpgl_Operable(Tp, ^);
            constexpr Column& operator&=(Column const& right)
                requires mpgl_Operable(Tp, &);
            constexpr Column& operator|=(Column const& right)
                requires mpgl_Operable(Tp, |);

            constexpr Column& operator+=(Tp const& right);
            constexpr Column& operator-=(Tp const& right);
            constexpr Column& operator*=(Tp const& right);
            constexpr Column& operator/=(Tp const& right);

            constexpr Column& operator%=(Tp const& right)
                requires mpgl_Operable(Tp, %);
            constexpr Column& operator^=(Tp const& right)
                requires mpgl_Operable(Tp, ^);
            constexpr Column& operator&=(Tp const& right)
                requires mpgl_Operable(Tp, &);
            constexpr Column& operator|=(Tp const& right)
                requires mpgl_Operable(Tp, |);

            template <Arithmetic value_type>
            class Iterator : public std::iterator<
                std::random_access_iterator_tag, value_type>
            {
            public:
                using iterator_category =       std::random_access_iterator_tag;
                using reference =               value_type&;
                using pointer =                 value_type*;
                using const_pointer =           pointer const;
                using difference_type =         decltype(
                    std::declval<pointer>() - std::declval<pointer>());
                using iter =                    Iterator;
                using iter_ref =                Iterator&;
                using iter_cref =               Iterator const&;
                using compare =
                    std::compare_three_way_result_t<pointer, pointer>;

                constexpr explicit Iterator(pointer ptr) noexcept
                    : ptr{ptr} {}
                constexpr explicit Iterator(void) noexcept = default;

                constexpr iter_ref operator++(void) noexcept
                    { ptr += Cols; return *this; }
                constexpr iter operator++(int) noexcept
                    { auto temp = *this; ++(*this); return temp; }
                constexpr iter_ref operator--(void) noexcept
                    { ptr -= Cols; return *this; }
                constexpr iter operator--(int) noexcept
                    { auto temp = *this; --(*this); return temp; }

                constexpr reference operator*(void) const noexcept
                    { return *ptr; }
                constexpr const_pointer operator->(void) const noexcept
                    { return ptr; }

                constexpr iter_ref operator+=(difference_type offset) noexcept
                    { ptr += offset * Cols; return *this; }
                constexpr iter_ref operator-=(difference_type offset) noexcept
                    { ptr -= offset * Cols; return *this; }
                constexpr reference operator[](difference_type offset) const noexcept
                    { return *(ptr + offset * Cols); }

                friend_expr iter operator+(iter_cref left,
                    difference_type right) noexcept
                        { auto temp = left; temp.ptr += right; return temp; }
                friend_expr iter operator+(difference_type left,
                    iter_cref right) noexcept
                        { auto temp = right; temp.ptr += left; return temp; }
                friend_expr iter operator-(iter_cref left,
                    difference_type right) noexcept
                        { auto temp = left; temp.ptr -= right; return temp; }
                friend_expr difference_type operator-(iter_cref left,
                    iter_cref right) noexcept
                        { return left.ptr - right.ptr; }

                friend_expr bool operator==(iter_cref left,
                    iter_cref right) noexcept
                        { return left.ptr == right.ptr; }

                friend_expr compare operator<=>(iter_cref left,
                    iter_cref right) noexcept
                        { return left.ptr <=> right.ptr; }
            private:
                pointer                         ptr;
            };

            typedef Iterator<Tp>                iterator;
            typedef Iterator<Tp const>          const_iterator;
            typedef std::reverse_iterator<
                iterator>                       reverse_iterator;
            typedef std::reverse_iterator<
                const_iterator>                 const_reverse_iterator;

            constexpr iterator begin(void) noexcept
                { return iterator{ &seed }; }
            constexpr iterator end(void) noexcept
                { return iterator{ &seed + Rows * Cols }; }

            constexpr const_iterator begin(void) const noexcept
                { return const_iterator{ &seed }; }
            constexpr const_iterator end(void) const noexcept
                { return const_iterator{ &seed + Rows * Cols }; }

            constexpr const_iterator cbegin(void) const noexcept
                { return const_iterator{ &seed }; }
            constexpr const_iterator cend(void) const noexcept
                { return const_iterator{ &seed + Rows * Cols }; }

            constexpr reverse_iterator rbegin(void) noexcept
                { return reverse_iterator{ end() - 1 }; }
            constexpr reverse_iterator rend(void) noexcept
                { return reverse_iterator{ begin() - 1}; }

            constexpr const_reverse_iterator rbegin(void) const noexcept
                { return const_reverse_iterator{ end() - 1 }; }
            constexpr const_reverse_iterator rend(void) const noexcept
                { return const_reverse_iterator{ begin() - 1}; }

            constexpr const_reverse_iterator crbegin(void) const noexcept
                { return const_reverse_iterator{ cend() - 1 }; }
            constexpr const_reverse_iterator crend(void) const noexcept
                { return const_reverse_iterator{ cbegin() - 1}; }

            constexpr Tp& operator[] (std::size_t index) noexcept
                { return *(&seed + index * Cols); }
            constexpr Tp const& operator[] (std::size_t index) const noexcept
                { return *(&seed + index * Cols); }

            constexpr ~Column(void) noexcept = default;
        private:
            Tp                                      seed;
        };
        #pragma pack(pop)
    private:
        typedef TensorTuple<Column, Rows, Cols> ColumnBase;
    public:
        typedef TensorTuple<Tp, Rows, Cols>     NormalBase;
        typedef Vector<Tp, Cols>                value_type;
        typedef Column                          column_value_type;

        template <AllAbsolutelySame<value_type>... Rws>
            requires (sizeof...(Rws) == Rows)
        constexpr Matrix(Rws&&... rows) noexcept;

        constexpr Matrix(NormalBase const& base) noexcept
            : normalBase{base} {}

        constexpr Matrix(NormalBase&& base) noexcept
            : normalBase{std::move(base)} {}

        constexpr Matrix(Matrix const& matrix) noexcept
            : normalBase{matrix.normalBase} {}

        constexpr Matrix(Matrix&& matrix) noexcept
            : normalBase{std::move(matrix.normalBase)} {}

        constexpr Matrix(void) noexcept
            : normalBase{} {}

        constexpr Matrix& operator=(Matrix const&
            matrix) noexcept
                { normalBase = matrix.normalBase; return *this; }

        constexpr Matrix& operator=(Matrix&& matrix) noexcept
            { normalBase = std::move(matrix.normalBase); return *this; }

        constexpr Matrix operator-(void) const noexcept;

        static constexpr std::size_t size(void) noexcept
            { return Rows; }

        template <class value_type>
        class Iterator : public std::iterator<
            std::contiguous_iterator_tag, value_type>
        {
        public:
            using pointer                     = value_type*;
            using reference                   = value_type&;
            using difference_type             = std::ptrdiff_t;
            using iterator_category           = std::contiguous_iterator_tag;
            using iter                        = Iterator;
            using iter_ref                    = Iterator&;
            using iter_cref                   = Iterator const&;
            using compare
                = std::compare_three_way_result_t<pointer, pointer>;

            constexpr explicit Iterator(pointer ptr) noexcept
                : ptr{ptr} {}

            constexpr explicit Iterator(void) noexcept = default;

            constexpr iter_ref operator++(void) noexcept
                { ++ptr; return *this; }
            constexpr iter operator++(int) noexcept
                { auto temp = *this; ++ptr; return temp; }

            constexpr iter_ref operator--(void) noexcept
                { --ptr; return *this; }
            constexpr iter operator--(int) noexcept
                { auto temp = *this; --ptr; return temp; }

            constexpr reference operator*(void) const noexcept
                { return *ptr; }
            constexpr pointer operator->(void) const noexcept
                { return ptr; }

            constexpr iter_ref operator+=(difference_type offset) noexcept
                { ptr += offset; return *this; }
            constexpr iter_ref operator-=(difference_type offset) noexcept
                { ptr -= offset; return *this; }

            constexpr reference operator[] (difference_type offset) const noexcept
                { return *(ptr + offset); }

            friend_expr iter operator+(iter_cref left,
                difference_type right) noexcept
                    { auto temp = left; temp.ptr += right; return temp; }
            friend_expr iter operator+(difference_type left,
                iter_cref right) noexcept
                    { auto temp = right; temp.ptr += left; return temp; }
            friend_expr iter operator-(iter_cref left,
                difference_type right) noexcept
                    { auto temp = left; temp.ptr -= right; return temp; }
            friend_expr difference_type operator-(iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr - right.ptr; }

            friend_expr bool operator==(iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr == right.ptr; }

            friend_expr compare operator<=>(iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr <=> right.ptr; }
        private:
            pointer                             ptr;
        };

        typedef Iterator<value_type>        iterator;
        typedef Iterator<value_type const>  const_iterator;
        typedef std::reverse_iterator<
            iterator>                       reverse_iterator;
        typedef std::reverse_iterator<
            const_iterator>                 const_reverse_iterator;

        constexpr iterator begin(void) noexcept
            { return iterator{ &first() }; }
        constexpr iterator end(void) noexcept
            { return iterator{ &first() + Rows }; }

        constexpr const_iterator begin(void) const noexcept
            { return const_iterator{ &first() }; }
        constexpr const_iterator end(void) const noexcept
            { return const_iterator{ &first() + Rows }; }

        constexpr const_iterator cbegin(void) const noexcept
            { return const_iterator{ &first() }; }
        constexpr const_iterator cend(void) const noexcept
            { return const_iterator{ &first() + Rows }; }

        constexpr reverse_iterator rbegin(void) noexcept
            { return reverse_iterator{ end() - 1 }; }
        constexpr reverse_iterator rend(void) noexcept
            { return reverse_iterator{ begin() - 1 }; }

        constexpr const_reverse_iterator rbegin(void) const noexcept
            { return const_reverse_iterator{ end() - 1 }; }
        constexpr const_reverse_iterator rend(void) const noexcept
            { return const_reverse_iterator{ begin() - 1 }; }

        constexpr const_reverse_iterator crbegin(void) const noexcept
            { return const_reverse_iterator{ end() - 1 }; }
        constexpr const_reverse_iterator crend(void) const noexcept
            { return const_reverse_iterator{ begin() - 1 }; }

        constexpr value_type& operator[](std::size_t index) noexcept
            { return *(&first() + index); }
        constexpr value_type const& operator[](std::size_t index) const noexcept
            { return *(&first() + index); }

        typedef Iterator<column_value_type> column_iterator;
        typedef Iterator<
            column_value_type const>        const_column_iterator;
        typedef std::reverse_iterator<
            column_iterator>                column_reverse_iterator;
        typedef std::reverse_iterator<
            const_column_iterator>          const_column_reverse_iterator;

        constexpr column_iterator columnBegin(void) noexcept
            { return column_iterator{ &firstCol() }; }
        constexpr column_iterator columnEnd(void) noexcept
            { return column_iterator{ &firstCol() + Cols }; }

        constexpr const_column_iterator columnBegin(void) const noexcept
            { return const_column_iterator{ &firstCol() }; }
        constexpr const_column_iterator columnEnd(void) const noexcept
            { return const_column_iterator{ &firstCol() + Cols }; }

        constexpr const_column_iterator constCBegin(void) const noexcept
            { return const_column_iterator{ &firstCol() }; }
        constexpr const_column_iterator constCEnd(void) const noexcept
            { return const_column_iterator{ &firstCol() + Cols }; }

        constexpr column_reverse_iterator columnRBegin(void) noexcept
            { return column_reverse_iterator{ columnEnd() - 1 }; }
        constexpr column_reverse_iterator columnREnd(void) noexcept
            { return column_reverse_iterator{ columnBegin() - 1 }; }

        constexpr const_column_reverse_iterator columnRBegin(void) const noexcept
            { return const_column_reverse_iterator{ columnEnd() - 1 }; }
        constexpr const_column_reverse_iterator columnREnd(void) const noexcept
            { return const_column_reverse_iterator{ columnBegin() - 1 }; }

        constexpr const_column_reverse_iterator columnCRBegin(void) const noexcept
            { return const_column_reverse_iterator{ columnEnd() - 1 }; }
        constexpr const_column_reverse_iterator columnCREnd(void) const noexcept
            { return const_column_reverse_iterator{ columnBegin() - 1 }; }

        constexpr column_value_type& getColumn(std::size_t index) noexcept
            { return *(&firstCol() + index); }
        constexpr column_value_type const& getColumn(std::size_t index) const noexcept
            { return *(&firstCol() + index); }

        template <std::integral Ip>
        constexpr value_type& operator[] (Vector2<Ip> const& index) noexcept
            { return (*this)[index[0]][index[1]]; }
        template <std::integral Ip>
        constexpr value_type const& operator[] (Vector2<Ip> const& index) const noexcept
            { return (*this)[index[0]][index[1]]; }

        constexpr auto columnsRange(void) noexcept
            { return std::ranges::subrange(columnBegin(), columnEnd()); }

        constexpr auto columnsRange(void) const noexcept
            { return std::ranges::subrange(columnBegin(), columnEnd()); }

        template <Arithmetic U>
        constexpr operator Matrix<U, Rows, Cols>() const noexcept;

        template <std::size_t URows, std::size_t UCols>
            requires (URows >= Rows && UCols >= Cols &&
                (URows != Rows || UCols != Cols))
        constexpr operator Matrix<Tp, URows, UCols>() const noexcept;

        constexpr Matrix& operator+=(Matrix const& right);
        constexpr Matrix& operator-=(Matrix const& right);
        constexpr Matrix& operator*=(Matrix const& right);
        constexpr Matrix& operator/=(Matrix const& right);

        constexpr Matrix& operator%=(
            Matrix const& right) requires mpgl_Operable(Tp, %);
        constexpr Matrix& operator^=(
            Matrix const& right) requires mpgl_Operable(Tp, ^);
        constexpr Matrix& operator&=(
            Matrix const& right) requires mpgl_Operable(Tp, &);
        constexpr Matrix& operator|=(
            Matrix const& right) requires mpgl_Operable(Tp, |);

        constexpr Matrix& operator+=(Tp const& right);
        constexpr Matrix& operator-=(Tp const& right);
        constexpr Matrix& operator*=(Tp const& right);
        constexpr Matrix& operator/=(Tp const& right);

        constexpr Matrix& operator%=(Tp const& right
            ) requires mpgl_Operable(Tp, %);
        constexpr Matrix& operator^=(Tp const& right
            ) requires mpgl_Operable(Tp, ^);
        constexpr Matrix& operator&=(Tp const& right
            ) requires mpgl_Operable(Tp, &);
        constexpr Matrix& operator|=(Tp const& right
            ) requires mpgl_Operable(Tp, |);


    private:
        template <typename... Args>
        constexpr NormalBase tupleBuilder(Args&&... args) const noexcept;

        constexpr value_type& first(void) noexcept;
        constexpr value_type const& first(void) const noexcept;

        constexpr column_value_type& firstCol(void) noexcept;
        constexpr column_value_type const& firstCol(void) const noexcept;

        union {
            NormalBase                      normalBase;
            ColumnBase                      columnBase;
        };
    };

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows == Cols && Rows > 1)
    constexpr Matrix<Tp, Rows, Cols> identityMatrix(
        Tp diagonal = {1}) noexcept
    {
        Matrix<Tp, Rows, Cols> identity;
        for (std::size_t i = 0; i < Rows; ++i)
            identity[i][i] = diagonal;
        return identity;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Cols, Rows>
        transpose(Matrix<Tp, Rows, Cols> const& matrix) noexcept
    {
        Matrix<Tp, Cols, Rows> transposed;
        for (std::size_t i = 0; i < Rows; ++i)
            for (std::size_t j = 0; j < Cols; ++j)
                transposed[j][i] = matrix[i][j];
        return transposed;
    }

    // methods

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>
        Matrix<Tp, Rows, Cols>::operator-(void) const noexcept
    {
        Matrix<Tp, Cols, Rows> result;
        for (std::size_t i = 0; i < Rows; ++i)
            for (std::size_t j = 0; j < Cols; ++j)
                result[i][j] = -(*this)[i][j];
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column&
        Matrix<Tp, Rows, Cols>::Column::operator=(
            Vector<Tp, Rows> const& vec) noexcept
    {
        std::ranges::copy(vec, begin());
        return *this;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    constexpr Matrix<Tp, Rows, Cols>::Column::operator
        Vector<Tp, Rows>() const noexcept
    {
        Vector<Tp, Rows> vector;
        std::ranges::copy(*this, vector.begin());
        return vector;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <AllAbsolutelySame<typename
        Matrix<Tp, Rows, Cols>::value_type>... Args>
            requires (sizeof...(Args) == Rows)
    constexpr Matrix<Tp, Rows, Cols>::Matrix(Args&&... args) noexcept
        : normalBase{tupleBuilder(std::forward<Args>(args)...)}
    {
        if constexpr (!std::is_constant_evaluated())
            reverse(begin(), end());
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    template <typename... Args>
    constexpr Matrix<Tp, Rows, Cols>::NormalBase
        Matrix<Tp, Rows, Cols>::tupleBuilder(Args&&... args) const noexcept
    {
        if constexpr (std::is_constant_evaluated())
            return tupleReverser(std::forward<Args>(args)...);
        else
            return {std::forward<Args>(args)...};
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::value_type&
        Matrix<Tp, Rows, Cols>::first(void)  noexcept
    {
        return static_cast<value_type&>(
            std::get<Rows - 1>(
                static_cast<NormalBase&>
                (this->normalBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::value_type const&
        Matrix<Tp, Rows, Cols>::first(void) const noexcept
    {
        return static_cast<value_type const&>(
            std::get<Rows - 1>(
                static_cast<NormalBase const&>
                (this->normalBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::column_value_type&
        Matrix<Tp, Rows, Cols>::firstCol(void)  noexcept
    {
        return std::get<Cols - 1>(
            std::get<Rows - 1>(
                static_cast<ColumnBase&>
                (this->columnBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows > 1 && Cols > 1)
    constexpr Matrix<Tp, Rows, Cols>::column_value_type const&
        Matrix<Tp, Rows, Cols>::firstCol(void) const noexcept
    {
        return std::get<Cols - 1>(
            std::get<Rows - 1>(
                static_cast<ColumnBase const&>
                (this->columnBase)));
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <Arithmetic Up>
    constexpr Up Matrix<Tp, Rows, Cols>::Column::length(
        Up init) const noexcept
    {
        return accumulate(*this, std::move(init),
            [](auto const& value){ return value * value; });
    }

    #ifndef ge_matcol_matcol_inner_op_factory
    #define ge_matcol_matcol_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>::Column& \
            Matrix<Tp, Rows, Cols>::Column::operator Op##=( \
                Column const& right) \
        { \
            std::ranges::transform(*this, right, begin(), \
                [](Tp const& left, Tp const& right)->Tp{ return left Op right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_matcol_matcol_c_inner_op_factory
    #define ge_matcol_matcol_c_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>::Column& \
            Matrix<Tp, Rows, Cols>::Column::operator Op##=( \
                Column const& right) requires mpgl_Operable(Tp, Op) \
        { \
            std::ranges::transform(*this, right, begin(), \
                [](Tp const& left, Tp const& right)->Tp{ return left Op right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_matcol_tp_inner_op_factory
    #define ge_matcol_tp_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>::Column& \
            Matrix<Tp, Rows, Cols>::Column::operator Op##=( \
                Tp const& right) \
        { \
            std::ranges::for_each(*this, [&right](Tp& value) -> void \
                { value Op##= right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_matcol_tp_c_inner_op_factory
    #define ge_matcol_tp_c_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>::Column& \
            Matrix<Tp, Rows, Cols>::Column::operator Op##=( \
                Tp const& right) requires mpgl_Operable(Tp, Op) \
        { \
            std::ranges::for_each(*this, [&right](Tp& value) -> void \
                { value Op##= right; }); \
            return *this; \
        }
    #endif

    ge_matcol_matcol_inner_op_factory(+)
    ge_matcol_matcol_inner_op_factory(-)
    ge_matcol_matcol_inner_op_factory(*)
    ge_matcol_matcol_inner_op_factory(/)

    ge_matcol_matcol_c_inner_op_factory(%)
    ge_matcol_matcol_c_inner_op_factory(^)
    ge_matcol_matcol_c_inner_op_factory(&)
    ge_matcol_matcol_c_inner_op_factory(|)

    ge_matcol_tp_inner_op_factory(+)
    ge_matcol_tp_inner_op_factory(-)
    ge_matcol_tp_inner_op_factory(*)
    ge_matcol_tp_inner_op_factory(/)

    ge_matcol_tp_c_inner_op_factory(%)
    ge_matcol_tp_c_inner_op_factory(^)
    ge_matcol_tp_c_inner_op_factory(&)
    ge_matcol_tp_c_inner_op_factory(|)

    #ifndef ge_matcol_vec_op_factory
    #define ge_matcol_vec_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Vector<Tp, Rows> \
            operator Op (typename Matrix<Tp, Rows, Cols>::Column const& left, \
                Vector<Tp, Rows> const& right) \
        { \
            Vector<Tp, Rows> result; \
            std::ranges::transform(left, right, result.begin(), \
                [](Tp const& left, Tp const& right)-> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_matcol_vec_c_op_factory
    #define ge_matcol_vec_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Vector<Tp, Rows> \
            operator Op (typename Matrix<Tp, Rows, Cols>::Column const& left, \
                Vector<Tp, Rows> const& right) \
                    requires mpgl_Operable(Tp, Op) \
        { \
            Vector<Tp, Rows> result; \
            std::ranges::transform(left, right, result.begin(), \
                [](Tp const& left, Tp const& right)-> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_vec_matcol_op_factory
    #define ge_vec_matcol_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Vector<Tp, Rows> \
            operator Op (Vector<Tp, Rows> const& left, \
                typename Matrix<Tp, Rows, Cols>::Column const& right) \
        { \
            Vector<Tp, Rows> result; \
            std::ranges::transform(left, right, result.begin(), \
                [](Tp const& left, Tp const& right)-> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_vec_matcol_c_op_factory
    #define ge_vec_matcol_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Vector<Tp, Rows> \
            operator Op (Vector<Tp, Rows> const& left, \
                typename Matrix<Tp, Rows, Cols>::Column const& right) \
                    requires mpgl_Operable(Tp, Op) \
        { \
            Vector<Tp, Rows> result; \
            std::ranges::transform(left, right, result.begin(), \
                [](Tp const& left, Tp const& right)-> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif

    ge_matcol_vec_op_factory(+)
    ge_matcol_vec_op_factory(-)
    ge_matcol_vec_op_factory(*)
    ge_matcol_vec_op_factory(/)

    ge_matcol_vec_c_op_factory(%)
    ge_matcol_vec_c_op_factory(^)
    ge_matcol_vec_c_op_factory(&)
    ge_matcol_vec_c_op_factory(|)

    ge_vec_matcol_op_factory(+)
    ge_vec_matcol_op_factory(-)
    ge_vec_matcol_op_factory(*)
    ge_vec_matcol_op_factory(/)

    ge_vec_matcol_c_op_factory(%)
    ge_vec_matcol_c_op_factory(^)
    ge_vec_matcol_c_op_factory(&)
    ge_vec_matcol_c_op_factory(|)

    // matrix methods

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <Arithmetic U>
    constexpr Matrix<Tp, Rows, Cols>::operator
        Matrix<U, Rows, Cols>() const noexcept
    {
        using u_type = typename Matrix<U, Rows, Cols>::value_type;

        Matrix<U, Rows, Cols> result;
        std::ranges::copy(*this, result.begin());
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    template <std::size_t URows, std::size_t UCols>
        requires (URows >= Rows && UCols >= Cols &&
            (URows != Rows || UCols != Cols))
    constexpr Matrix<Tp, Rows, Cols>::operator
        Matrix<Tp, URows, UCols>() const noexcept
    {
        Matrix<Tp, URows, UCols> result;
        std::ranges::copy(*this, result.begin());
        return result;
    }

    #ifndef ge_mat_mat_inner_op_factory
    #define ge_mat_mat_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>& \
            Matrix<Tp, Rows, Cols>::operator Op##= ( \
                Matrix<Tp, Rows, Cols> const& right) \
        { \
            std::ranges::transform(*this, right, begin(), \
                [](auto const& left, auto const& right) \
                { return left Op right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_mat_mat_inner_c_op_factory
    #define ge_mat_mat_inner_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>& \
            Matrix<Tp, Rows, Cols>::operator Op##= ( \
                Matrix<Tp, Rows, Cols> const& right) \
                    requires mpgl_Operable(Tp, Op) \
        { \
            std::ranges::transform(*this, right, begin(), \
                [](auto const& left, auto const& right) \
                { return left Op right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_mat_tp_inner_op_factory
    #define ge_mat_tp_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>& \
            Matrix<Tp, Rows, Cols>::operator Op##= ( \
                Tp const& right) \
        { \
            std::ranges::for_each(*this, [&right](auto& value) -> void \
                { value Op##= right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_mat_tp_inner_c_op_factory
    #define ge_mat_tp_inner_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols>& \
            Matrix<Tp, Rows, Cols>::operator Op##= ( \
                Tp const& right) requires mpgl_Operable(Tp, Op) \
        { \
            std::ranges::for_each(*this, [&right](auto& value) -> void \
                { value Op##= right; }); \
            return *this; \
        }
    #endif

    ge_mat_mat_inner_op_factory(+)
    ge_mat_mat_inner_op_factory(-)
    ge_mat_mat_inner_op_factory(*)
    ge_mat_mat_inner_op_factory(/)

    ge_mat_mat_inner_c_op_factory(%)
    ge_mat_mat_inner_c_op_factory(^)
    ge_mat_mat_inner_c_op_factory(&)
    ge_mat_mat_inner_c_op_factory(|)

    ge_mat_tp_inner_op_factory(+)
    ge_mat_tp_inner_op_factory(-)
    ge_mat_tp_inner_op_factory(*)
    ge_mat_tp_inner_op_factory(/)

    ge_mat_tp_inner_c_op_factory(%)
    ge_mat_tp_inner_c_op_factory(^)
    ge_mat_tp_inner_c_op_factory(&)
    ge_mat_tp_inner_c_op_factory(|)

    template <Arithmetic Tp, std::size_t LRows,
        std::size_t LCols, std::size_t RCols>
            requires (LRows > 1 && LCols > 1 && RCols > 1)
    Matrix<Tp, LRows, RCols> operator*(
        Matrix<Tp, LRows, LCols> const& left,
        Matrix<Tp, LCols, RCols> const& right)
    {
        Matrix<Tp, LRows, RCols> result;
        for (std::size_t i = 0; i < LRows; ++i)
            for (std::size_t j = 0; j < RCols; ++j)
                result[i][j] = innerProduct(left[i],
                    right.getColumn(j), Tp{0});
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    Matrix<Tp, Rows, Cols> operator+(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result += right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    Matrix<Tp, Rows, Cols> operator-(
        Matrix<Tp, Rows, Cols> const& left,
        Matrix<Tp, Rows, Cols> const& right)
    {
        Matrix<Tp, Rows, Cols> result{left};
        result -= right;
        return result;
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
    Vector<Tp, Rows> operator*(
        Matrix<Tp, Rows, Cols> const& matrix,
        Vector<Tp, Cols> const& vector)
    {
        Vector<Tp, Cols> result;
        for (std::size_t i = 0;i < Cols; ++i)
            result[i] = innerProduct(matrix[i], vector, Tp{0});
        return result;
    }

    #ifndef ge_mat_tp_op_factory
    #define ge_mat_tp_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols> \
            operator Op (Matrix<Tp, Rows, Cols> const& left, \
                Tp const& right) \
        { \
            Matrix<Tp, Rows, Cols> result{left}; \
            result Op##= right; \
            return result; \
        }
    #endif
    #ifndef ge_mat_tp_c_op_factory
    #define ge_mat_tp_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols> \
            operator Op (Matrix<Tp, Rows, Cols> const& left, \
                Tp const& right) requires mpgl_Operable(Tp, Op) \
        { \
            Matrix<Tp, Rows, Cols> result{left}; \
            result Op##= right; \
            return result; \
        }
    #endif
    #ifndef ge_tp_mat_op_factory
    #define ge_tp_mat_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols> \
            operator Op (Tp const& left, \
                Matrix<Tp, Rows, Cols> const& right) \
        { \
            Matrix<Tp, Rows, Cols> result{right}; \
            result Op##= left; \
            return result; \
        }
    #endif
    #ifndef ge_tp_mat_c_op_factory
    #define ge_tp_mat_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Rows, std::size_t Cols> \
        constexpr Matrix<Tp, Rows, Cols> \
            operator Op (Tp const& left, \
                Matrix<Tp, Rows, Cols> const& right) \
                    requires mpgl_Operable(Tp, Op) \
        { \
            Matrix<Tp, Rows, Cols> result{right}; \
            result Op##= left; \
            return result; \
        }
    #endif

    ge_mat_tp_op_factory(+)
    ge_mat_tp_op_factory(-)
    ge_mat_tp_op_factory(*)
    ge_mat_tp_op_factory(/)

    ge_mat_tp_c_op_factory(%)
    ge_mat_tp_c_op_factory(^)
    ge_mat_tp_c_op_factory(&)
    ge_mat_tp_c_op_factory(|)

    ge_tp_mat_op_factory(+)
    ge_tp_mat_op_factory(-)
    ge_tp_mat_op_factory(*)
    ge_tp_mat_op_factory(/)

    ge_tp_mat_c_op_factory(%)
    ge_tp_mat_c_op_factory(^)
    ge_tp_mat_c_op_factory(&)
    ge_tp_mat_c_op_factory(|)

    // common operations

    class LUPDecompositionFn {
    public:
        static constexpr struct CTag {}         CounterTag{};

        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        constexpr std::optional<Vector<Tp, Rows>>
            operator() (
                Matrix<Tp, Rows, Rows>& matrix) const noexcept
        {
            Vector<Tp, Rows> perms;
            std::iota(perms.begin(), perms.end(), Tp{0});
            for (std::size_t k = 0, u; k < Rows; ++k) {
                if (!findP(u, k, matrix))
                    return {};
                if (u != k) {
                    std::swap(perms[k], perms[u]);
                    std::swap(matrix[k], matrix[u]);
                }
                permute(k, matrix);
            }
            return { perms };
        }

        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        constexpr std::optional<bool>
            operator() (
                Matrix<Tp, Rows, Rows>& matrix, CTag) const noexcept
        {
            std::size_t counter = 0;
            for (std::size_t k = 0, u = 0; k < Rows; ++k, u = k) {
                if (!findP(u, k, matrix))
                    return {};
                if (u != k) {
                    ++counter;
                    std::swap(matrix[k], matrix[u]);
                }
                permute(k, matrix);
            }
            return { counter % 2 };
        }
    private:
        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        constexpr bool findP(std::size_t& u, std::size_t& k,
            Matrix<Tp, Rows, Rows> const& matrix) const noexcept
        {
            Tp p{0};
            for (std::size_t i = k; i < Rows; ++i) {
                if (Tp v = std::abs(matrix[i][k]); v > p) {
                    p = v;
                    u = i;
                }
            }
            return p != 0;
        }

        template <Arithmetic Tp, std::size_t Rows>
            requires (Rows > 1)
        constexpr void permute(std::size_t& k,
            Matrix<Tp, Rows, Rows>& matrix) const noexcept
        {
            for (std::size_t i = k + 1; i < Rows; ++i) {
                matrix[i][k] /= matrix[k][k];
                for (std::size_t j = k + 1; j < Rows; ++j)
                    matrix[i][j] -= matrix[i][k] * matrix[k][j];
            }
        }
    };

    inline constexpr LUPDecompositionFn         lupDecomposition;

    template <Arithmetic Tp, std::size_t Rows,
        SizedRange<Tp, Rows> Permutations,
        SizedRange<Tp, Rows> Results>
        requires (Rows > 1)
    constexpr Vector<Tp, Rows>
        lupSolve(Matrix<Tp, Rows, Rows>& luMatrix,
            Permutations const& permutations,
            Results const& results) noexcept
    {
        Vector<Tp, Rows> x, y;
        for (std::size_t i = 0; i < Rows; ++i) {
            Tp sum = std::inner_product(luMatrix[i].begin(),
                luMatrix[i].begin() + i, y.begin(), Tp{0});
            y[i] = results[permutations[i]] - sum;
        }
        for (std::size_t i = Rows - 1; i < Rows; --i) {
            Tp sum = std::inner_product(luMatrix[i].begin() + i + 1,
                luMatrix[i].end(), x.begin() + i + 1, Tp{0});
            x[i] = (y[i] - sum) / luMatrix[i][i];
        }
        return x;
    }

    template <Arithmetic Tp, std::size_t Rows,
            Arithmetic Up = Tp>
        requires (Rows > 1)
    constexpr std::optional<Matrix<Up, Rows, Rows>>
        invert(Matrix<Tp, Rows, Rows> const& matrix) noexcept
    {
        Matrix<Up, Rows, Rows> luMatrix = matrix;
        if (auto permutations = lupDecomposition(luMatrix)) {
            auto inverseMatrix = identityMatrix<Up, Rows, Rows>();
            for (auto& column : inverseMatrix.columnsRange())
                column = lupSolve(luMatrix, *permutations, column);
            return { inverseMatrix };
        }
        return {};
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols,
        Arithmetic Up = Tp>
            requires (Rows == Cols && Cols > 1)
    constexpr Up det(
        Matrix<Tp, Rows, Cols> const& matrix) noexcept
    {
        Matrix<Up, Rows, Rows> luMatrix = matrix;
        if (auto parity = lupDecomposition(luMatrix,
            LUPDecompositionFn::CounterTag))
        {
            Up det{luMatrix[0][0]};
            for (std::size_t i = 1; i < Rows; ++i)
                det *= luMatrix[i][i];
            return *parity ? -det : det;
        }
        return Up{0};
    }

    template <Arithmetic Tp, std::size_t Rows, std::size_t Cols>
        requires (Rows == Cols && Cols > 1)
    constexpr Tp trace(
        Matrix<Tp, Rows, Cols> const& matrix) noexcept
    {
        Tp sum{0};
        for (std::size_t i = 0;i < Rows; ++i)
            sum += matrix[i][i];
        return sum;
    }

    template <Arithmetic Tp, std::size_t Size>
    using SquareMatrix                    = Matrix<Tp, Size, Size>;

    template <typename Tp>
    using Matrix2                         = Matrix<Tp, 2, 2>;
    template <typename Tp>
    using Matrix3                         = Matrix<Tp, 3, 3>;
    template <typename Tp>
    using Matrix4                         = Matrix<Tp, 4, 4>;
    template <typename Tp>
    using Matrix8                         = Matrix<Tp, 8, 8>;

    template <std::size_t Rows, std::size_t Cols>
    using MatrixC                         = Matrix<
        std::complex<float64>, Rows, Cols>;

    template class Matrix<float32, 2, 2>;

    typedef Matrix<float32, 2, 2>           Matrix2f;
    typedef Matrix<int32, 2, 2>             Matrix2i;
    typedef Matrix<float64, 2, 2>           Matrix2d;
    typedef Matrix<uint32, 2, 2>            Matrix2u;
    typedef MatrixC<2, 2>                   Matrix2c;

    typedef Matrix<float32, 3, 3>           Matrix3f;
    typedef Matrix<int32, 3, 3>             Matrix3i;
    typedef Matrix<float64, 3, 3>           Matrix3d;
    typedef Matrix<uint32, 3, 3>            Matrix3u;
    typedef MatrixC<3, 3>                   Matrix3c;

    typedef Matrix<float32, 4, 4>           Matrix4f;
    typedef Matrix<int32, 4, 4>             Matrix4i;
    typedef Matrix<uint32, 4, 4>            Matrix4u;
    typedef Matrix<float64, 4, 4>           Matrix4d;
    typedef MatrixC<4, 4>                   Matrix4c;

}
