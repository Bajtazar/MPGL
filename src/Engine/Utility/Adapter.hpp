#pragma once

#include <memory>

#include "../Core/Context/Context.hpp"
#include "../Traits/Concepts.hpp"

namespace ge {

    template <Adaptable T>
    class Adapter : private GraphicalObject {
    public:
        Adapter(T const& range) noexcept;

        using value_type = typename T::value_type;

        T& operator= (T const& factor) noexcept;
        T& operator= (T&& factor) noexcept;

        operator T() const& noexcept;

        T& get(void) noexcept { return range; }
        T const& get(void) const noexcept { return range; }

        ~Adapter(void) noexcept = default;
    private:
        T                           range;
    };

    template <Adaptable T>
    Adapter<T>::Adapter(T const& range) noexcept
        : range{range / static_cast<T>(context.windowDimmensions)
            * value_type{2} - value_type{1}} {}

    template <Adaptable T>
    T& Adapter<T>::operator= (T const& factor) noexcept {
        return range = factor / static_cast<T>(context.windowDimmensions)
            * value_type{2} - value_type{1};
    }

    template <Adaptable T>
    T& Adapter<T>::operator= (T&& factor) noexcept {
        return range = std::move(factor)
            / static_cast<T>(context.windowDimmensions)
                * value_type{2} - value_type{1};
    }

    template <Adaptable T>
    Adapter<T>::operator T() const& noexcept {
        return (range + value_type{1})
            * static_cast<T>(context.windowDimmensions) / value_type{2};
    }

}
