#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <complex>
#include <math.h>

#include "../Traits/TupleTraits.hpp"
#include "../Traits/Concepts.hpp"
#include "../Utility/Ranges.hpp"
#include "../Traits/Types.hpp"

#ifndef friend_expr
#define friend_expr     friend constexpr
#endif

namespace mpgl {

    template <Arithmetic Tp, std::size_t Size>
    class Vector : public UniformTuple<Tp, Size> {
    public:
        typedef Tp                      value_type;
        typedef Tp&                     reference;
        typedef Tp const&               const_reference;
        typedef UniformTuple<Tp, Size>  Base;

        template <AbsolutelyArithmetic... Args>
            requires (sizeof...(Args) == Size
                && AllConvertible<value_type, std::remove_cvref_t<Args>...>)
        constexpr Vector(Args&&... args) noexcept;

        constexpr Vector(Base const& base) noexcept
            : Base{base} {}

        constexpr Vector(Base&& base) noexcept
            : Base{std::move(base)} {}

        constexpr Vector(void) noexcept = default;

        static constexpr std::size_t size(void) noexcept
            { return Size; }

        template <Arithmetic Up = Tp>
        constexpr Up length(void) const noexcept;

        constexpr Vector& operator+=(Vector const& right);
        constexpr Vector& operator-=(Vector const& right);
        constexpr Vector& operator*=(Vector const& right);
        constexpr Vector& operator/=(Vector const& right);

        constexpr Vector& operator%=(Vector const& right)
            requires mpgl_Operable(Tp, %);
        constexpr Vector& operator^=(Vector const& right)
            requires mpgl_Operable(Tp, ^);
        constexpr Vector& operator&=(Vector const& right)
            requires mpgl_Operable(Tp, &);
        constexpr Vector& operator|=(Vector const& right)
            requires mpgl_Operable(Tp, |);

        constexpr Vector& operator+=(Tp const& right);
        constexpr Vector& operator-=(Tp const& right);
        constexpr Vector& operator*=(Tp const& right);
        constexpr Vector& operator/=(Tp const& right);

        constexpr Vector& operator%=(Tp const& right)
            requires mpgl_Operable(Tp, %);
        constexpr Vector& operator^=(Tp const& right)
            requires mpgl_Operable(Tp, ^);
        constexpr Vector& operator&=(Tp const& right)
            requires mpgl_Operable(Tp, &);
        constexpr Vector& operator|=(Tp const& right)
            requires mpgl_Operable(Tp, |);

        template <Arithmetic Up>
        constexpr operator Vector<Up, Size>() const noexcept;

        template <std::size_t USize>
            requires (USize > Size)
        constexpr operator Vector<Tp, USize>() const noexcept;

        constexpr Vector operator- (void) const noexcept;

        template <Arithmetic value_type>
        class Iterator : public std::iterator<std::contiguous_iterator_tag,
            value_type>
        {
        public:
            using iterator_category =       std::contiguous_iterator_tag;
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
                { ++ptr; return *this; }
            constexpr iter operator++(int) noexcept
                { auto temp = *this; ++ptr; return temp; }
            constexpr iter_ref operator--(void) noexcept
                { --ptr; return *this; }
            constexpr iter operator--(int) noexcept
                { auto temp = *this; --ptr; return temp; }

            constexpr reference operator*(void) const noexcept
                { return *ptr; }
            constexpr const_pointer operator->(void) const noexcept
                { return ptr; }

            constexpr iter_ref operator+=(difference_type offset) noexcept
                { ptr += offset; return *this; }
            constexpr iter_ref operator-=(difference_type offset) noexcept
                { ptr -= offset; return *this; }
            constexpr reference operator[](difference_type offset) const noexcept
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

            friend_expr compare operator<=> (iter_cref left,
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
            { return iterator{ &first() }; }
        constexpr iterator end(void) noexcept
            { return iterator{ &first() + Size }; }

        constexpr const_iterator begin(void) const noexcept
            { return const_iterator{ &first() }; }
        constexpr const_iterator end(void) const noexcept
            { return const_iterator{ &first() + Size }; }

        constexpr const_iterator cbegin(void) const noexcept
            { return const_iterator{ &first() }; }
        constexpr const_iterator cend(void) const noexcept
            { return const_iterator{ &first() + Size}; }

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

        constexpr reference operator[] (std::size_t index) noexcept
            { return *(&first() + index); }
        constexpr const_reference operator[] (std::size_t index) const noexcept
            { return *(&first() + index); }
    private:
        template <typename... Args>
        constexpr Base tupleBuilder(Args&&... args) const noexcept;

        constexpr reference first(void) noexcept
            { return std::get<Size - 1>(static_cast<Base&>(*this)); }
        constexpr const_reference first(void) const noexcept
            { return std::get<Size - 1>(static_cast<Base const&>(*this)); }
    };

    template <Arithmetic Tp, std::size_t Size>
    constexpr Tp dot(Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept;

    template <Arithmetic Tp>
    constexpr Tp cross(Vector<Tp, 2> const& left,
        Vector<Tp, 2> const& right) noexcept;

    template <Arithmetic Up, Arithmetic Tp, std::size_t Size>
    constexpr Vector<Up, Size>
        vectorCast(Vector<Tp, Size> const& vector) noexcept;

    template <std::floating_point Tp, std::size_t Size>
    constexpr Vector<Tp, Size> floor(Vector<Tp, Size> vector);

    template <std::floating_point Tp, std::size_t Size>
    constexpr Vector<Tp, Size> ceil(Vector<Tp, Size> vector);

    template <std::floating_point Tp, std::size_t Size>
    constexpr Vector<Tp, Size> round(Vector<Tp, Size> vector);

    template <Arithmetic Tp, std::size_t Size>
    template <AbsolutelyArithmetic... Args>
        requires (sizeof...(Args) == Size && AllConvertible<
            typename Vector<Tp, Size>::value_type,
            std::remove_cvref_t<Args>...>)
    constexpr Vector<Tp, Size>::Vector(Args&&... args) noexcept
        : UniformTuple<Tp, Size>{tupleBuilder(std::forward<Args>(args)...)}
    {
        if constexpr (!std::is_constant_evaluated())
            reverse(*this);
    }

    template <Arithmetic Tp, std::size_t Size>
    template <typename... Args>
    constexpr UniformTuple<Tp, Size>
        Vector<Tp, Size>::tupleBuilder(Args&&... args) const noexcept
    {
        if constexpr (std::is_constant_evaluated())
            return tupleReverser(std::forward<Args>(args)...);
        else
            return {std::forward<Args>(args)...};
    }

    template <Arithmetic Tp, std::size_t Size>
    template <Arithmetic Up>
    constexpr Vector<Tp, Size>::operator Vector<Up, Size>() const noexcept {
        return std::apply([](auto&&... args) -> Vector<Up, Size> {
            return Vector<Up, Size>{ std::move(std::tuple{
                static_cast<Up>(args)... }) };
        }, static_cast<Base const&>(*this));
    }

    template <Arithmetic Tp, std::size_t Size>
    template <std::size_t USize>
        requires (USize > Size)
    constexpr Vector<Tp, Size>::operator Vector<Tp, USize>() const noexcept {
        Vector<Tp, USize> base;
        for (std::size_t i = 0 ;i < Size; ++i)
            base[i] = (*this)[i];
        return base;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Vector<Tp, Size> Vector<Tp, Size>::operator- (
        void) const noexcept
    {
        return std::apply([]<typename... Args>(Args&&... args) {
                return Vector{std::forward<Args>(-args)...};
            }, std::apply([]<typename... Args>(Args&&... args) {
                return tupleReverser(std::forward<Args>(args)...);
            }, static_cast<Base const&>(*this)));
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Tp dot(Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        return std::inner_product(left.begin(), left.end(),
            right.begin(), static_cast<Tp>(0));
    }

    template <Arithmetic Tp>
    constexpr Tp cross(Vector<Tp, 2> const& left,
        Vector<Tp, 2> const& right) noexcept
    {
        return left[0] * right[1] - left[1] * right[0];
    }

    template <Arithmetic Up, Arithmetic Tp, std::size_t Size>
    constexpr Vector<Up, Size>
        vectorCast(Vector<Tp, Size> const& vector) noexcept
    {
        return static_cast<Vector<Up, Size>>(vector);
    }

    template <Arithmetic Tp, std::size_t Size>
    template <Arithmetic Up>
    constexpr Up Vector<Tp, Size>::length(void) const noexcept {
        Tp sum = accumulate(*this, Tp{0}, [](Tp const& value)
            { return value * value; });
        return static_cast<Up>(std::sqrt(sum));
    }

    template <std::floating_point Tp, std::size_t Size>
    constexpr Vector<Tp, Size> floor(Vector<Tp, Size> vector) {
        std::ranges::for_each(vector, [](Tp& value)
            { value = std::floor(value); });
        return vector;
    }

    template <std::floating_point Tp, std::size_t Size>
    constexpr Vector<Tp, Size> ceil(Vector<Tp, Size> vector) {
        std::ranges::for_each(vector, [](Tp& value)
            { value = std::ceil(value); });
        return vector;
    }

    template <std::floating_point Tp, std::size_t Size>
    constexpr Vector<Tp, Size> round(Vector<Tp, Size> vector) {
        std::ranges::for_each(vector, [](Tp& value)
            { value = std::round(value); });
        return vector;
    }

    #ifndef ge_vec_vec_inner_op_factory
    #define ge_vec_vec_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator Op##=( \
            Vector<Tp, Size> const& right) \
        { \
            std::ranges::transform(*this, right, begin(), \
                [](Tp const& left, Tp const& right)->Tp{ return left Op right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_vec_vec_c_inner_op_factory
    #define ge_vec_vec_c_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator Op##=( \
            Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, Op) \
        { \
            std::ranges::transform(*this, right, begin(), \
                [](Tp const& left, Tp const& right)->Tp{ return left Op right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_vec_tp_inner_op_factory
    #define ge_vec_tp_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator Op##=( \
           Tp const& right) \
        { \
            std::ranges::for_each(*this, [&right](Tp& value) -> void \
                { value Op##= right; }); \
            return *this; \
        }
    #endif
    #ifndef ge_vec_tp_c_inner_op_factory
    #define ge_vec_tp_c_inner_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size>& Vector<Tp, Size>::operator Op##=( \
           Tp const& right) requires mpgl_Operable(Tp, Op) \
        { \
            std::ranges::for_each(*this, [&right](Tp& value) -> void \
                { value Op##= right; }); \
            return *this; \
        }
    #endif

    ge_vec_vec_inner_op_factory(+)
    ge_vec_vec_inner_op_factory(-)
    ge_vec_vec_inner_op_factory(*)
    ge_vec_vec_inner_op_factory(/)

    ge_vec_vec_c_inner_op_factory(%)
    ge_vec_vec_c_inner_op_factory(^)
    ge_vec_vec_c_inner_op_factory(|)
    ge_vec_vec_c_inner_op_factory(&)

    ge_vec_tp_inner_op_factory(+)
    ge_vec_tp_inner_op_factory(-)
    ge_vec_tp_inner_op_factory(*)
    ge_vec_tp_inner_op_factory(/)

    ge_vec_tp_c_inner_op_factory(%)
    ge_vec_tp_c_inner_op_factory(^)
    ge_vec_tp_c_inner_op_factory(|)
    ge_vec_tp_c_inner_op_factory(&)

    // operators

    #ifndef ge_vec_vec_op_factory
    #define ge_vec_vec_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size> \
            operator Op (Vector<Tp, Size> const& left, \
                Vector<Tp, Size> const& right) \
        { \
            Vector<Tp, Size> result; \
            std::ranges::transform(left, right, result.begin(), \
                [](Tp const& left, Tp const& right)-> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_vec_c_vec_op_factory
    #define ge_vec_c_vec_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size> \
            operator Op (Vector<Tp, Size> const& left, \
                Vector<Tp, Size> const& right) requires mpgl_Operable(Tp, Op) \
        { \
            Vector<Tp, Size> result; \
            std::ranges::transform(left, right, result.begin(), \
                [](Tp const& left, Tp const& right)-> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_vec_tp_op_factory
    #define ge_vec_tp_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size> \
            operator Op (Vector<Tp, Size> const& left, \
                Tp const& right) \
        { \
            Vector<Tp, Size> result; \
            std::ranges::transform(left, result.begin(), \
                [&right](Tp const& left) -> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_vec_tp_c_op_factory
    #define ge_vec_tp_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size> \
            operator Op (Vector<Tp, Size> const& left, \
                Tp const& right) requires mpgl_Operable(Tp, Op) \
        { \
            Vector<Tp, Size> result; \
            std::ranges::transform(left, result.begin(), \
                [&right](Tp const& left) -> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_tp_vec_op_factory
    #define ge_tp_vec_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size> operator Op ( \
            Tp const& left, Vector<Tp, Size> const& right) \
        { \
            Vector<Tp, Size> result; \
            std::ranges::transform(right, result.begin(), \
                [&left](Tp const& right) -> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif
    #ifndef ge_tp_vec_c_op_factory
    #define ge_tp_vec_c_op_factory(Op) \
        template <Arithmetic Tp, std::size_t Size> \
        constexpr Vector<Tp, Size> operator Op ( \
            Tp const& left, Vector<Tp, Size> const& right) \
                requires mpgl_Operable(Tp, Op) \
        { \
            Vector<Tp, Size> result; \
            std::ranges::transform(right, result.begin(), \
                [&left](Tp const& right) -> Tp { \
                    return left Op right; }); \
            return result; \
        }
    #endif

    ge_vec_vec_op_factory(+)
    ge_vec_vec_op_factory(-)
    ge_vec_vec_op_factory(*)
    ge_vec_vec_op_factory(/)

    ge_vec_c_vec_op_factory(%)
    ge_vec_c_vec_op_factory(^)
    ge_vec_c_vec_op_factory(&)
    ge_vec_c_vec_op_factory(|)

    ge_vec_tp_op_factory(+)
    ge_vec_tp_op_factory(-)
    ge_vec_tp_op_factory(*)
    ge_vec_tp_op_factory(/)

    ge_vec_tp_c_op_factory(%)
    ge_vec_tp_c_op_factory(^)
    ge_vec_tp_c_op_factory(&)
    ge_vec_tp_c_op_factory(|)

    ge_tp_vec_op_factory(+)
    ge_tp_vec_op_factory(-)
    ge_tp_vec_op_factory(*)

    ge_tp_vec_c_op_factory(^)
    ge_tp_vec_c_op_factory(&)
    ge_tp_vec_c_op_factory(|)

    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::weak_ordering>
    constexpr std::weak_ordering lexicographicalCompare(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        for (std::size_t i = 0; i != Size - 1; ++i)
            if (left[i] != right[i])
                return left[i] <=> right[i];
        return left[Size - 1] <=> right[Size - 1];
    }

    template <Arithmetic Tp, std::size_t Size>
        requires std::three_way_comparable<Tp, std::partial_ordering>
    constexpr std::partial_ordering totalCompare(
        Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        auto first = left[0] <=> right[0];
        for (std::size_t i = 1; i != Size; ++i)
            if ((left[i] <=> right[i]) != first)
                return std::partial_ordering::unordered;
        return first;
    }

    template class Vector<float32, 2>;
    template class Vector<uint32, 2>;

    template <std::size_t Size>
    using VectorC                     = Vector<
        std::complex<float64>, Size>;

    typedef Vector<float32, 2>          Vector2f;
    typedef Vector<uint32, 2>           Vector2u;
    typedef Vector<int32, 2>            Vector2i;
    typedef Vector<float64, 2>          Vector2d;
    typedef VectorC<2>                  Vector2c;

    typedef Vector<float32, 3>          Vector3f;
    typedef Vector<uint32, 3>           Vector3u;
    typedef Vector<int32, 3>            Vector3i;
    typedef Vector<float64, 3>          Vector3d;
    typedef VectorC<3>                  Vector3c;

    typedef Vector<float32, 4>          Vector4f;
    typedef Vector<uint32, 4>           Vector4u;
    typedef Vector<int32, 4>            Vector4i;
    typedef Vector<float64, 4>          Vector4d;
    typedef VectorC<4>                  Vector4c;

    template <Arithmetic Tp>
    using Vector2 =                     Vector<Tp, 2>;

    template <Arithmetic Tp>
    using Vector3 =                     Vector<Tp, 3>;

    template <Arithmetic Tp>
    using Vector4 =                     Vector<Tp, 4>;

    constexpr Vector2f operator"" _x(float128 value) noexcept {
        return {value, 0.f};
    }

    constexpr Vector2u operator"" _x(uint64 value) noexcept {
        return {value, 0u};
    }

    constexpr Vector2f operator"" _y(float128 value) noexcept {
        return {0.f, value};
    }

    constexpr Vector2u operator"" _y(uint64 value) noexcept {
        return {0u, value};
    }

    constexpr Vector3f operator"" _z(float128 value) noexcept {
        return {0.f, 0.f, value};
    }

    constexpr Vector3u operator"" _z(uint64 value) noexcept {
        return {0u, 0u, value};
    }

    constexpr Vector4f operator"" _w(float128 value) noexcept {
        return {0.f, 0.f, 0.f, value};
    }

    constexpr Vector4u operator"" _w(uint64 value) noexcept {
        return {0u, 0u, 0u, value};
    }

}

namespace std {

    template <mpgl::Arithmetic Tp, std::size_t Size>
    struct tuple_size<mpgl::Vector<Tp, Size>>
        : integral_constant<size_t, Size> {};

    template <mpgl::Arithmetic Tp, std::size_t Size,
        std::size_t Index>
            requires (Index < Size)
    struct tuple_element<Index, mpgl::Vector<Tp, Size>> {
        using type = Tp;
    };

}
