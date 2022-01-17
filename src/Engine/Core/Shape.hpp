#pragma once

#include "Drawable.hpp"

namespace ge {

    class Shape : virtual public Drawable {
    public:
        Shape(Shape const& shape) = delete;

        Shape& operator=(Shape const& shape) = delete;

        virtual void copyToGPU(void) const noexcept = 0;
        virtual void draw(void) const noexcept = 0;

        virtual ~Shape(void) noexcept;
    protected:
        explicit Shape(void) noexcept;

        explicit Shape(Shape&& shape) noexcept;

        Shape& operator=(Shape&& shape) noexcept;

        uint32                                  vertexBuffer;
        uint32                                  vertexArrayObject;
    };

}
