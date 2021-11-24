#include "ShadersContext.hpp"

namespace ge {

    void ShadersContext::setLibrary(ShaderLibrary const& library,
        WindowID windowID)
    {
        shaders = library;
        currentWindowID = windowID;
        std::exception_ptr exception;
        while (!queue.empty())
            setShaderFromQueue(exception, library);
        while (!executables.empty())
            runExecutable(exception);
        if (exception)
            std::rethrow_exception(exception);
    }

    void ShadersContext::setShaderFromQueue(
        std::exception_ptr& exception,
        ShaderLibrary const& library) noexcept
    {
        try {
            auto& [shader, name] = queue.front();
            *shader = library[name];
            queue.pop();
        } catch (...) {
            exception = std::current_exception();
        }
    }

    void ShadersContext::runExecutable(std::exception_ptr& exception)
        noexcept
    {
        try {
            auto& [shader, exec] = executables.front();
            exec(shader);
            executables.pop();
        } catch (...) {
            exception = std::current_exception();
        }
    }

    void ShadersContext::removeLibrary(WindowID windowID) noexcept {
        if (windowID == currentWindowID)
            shaders = std::monostate{};
    }

    ShadersContext::Library ShadersContext::getLibrary(void) const noexcept {
        if (std::holds_alternative<ShaderLibrary>(shaders))
            return {std::get<ShaderLibrary>(shaders)};
        return {};
    }

    void ShadersContext::setOrQueue(ProgramPtr& pointer,
        std::string const& name)
    {
        if (std::holds_alternative<ShaderLibrary>(shaders))
            *pointer = std::get<ShaderLibrary>(shaders)[name];
        else
            queue.emplace(pointer, name);
    }

    void ShadersContext::setOrQueue(ProgramPtr& pointer,
        std::string const& name, Executable exec)
    {
        if (std::holds_alternative<ShaderLibrary>(shaders)) {
            *pointer = std::get<ShaderLibrary>(shaders)[name];
            exec(pointer);
        } else {
            queue.emplace(pointer, name);
            executables.emplace(pointer, std::move(exec));
        }
    }

}
