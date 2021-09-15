#pragma once

#include "../../Mathematics/Matrix.hpp"

namespace ge {

    struct Transformable {

        virtual void onScreenTransformation(const Vector2ui& oldDimmensions) noexcept = 0;

        virtual void translate(const Vector2f& shift) noexcept = 0;
        virtual void scale(const Vector2f& center, float factor) noexcept = 0;
        virtual void rotate(const Vector2f& center, float angle) noexcept = 0;
        virtual void rotate(const Vector2f& center, const Matrix2f& rotation) noexcept = 0;

        virtual ~Transformable(void) noexcept = default;
    };

}
