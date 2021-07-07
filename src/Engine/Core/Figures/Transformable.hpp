#pragma once

#include "../../Mathematics/Vector.hpp"

namespace ge {

    struct Transformable {

        //virtual void onScreenTransformation(void) noexcept = 0;
        virtual void translate(const Vector2f& shift) noexcept = 0;
        virtual void scale(const Vector2f& center, float factor) noexcept = 0;
        virtual void rotate(const Vector2f& center, float angle) noexcept = 0;

        virtual ~Transformable(void) noexcept = default;
    };

}
