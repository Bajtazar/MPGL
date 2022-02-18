#include "Shadeable.hpp"

namespace mpgl {

    Shadeable::Shadeable(void)
        : shaderProgram{new ShaderProgram{}} {}

    Shadeable::Shadeable(ProgramPtr const& program)
        : shaderProgram{program} {}

    Shadeable::Shadeable(ProgramPtr&& program) noexcept
        : shaderProgram{std::move(program)} {}

    Shadeable::Shadeable(std::string const& name) : Shadeable{} {
        setShader(name);
    }

    void Shadeable::setShader(std::string const& name,
        Executable exec)
    {
        context.shaders.setOrQueue(shaderProgram, name, std::move(exec));
    }

    Shadeable::Shadeable(std::string const& name,
        ShadersContext::Executable exec) : Shadeable{}
    {
        context.shaders.setOrQueue(shaderProgram,
            name, std::move(exec));
    }

}
