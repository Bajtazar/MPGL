#include "Engine/2DGraphics.hpp"

using namespace ge;

int main(void) noexcept {
    ge::RenderWindow window{640_x + 480_y, "Hello World!"};
    Circle cc(window.getWindowBaseDimmensions(), 400_x + 200_y, 100.f, ge::Color::literals::Blue);

    ge::Rectangle component(window.getWindowBaseDimmensions(), 200_x + 200_y, 100_x + 100_y, ge::Color::literals::Green);

    component[0].color = ge::Color::literals::Red;
    component[1].color = ge::Color::literals::Green;
    component[2].color = ge::Color::literals::Blue;
    component[3].color = ge::Color::literals::Yellow;

    ge::PolygonTriangle component2(window.getWindowBaseDimmensions(),
        200_x + 200_y, 300_x + 200_y, 200_x + 300_y, ge::Color::literals::Blue);

    component.setShaders(window.getShaderLib());
    component2.setShaders(window.getShaderLib());
    cc.setShaders(window.getShaderLib());

    component.copyToGPU();
    component2.copyToGPU();
    cc.copyToGPU();

    while (!window){
        window.clear();

        component.draw();
        component2.draw();
        cc.draw();

        window.draw();
    }

    return 0;
}