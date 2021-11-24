#include "Shadeable.hpp"

namespace ge {

    Shadeable::Shadeable(void) noexcept
        : shaderProgram{new ShaderProgram{}} {}

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
        setShader(name, std::move(exec));
    }

}
