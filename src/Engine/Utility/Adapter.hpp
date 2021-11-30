#pragma once

#include <memory>

#include "../Core/Context/Context.hpp"
#include "../Traits/Concepts.hpp"

namespace ge {

    template <Adaptable Tp>
    class Adapter : private GraphicalObject {
    public:
        typedef typename Tp::value_type value_type;

        Adapter(Tp const& range) noexcept;
        Adapter(Tp&& range) noexcept;

        void operator= (Tp const& factor) noexcept;
        void operator= (Tp&& factor) noexcept;

        operator Tp() const noexcept;

        Tp& get(void) noexcept
            { return range; }
        Tp const& get(void) const noexcept
            { return range; }

        ~Adapter(void) noexcept = default;
    private:
        Tp                              range;
    };

    template <Adaptable Tp>
    Adapter<Tp>::Adapter(Tp const& range) noexcept
        : range{range / static_cast<Tp>(context.windowDimmensions)
            * value_type{2} - value_type{1}} {}

    template <Adaptable Tp>
    Adapter<Tp>::Adapter(Tp&& range) noexcept
        : range{std::move(range) / static_cast<Tp>(context.windowDimmensions)
            * value_type{2} - value_type{1}} {}

    template <Adaptable Tp>
    void Adapter<Tp>::operator= (Tp const& factor) noexcept {
        range = factor
            / static_cast<Tp>(context.windowDimmensions)
                * value_type{2} - value_type{1};
    }

    template <Adaptable Tp>
    void Adapter<Tp>::operator= (Tp&& factor) noexcept {
        range = std::move(factor)
            / static_cast<Tp>(context.windowDimmensions)
                * value_type{2} - value_type{1};
    }

    template <Adaptable Tp>
    Adapter<Tp>::operator Tp() const noexcept {
        return (range + value_type{1})
            * static_cast<Tp>(context.windowDimmensions) / value_type{2};
    }

}
