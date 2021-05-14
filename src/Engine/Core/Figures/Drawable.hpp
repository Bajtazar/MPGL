#pragma once

#include "ShaderLibrary.hpp"
#include "../../Mathematics/Vector2.hpp"

namespace ge {

    class Drawable {
    public:
        explicit Drawable(const Vector2i&  scene) noexcept : scene(scene) {}
        /*friend RenderWindow& operator<< (RenderWindow& target, Drawable& drawable) noexcept{
            //drawable.draw(target);
            return target;
        }*/
        virtual ~Drawable(void) noexcept = default;
    protected:
        const Vector2i& scene;
        virtual void setShaders(const ShaderLibrary&) const noexcept = 0;
        virtual void copyToGPU(void) const noexcept = 0;
        virtual void draw(void) const noexcept = 0;
    };

}