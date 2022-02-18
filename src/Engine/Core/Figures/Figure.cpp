#include "Figure.hpp"

namespace mpgl {

    Figure::Figure(std::string const& programName)
        : Shadeable{programName} {}

    Figure::Figure(std::string const& programName,
        Executable exec) : Shadeable{programName, exec} {}

    Figure::Figure(Figure const& shape)
        : Shadeable{shape.shaderProgram} {}

    Figure::Figure(Figure&& shape) noexcept
        : Shadeable{std::move(shape.shaderProgram)},
        Shape{std::move(shape)} {}

    Figure& Figure::operator=(Figure&& shape) noexcept {
        shaderProgram = std::move(shape.shaderProgram);
        Shape::operator=(std::move(shape));
        return *this;
    }

    Figure& Figure::operator=(Figure const& shape) {
        shaderProgram = shape.shaderProgram;;
        return *this;
    }

}
