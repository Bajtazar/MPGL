/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include <MPGL/Core/Context/ShadersContext.hpp>

namespace mpgl {

    void ShadersContext::setLibrary(ShaderLibrary const& library) {
        shaders = library;
        std::exception_ptr exception;
        while (!tupleQueue.empty())
            setShaderFromTupleQueue(exception, library);
        while (!pairQueue.empty())
            setShaderFromPairQueue(exception, library);
        while (!executables.empty())
            runExecutable(exception);
        if (exception)
            std::rethrow_exception(exception);
    }

    void ShadersContext::setShaderFromPairQueue(
        std::exception_ptr& exception,
        ShaderLibrary const& library) noexcept
    {
        try {
            auto&& [shader, name] = std::move(pairQueue.front());
            pairQueue.pop();
            if (auto program = shader.lock())
                *program = library[name];
        } catch (...) {
            exception = std::current_exception();
        }
    }

    void ShadersContext::setShaderFromTupleQueue(
        std::exception_ptr& exception,
        ShaderLibrary const& library) noexcept
    {
        try {
            auto&& [shader, name, exec] = std::move(tupleQueue.front());
            tupleQueue.pop();
            if (auto program = shader.lock())
                exec(*program = library[name]);
        } catch (...) {
            exception = std::current_exception();
        }
    }

    void ShadersContext::runExecutable(
        std::exception_ptr& exception) noexcept
    {
        try {
            auto&& exec = std::move(executables.front());
            executables.pop();
            exec();
        } catch (...) {
            exception = std::current_exception();
        }
    }

    [[nodiscard]] ShadersContext::Library ShadersContext::getLibrary(
        void) const noexcept
    {
        if (isHolding())
            return {std::get<ShaderLibrary>(shaders)};
        return std::nullopt;
    }

    void ShadersContext::setOrQueue(
        ProgramPtr& pointer,
        std::string const& name)
    {
        if (isHolding())
            *pointer = std::get<ShaderLibrary>(shaders)[name];
        else
            pairQueue.emplace(pointer, name);
    }

    void ShadersContext::setOrQueue(
        ProgramPtr& pointer,
        std::string const& name,
        Executable exec)
    {
        if (isHolding())
            exec(*pointer = std::get<ShaderLibrary>(shaders)[name]);
        else
            tupleQueue.emplace(pointer, name, std::move(exec));
    }

    void ShadersContext::executeOrQueue(IndependentExecutable exec) {
        if (isHolding())
            exec();
        else
            executables.emplace(std::move(exec));
    }

}
