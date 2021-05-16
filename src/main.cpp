#include "Engine/2DGraphics.hpp"

#include <iostream>

int main(void) noexcept {
    ge::RenderWindow window{{640, 480}, "Hello World!"};

    ge::PolygonComponent component(window.getWindowBaseDimmensions(), ge::FigureTypes::Triangle);
    component.setShaders(window.getShaderLib());

    component[0].position = {-0.5f, -0.5f};
    component[0].color = ge::Color::literals::Red;
    component[1].position = {0.f, 0.5f};
    component[1].color = ge::Color::literals::Green;
    component[2].position = {0.5f, -0.5f};
    component[2].color = ge::Color::literals::Blue;

    component.copyToGPU();

    while (!window){
        window.clear();
        component.draw();
        window.draw();
    }

    return 0;
}