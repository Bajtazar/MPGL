#pragma once

#include "Context/Context.hpp"

namespace mpgl {

    class Drawable : virtual protected GraphicalObject {
    public:
        explicit Drawable(void) noexcept = default;

        virtual void copyToGPU(void) const noexcept = 0;
        virtual void draw(void) const noexcept = 0;

        virtual ~Drawable(void) noexcept = default;
    };

}