#pragma once

#include <ranges>
#include <memory>

namespace ge {

    template <typename T>
    concept Adaptable = requires(T& a, typename T::value_type b) { a / a; a * a; a * b; a / b; a - b; a + b; } && std::is_convertible_v<uint32_t, typename T::value_type>;

    template <Adaptable T, Adaptable U>
    class Adapter {
    public:
        constexpr explicit Adapter(const T& range, const std::shared_ptr<U>& scale) noexcept : range(range) { scaleFactor = scale; }

        using value_type = typename T::value_type;

        constexpr T& operator= (const T& factor) noexcept {
            return range = factor / static_cast<T>(*scaleFactor) * value_type{2} - value_type{1};
        }

        constexpr T& operator= (T&& factor) noexcept {
            return range = std::move(factor) / static_cast<T>(*scaleFactor) * value_type{2} - value_type{1};
        }

        constexpr operator T() const noexcept {
            return (range + value_type{1}) * static_cast<T>(*scaleFactor) / value_type{2};
        }

        T& get(void) noexcept { return range; }
        const T& get(void) const noexcept { return range; }

        constexpr ~Adapter(void) noexcept = default;
    private:
        T range;
        static std::shared_ptr<U> scaleFactor;
    };

    template <Adaptable T, Adaptable U>
    std::shared_ptr<U> Adapter<T, U>::scaleFactor {};

}
