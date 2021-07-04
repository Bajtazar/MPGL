#pragma once

#include <ranges>
#include "../Core/Color.hpp"

namespace ge {

    template <typename T>
    concept Adaptable = requires(T& a, typename T::value_type b) { a / a; a * a; a * b; a / b; a - b; a + b; } && std::is_convertible_v<uint32_t, typename T::value_type>;

    template <Adaptable T>
    class Adapter {
    public:
        constexpr explicit Adapter(const T& range, T scale) noexcept : range(range) { scaleFactor = scale; }

        using value_type = typename T::value_type;

        constexpr T& operator= (const T& factor) noexcept {
            return range = factor / scaleFactor * value_type{2} - value_type{1};
        }

        constexpr T& operator= (T&& factor) noexcept {
            return range = std::move(factor) / scaleFactor * value_type{2} - value_type{1};
        }

        constexpr operator T() const noexcept {
            return (range + value_type{1}) * scaleFactor / value_type{2};
        }

        T& get(void) noexcept { return range; }
        const T& get(void) const noexcept { return range; }

    private:
        T range;
        static T scaleFactor;
    };

    template <Adaptable T>
    T Adapter<T>::scaleFactor {};

}
