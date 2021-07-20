#pragma once

#include <concepts>

namespace ge {

    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <typename T, typename... Args>
    concept AllSame = std::conjunction_v<std::is_same<T, Args>...>;

    template <typename T>
    concept Constant = std::is_const_v<std::remove_reference_t<T>>;

    template <typename T>
    concept NotConstant = !Constant<T>;

}
