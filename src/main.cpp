#include "Engine/2DGraphics.hpp"

int main(void) noexcept {
    ge::RenderWindow window{{640, 480}, "Hello World!"};

    while (!window){
        window.clear();

        window.draw();
    }

    return 0;
}