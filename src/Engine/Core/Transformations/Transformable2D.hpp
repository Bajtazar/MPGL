#pragma once

#include "../../Mathematics/Matrix.hpp"
#include "Transformable.hpp"

namespace mpgl {

    template<>
    struct Transformable<dim::Dim2> : public virtual ScreenTransformationEvent {
        explicit Transformable(void) noexcept = default;

        virtual void translate(Vector2f const& shift) noexcept = 0;
        virtual void scale(Vector2f const& center, float32 factor) noexcept = 0;
        virtual void rotate(Vector2f const& center, float32 angle) noexcept = 0;
        virtual void rotate(Vector2f const& center, Matrix2f const& rotation) noexcept = 0;

        virtual ~Transformable(void) noexcept = default;
    };

    typedef Transformable<dim::Dim2>      Transformable2D;

}
