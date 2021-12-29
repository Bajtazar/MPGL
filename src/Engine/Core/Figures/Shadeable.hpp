#pragma once

#include "../Context/Context.hpp"
#include "ShaderProgram.hpp"

namespace ge {

    class Shadeable : virtual protected GraphicalObject {
    public:
        typedef std::shared_ptr<ShaderProgram>      ProgramPtr;
        typedef typename ShadersContext::Executable Executable;

        void setShader(ShaderProgram const& program) noexcept
            { *shaderProgram = program; }
        void setShader(ShaderProgram&& program) noexcept
            { *shaderProgram = std::move(program); }
        void setShader(std::string const& name)
            { context.shaders.setOrQueue(shaderProgram, name); }
        void setShader(std::string const& name, Executable exec);

        ShaderProgram const& getProgram(void) const noexcept
            { return *shaderProgram; }

        virtual ~Shadeable(void) noexcept = default;
    protected:
        explicit Shadeable(ProgramPtr const& program);
        explicit Shadeable(ProgramPtr&& program) noexcept;
        explicit Shadeable(std::string const& programName);
        explicit Shadeable(std::string const& programName,
            Executable exec);
        explicit Shadeable(void);

        ProgramPtr                                  shaderProgram;
    };

}
