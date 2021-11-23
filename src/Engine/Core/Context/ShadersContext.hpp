#pragma once

#include "../Figures/ShaderLibrary.hpp"

#include <queue>
#include <variant>
#include <optional>
#include <exception>
#include <functional>

namespace ge {

    class ShadersContext {
    public:
        typedef std::optional<ShaderLibrary>    Library;
        typedef std::shared_ptr<ShaderProgram>  ProgramPtr;
        typedef void*                           WindowID;
        typedef std::function<void(
            ProgramPtr&)>                       Executable;

        explicit ShadersContext(void) noexcept
            : currentWindowID{nullptr} {}

        void setLibrary(ShaderLibrary const& library,
            WindowID windowID);
        void removeLibrary(WindowID windowID) noexcept;

        Library getLibrary(void) const noexcept;

        void setOrQueue(ProgramPtr& pointer,
            std::string const& name);
        void setOrQueue(ProgramPtr& pointer,
            std::string const& name, Executable exec);
    private:
        typedef std::pair<ProgramPtr,
            Executable>                         ExecutablePair;
        typedef std::pair<ProgramPtr,
            std::string>                        ShaderPair;
        typedef std::queue<ShaderPair>          Queue;
        typedef std::queue<ExecutablePair>      Executables;
        typedef std::variant<std::monostate,
            ShaderLibrary>                      Shaders;

        Queue                                   queue;
        Shaders                                 shaders;
        Executables                             executables;
        WindowID                                currentWindowID;

        void setShaderFromQueue(std::exception_ptr& exception,
            ShaderLibrary const& library) noexcept;
        void runExecutable(std::exception_ptr& exception) noexcept;
    };

}
