#pragma once

#include "ShaderProgram.hpp"
#include "../Drawable.hpp"

namespace ge {

    class Shadeable : virtual public Drawable {
    public:
        typedef std::shared_ptr<ShaderProgram>      ProgramPtr;

        virtual void setShader(ShaderProgram const& program) noexcept
            { *shaderProgram = program; }
        virtual void setShader(ShaderProgram&& program) noexcept
            { *shaderProgram = std::move(program); }
        virtual void setShader(std::string const& name)
            { context.shaders.setOrQueue(shaderProgram, name); }

        ShaderProgram const& getProgram(void) const noexcept
            { return *shaderProgram; }

        virtual void copyToGPU(void) const noexcept = 0;
        virtual void draw(void) const noexcept = 0;

        virtual ~Shadeable(void) noexcept = default;
    protected:
        typedef typename ShadersContext::Executable Executable;

        explicit Shadeable(ProgramPtr const& program);
        explicit Shadeable(ProgramPtr&& program) noexcept;
        explicit Shadeable(std::string const& programName);
        explicit Shadeable(std::string const& programName,
            Executable exec);
        explicit Shadeable(void);

        void setShader(std::string const& program,
            Executable exec);

        ProgramPtr                                  shaderProgram;
    };

}
