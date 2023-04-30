/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Core/Shaders/ShaderProgram.hpp>
#include <MPGL/Core/Context/Context.hpp>

namespace mpgl {

    /**
     * Base class for all classes that contains shader program.
     * Simplifies the shader initialization process
     */
    class Shadeable : virtual protected GraphicalObject {
    public:
        typedef std::shared_ptr<ShaderProgram>      ProgramPtr;

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the shader program
         * object
         */
        virtual void setShader(ShaderProgram const& program) noexcept
            { *shaderProgram = program; }

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the shader program
         * object
         */
        virtual void setShader(ShaderProgram&& program) noexcept
            { *shaderProgram = std::move(program); }

        /**
         * Sets the given shader program
         *
         * @throws ShaderLibraryNotLoadedException if shader library
         * is not loaded yet
         * @param name the name of the shader program
         */
        virtual void setShader(std::string const& name);

        /**
         * Returns the constant reference to the shader program
         * object
         *
         * @return the constant reference to the shader program
         * object
         */
        [[nodiscard]] ShaderProgram const& getProgram(
            void) const noexcept
                { return *shaderProgram; }

        /**
         * Virtual destructor. Destroys the Shadeable object
         */
        virtual ~Shadeable(void) noexcept = default;
    protected:
        /**
         * Constructs a new Shadeable object from the given
         * constant reference to the shader program pointer
         * object
         *
         * @param program the constant reference to the shader
         * program pointer object
         */
        explicit Shadeable(ProgramPtr const& program);

        /**
         * Constructs a new Shadeable object from the given
         * rvalue reference to the shader program pointer
         * object
         *
         * @param program the rvalue reference to the shader
         * program pointer object
         */
        explicit Shadeable(ProgramPtr&& program) noexcept;

        /**
         * Constructs a new Shadeable object from the given
         * constant reference to the string with the shader
         * program name
         *
         * @throws ShaderLibraryNotLoadedException if shader library
         * is not loaded yet
         * @param programName the shader program name
         */
        explicit Shadeable(std::string const& programName);

        /**
         * Constructs a new Shadeable object
         */
        explicit Shadeable(void);

        /**
         * Constructs a new Shadeable object from the given constant
         * reference to the shadeable object
         *
         * @param shadeable the constant reference to the shadeable
         * object
         */
        Shadeable(Shadeable const& shadeable);

        /**
         * Constructs a new Shadeable object from the given rvalue
         * reference to the shadeable object
         *
         * @param shadeable the rvalue reference to the shadeable
         * object
         */
        Shadeable(Shadeable&& shadeable) noexcept;

        /**
         * Assigns the given constant reference to the shadeable
         * object to this object
         *
         * @param shadeable the constant reference to the shadeable
         * object
         * @return the reference to this object
         */
        Shadeable& operator=(Shadeable const& shadeable);

        /**
         * Assigns the given rvalue reference to the shadeable
         * object to this object
         *
         * @param shadeable the rvalue reference to the shadeable
         * object
         * @return the reference to this object
         */
        Shadeable& operator=(Shadeable&& shadeable) noexcept;

        ProgramPtr                                  shaderProgram;
    };

}
