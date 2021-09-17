#pragma once

#include <memory>

#include "Figures/ShaderLibrary.hpp"
#include "../Mathematics/Vector.hpp"
#include "Context.hpp"

namespace ge {

    class Drawable : protected GraphicalObject {
    public:
        explicit Drawable(void) noexcept = default;

        virtual void setShaders(const ShaderLibrary&) noexcept = 0;
        virtual void copyToGPU(void) const noexcept = 0;
        virtual void draw(void) const noexcept = 0;

        virtual ~Drawable(void) noexcept = default;
    };

}