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
#pragma once

#include "../Shaders/ShaderLibrary.hpp"

#include <queue>
#include <variant>
#include <optional>
#include <exception>
#include <functional>

namespace mpgl {

    /**
     * Manages the shader library distributed via context
     */
    class ShadersContext {
    public:
        typedef std::optional<ShaderLibrary>    Library;
        typedef std::shared_ptr<ShaderProgram>  ProgramPtr;
        typedef std::function<void(
            ProgramPtr&)>                       Executable;

        /**
         * Constructs a new Shaders Context object
         */
        explicit ShadersContext(void) noexcept = default;

        /**
         * Sets the new shader library
         *
         * @throw Errors occured during initialization of
         * shaders in the queue
         * @param library the constant reference to
         * the shader library
         */
        void setLibrary(ShaderLibrary const& library);

        /**
         * Sets the libraty to the undefined state
         */
        void removeLibrary(void) noexcept
            { shaders = std::monostate{}; }

        /**
         * Returns an optional with the shader library. If the
         * shader library is undefined then returns an empty
         * optional
         *
         * @return the optional with the shader library
         */
        [[nodiscard]] Library getLibrary(void) const noexcept;

        /**
         * Sets the shader program with the given name in the
         * pointer is available. Otherwise sets the shader
         * to the queue and waits until the library is available
         *
         * @param pointer the reference to the shader program
         * shadred pointer
         * @param name the constant reference to the shader program
         * name
         */
        void setOrQueue(
            ProgramPtr& pointer,
            std::string const& name);

        /**
         * Sets the shader program with the given name in the
         * pointer is available and executes associated
         * executable. Otherwise sets the shader and executable
         * to the queue and waits until the library is available
         *
         * @param pointer the reference to the shader program
         * shadred pointer
         * @param name the constant reference to the shader program
         * name
         * @param exec the executable called after shader program
         * shared pointer initialization
         */
        void setOrQueue(
            ProgramPtr& pointer,
            std::string const& name,
            Executable exec);

        /**
         * Returns whether the shader library is defined
         *
         * @return if the shader library is defined
         */
        [[nodiscard]] bool isHolding(void) const noexcept
            { return std::holds_alternative<ShaderLibrary>(shaders); }
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

        /**
         * Initializes the shaders from the shader program queue
         *
         * @param exception the reference to the exception object
         * @param library the constant reference to the shader
         * library object
         */
        void setShaderFromQueue(
            std::exception_ptr& exception,
            ShaderLibrary const& library) noexcept;

        /**
         * Executes the executable associated with the shader program
         * shared pointer
         *
         * @param exception the reference to the exception object
         */
        void runExecutable(std::exception_ptr& exception) noexcept;
    };

}
