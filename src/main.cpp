#include "Engine/2DGraphics.hpp"
#include "Engine/Mathematics/Vector.hpp"

int main(void) noexcept {
    ge::RenderWindow window{{640, 480}, "Hello World!"};

    ge::FilledComponent component(window.getWindowBaseDimmensions(), ge::FigureTypes::Triangle);
    component.setShaders(window.getShaderLib());

    component[0].position = {200, 300};
    component[0].color = ge::Color::literals::Red;
    component[1].position = {300, 200};
    component[1].color = ge::Color::literals::Green;
    component[2].position = {400, 300};
    component[2].color = ge::Color::literals::Blue;

    ge::PolygonComponent component2(window.getWindowBaseDimmensions(), ge::FigureTypes::Triangle);
    component2.setShaders(window.getShaderLib());

    component2[0].position = {200, 200};
    component2[1].position = {300, 300};
    component2[2].position = {200, 300};
    for (auto& color : component2 | ge::views::color)
        color = ge::Color::literals::Blue;

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