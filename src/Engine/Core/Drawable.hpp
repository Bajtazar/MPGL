#pragma once

#include "RenderWindow.hpp"

namespace ge {

    class Drawable {
    public:
        friend RenderWindow& operator<< (RenderWindow& target, Drawable& drawable) noexcept{
            //drawable.draw(target);
            return target;
        }
    protected:
        virtual void draw(RenderWindow& target) const noexcept = 0;
    };

}