#pragma once

#include "../RenderWindow.hpp"

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
        virtual void setShaders() const noexcept = 0;
        virtual void copyToGPU(void) const noexcept = 0;
        virtual void draw(RenderWindow& target) const noexcept = 0;
    };

}