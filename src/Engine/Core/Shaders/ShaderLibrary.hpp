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

#include "ShaderProgram.hpp"

#include <string>
#include <vector>
#include <map>

namespace mpgl {

    /**
     * Loads and stores shader programs found in designated
     * folder
     */
    class ShaderLibrary {
    public:
        typedef std::map<std::string, ShaderProgram>    ProgramMap;

        /**
         * Initializes and saves all of the found shader programs.
         * Raises exception when shaders are invalid
         *
         * @throw ShaderLibraryInvalidShadersException when there
         * are shaders without pair
         * @throw ShaderCompilationException when there is an error
         * in shader compilation
         * @throw ShaderProgramLinkingException when there is an error
         * in shader program linking
         * @throw std::filesystem errors during file IO
         */
        explicit ShaderLibrary(void);

        using const_iterator = typename ProgramMap::const_iterator;
        using const_reverse_iterator
            = typename ProgramMap::const_reverse_iterator;

        /**
         * Returns a constant iterator to the begining of the
         * shaders map
         *
         * @return the constant iterator to the begining of the
         * shaders map
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return programs.begin(); }

        /**
         * Returns a constant iterator to the end of the shaders map
         *
         * @return the constant iterator to the end of the
         * shaders map
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return programs.end(); }

        /**
         * Returns a constant iterator to the begining of the
         * shaders map
         *
         * @return the constant iterator to the begining of the
         * shaders map
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return programs.begin(); }

        /**
         * Returns a constant iterator to the end of the shaders map
         *
         * @return the constant iterator to the end of the
         * shaders map
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return programs.end(); }

        /**
         * Returns a constant reverse iterator to the end of the
         * shaders map
         *
         * @return the constant reverse iterator to the end of the
         * shaders map
         */
        [[nodiscard]] const_reverse_iterator rbegin(void) const noexcept
            { return programs.rbegin(); }

        /**
         * Returns a constant reverse iterator to the begining of the
         * shaders map
         *
         * @return the constant reverse iterator to the begining of the
         * shaders map
         */
        [[nodiscard]] const_reverse_iterator rend(void) const noexcept
            { return programs.rend(); }

        /**
         * Returns a constant reverse iterator to the end of the
         * shaders map
         *
         * @return the constant reverse iterator to the end of the
         * shaders map
         */
        [[nodiscard]] const_reverse_iterator crbegin(void) const noexcept
            { return programs.rbegin(); }

        /**
         * Returns a constant reverse iterator to the begining of the
         * shaders map
         *
         * @return the constant reverse iterator to the begining of the
         * shaders map
         */
        [[nodiscard]] const_reverse_iterator crend(void) const noexcept
            { return programs.rend(); }

        /**
         * Returns a constant reference to the program with a given
         * name
         *
         * @param name the name of the shader program
         * @throw std::out_of_range when there is no shader program
         * with given name
         * @return the constant reference to the program
         */
        [[nodiscard]] ShaderProgram const& operator[] (
            std::string const& name) const
                { return programs.at(name); }
    private:
        typedef std::vector<std::string>                Paths;

        ProgramMap                                      programs;

        /**
         * Returns a vector with shader names
         *
         * @return the vector with shader names
         */
        Paths getShaderList(void) const;

        /**
         * Returns whether both vectors contains same shaders names
         *
         * @param vertexShaders the vertex shaders list
         * @param fragmentShaders the fragment shaders list
         * @return if both vectors contains same shaders names
         */
        static bool sameShaders(
            Paths const& vertexShaders,
            Paths const& fragmentShaders) noexcept;
    };

}
