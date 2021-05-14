#include "Engine/2DGraphics.hpp"

#include <iostream>
#include <numeric>
#include <tuple>

int main(void) noexcept {
    ge::RenderWindow window{{640, 480}, "Hello World!"};

    ge::VertexComponent component(window.getWindowBaseDimmensions(), ge::FigureTypes::Triangle);
    ge::VertexComponent component2(window.getWindowBaseDimmensions(), ge::FigureTypes::Triangle);
    component.setShaders(window.getShaderLib());
    component2.setShaders(window.getShaderLib());

    auto iter = component.begin();
    *iter++ = {-0.5f, -0.5f};
    *iter++ = {0.f, 0.5f};
    *iter++ = {0.5f, -0.5f};

    auto iter2 = component2.begin();
    *iter2++ = {0.5f, -0.5f};
    *iter2++ = {0.f, 0.5f};
    *iter2++ = {0.5f, 0.5f};

    component.copyToGPU();
    component2.copyToGPU();

    while (!window){
        window.clear();
        component.draw();
        component2.draw();
        window.draw();
    }

    return 0;
}