#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <math.h>

#include "../Traits/TupleTraits.hpp"
#include "../Traits/Concepts.hpp"
#include "../Utility/Ranges.hpp"

#include <initializer_list>

#ifndef friend_expr
#define friend_expr     friend constexpr
#endif

namespace ge {

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

        template <std::size_t Index>
            requires (Index < Size)
        constexpr reference get(void) & noexcept
            { return std::get<Size - 1 - Index>(static_cast<Base&>(*this)); }

        template <std::size_t Index>
            requires (Index < Size)
        constexpr const_reference get(void) const& noexcept
            { return std::get<Size - 1 - Index>(static_cast<Base const&>(*this)); }

        template <std::size_t Index>
            requires (Index < Size)
        constexpr value_type&& get(void) && noexcept
            { return std::get<Size - 1 - Index>(static_cast<Base&&>(*this)); }

        static constexpr std::size_t size(void) noexcept
            { return Size; }

        template <Arithmetic Up = Tp>
        constexpr Up length(void) const noexcept;

        constexpr Vector& operator+=(Vector const& right);
        constexpr Vector& operator-=(Vector const& right);
        constexpr Vector& operator*=(Vector const& right);
        constexpr Vector& operator/=(Vector const& right);

        constexpr Vector& operator%=(Vector const& right)
            requires Operable(Tp, %);
        constexpr Vector& operator^=(Vector const& right)
            requires Operable(Tp, ^);
        constexpr Vector& operator&=(Vector const& right)
            requires Operable(Tp, &);
        constexpr Vector& operator|=(Vector const& right)
            requires Operable(Tp, |);

        constexpr Vector& operator+=(Tp&& right);
        constexpr Vector& operator-=(Tp&& right);
        constexpr Vector& operator*=(Tp&& right);
        constexpr Vector& operator/=(Tp&& right);

        constexpr Vector& operator%=(Tp&& right)
            requires Operable(Tp, %);
        constexpr Vector& operator^=(Tp&& right)
            requires Operable(Tp, ^);
        constexpr Vector& operator&=(Tp&& right)
            requires Operable(Tp, &);
        constexpr Vector& operator|=(Tp&& right)
            requires Operable(Tp, |);

        template <Arithmetic Up>
        constexpr operator Vector<Up, Size>() const noexcept;

        template <std::size_t USize>
            requires (USize > Size)
        constexpr operator Vector<Tp, USize>() const noexcept;

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
            constexpr iter operator[](difference_type offset) noexcept
                { auto temp = *this; temp.ptr += offset; return temp; }

            friend_expr bool operator==(iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr == right.ptr; }

            friend_expr iter operator+(iter_cref left,
                difference_type right) noexcept
                    { auto temp = left; left.ptr += right; return temp; }
            friend_expr iter operator+(difference_type left,
                iter_cref right) noexcept
                    { auto temp = right; right.ptr += left; return temp; }
            friend_expr iter operator-(iter_cref left,
                difference_type right) noexcept
                    { auto temp = left; temp.ptr -= right; return temp; }
            friend_expr difference_type operator-(iter_cref left,
                iter_cref right) noexcept
                    { return left.ptr - right.ptr; }

            friend_expr bool operator> (iter_cref right,
                iter_cref left) noexcept
                    { return right.ptr > left.ptr; }
            friend_expr bool operator< (iter_cref right,
                iter_cref left) noexcept
                    { return right.ptr < left.ptr; }
            friend_expr bool operator>= (iter_cref right,
                iter_cref left) noexcept
                    { return right.ptr >= left.ptr; }
            friend_expr bool operator<= (iter_cref right,
                iter_cref left) noexcept
                    { return right.ptr <= left.ptr; }
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
            { return iterator{ &get<0>() }; }
        constexpr iterator end(void) noexcept
            { return iterator{ &get<0>() + Size }; }

        constexpr const_iterator begin(void) const noexcept
            { return const_iterator{ &get<0>() }; }
        constexpr const_iterator end(void) const noexcept
            { return const_iterator{ &get<0>() + Size }; }

        constexpr const_iterator cbegin(void) const noexcept
            { return const_iterator{ &get<0>() }; }
        constexpr const_iterator cend(void) const noexcept
            { return const_iterator{ &get<0>() + Size}; }

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
            { return *(&get<0>() + index); }
        constexpr const_reference operator[] (std::size_t index) const noexcept
            { return *(&get<0>() + index); }
    private:
        template <typename... Args>
        constexpr Base tupleBuilder(Args&&... args) const noexcept;
    };

    template <Arithmetic Tp, std::size_t Size>
    constexpr Tp dotProduct(Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept;

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
        std::ranges::copy(*this, base);
        return base;
    }

    template <Arithmetic Tp, std::size_t Size>
    constexpr Tp dotProduct(Vector<Tp, Size> const& left,
        Vector<Tp, Size> const& right) noexcept
    {
        return std::inner_product(left.begin(), left.end(),
            right.begin(), static_cast<Tp>(0));
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
            Vector<Tp, Size> const& right) requires Operable(Tp, Op) \
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
           Tp&& right) \
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
           Tp&& right) requires Operable(Tp, Op) \
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
                Vector<Tp, Size> const& right) requires Operable(Tp, Op) \
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
                Tp const& right) requires Operable(Tp, Op) \
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
                requires Operable(Tp, Op) \
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

    template class Vector<float, 2>;
    template class Vector<uint32_t, 2>;

    typedef Vector<float, 2>    Vector2f;
    typedef Vector<uint32_t, 2> Vector2ui;
    typedef Vector<int32_t, 2>  Vector2i;

    template <Arithmetic Tp>
    using TwoVector = Vector<Tp, 2>;

    template <Arithmetic Tp>
    using ThreeVector = Vector<Tp, 3>;

    template <Arithmetic Tp>
    using FourVector = Vector<Tp, 4>;

    constexpr Vector2f operator"" _x(long double value) noexcept {
        return Vector2f{static_cast<float>(value), 0.f};
    }

    constexpr Vector2ui operator"" _x(unsigned long long int value) noexcept {
        return Vector2ui{static_cast<uint32_t>(value), 0u};
    }

    constexpr Vector2f operator"" _y(long double value) noexcept {
        return Vector2f{0.f, static_cast<float>(value)};
    }

    constexpr Vector2ui operator"" _y(unsigned long long int value) noexcept {
        return Vector2ui{0u, static_cast<uint32_t>(value)};
    }

}

namespace std {

    template <ge::Arithmetic Tp, std::size_t Size>
    struct tuple_size<ge::Vector<Tp, Size>> : integral_constant<size_t, Size> {};

    template <ge::Arithmetic Tp, std::size_t Size, std::size_t Index>
        requires (Index < Size)
    struct tuple_element<Index, ge::Vector<Tp, Size>> { using type = Tp; };

}
