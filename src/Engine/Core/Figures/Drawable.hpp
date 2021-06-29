#pragma once

#include "ShaderLibrary.hpp"
#include "../../Mathematics/Vector.hpp"

namespace ge {

    class Drawable {
    public:
        explicit Drawable(const Vector2i&  scene) noexcept : scene(scene) {}
        /*friend RenderWindow& operator<< (RenderWindow& target, Drawable& drawable) noexcept{
            //drawable.draw(target);
            return target;
        }*/

        virtual void setShaders(const ShaderLibrary&) noexcept = 0;
        virtual void copyToGPU(void) noexcept = 0;
        virtual void draw(void) const noexcept = 0;

        virtual ~Drawable(void) noexcept = default;
    protected:
        const Vector2i& scene;
        uint32_t shaderProgram;
    };

}