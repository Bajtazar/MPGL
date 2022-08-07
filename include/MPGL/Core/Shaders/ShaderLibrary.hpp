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

#include <MPGL/Core/Shaders/ShaderProgram.hpp>
#include <MPGL/Core/Shaders/GZSLLoader.hpp>

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
        typedef std::string                             Path;
        typedef std::vector<std::string>                Paths;

        /**
         * Initializes and saves all of the found shader programs.
         * Raises exception when shaders are invalid
         *
         * @param locations a constant reference to the vector
         * containing looked directories or gzsl packages
         * @throw ShaderLibraryInvalidShadersException when there
         * are shaders without pair
         * @throw ShaderCompilationException when there is an error
         * in shader compilation
         * @throw ShaderProgramLinkingException when there is an error
         * in shader program linking
         * @throw std::filesystem errors during file IO
         */
        explicit ShaderLibrary(Paths const& locations);

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
         * Returns a constant reference to the program with the given
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
        using ShaderMap = typename GZSLLoader<>::ShaderMap;

        ProgramMap                                      programs;

        /**
         * Returns a vector with shader names
         *
         * @param path a constant reference to a string with
         * the looked directory
         * @return the vector with shader names
         */
        Paths getShaderList(Path const& path) const;

        /**
         * Returns a vector with shader names
         *
         * @param path a constant reference to a map with
         * shaders
         * @return the vector with shader names
         */
        Paths getShaderList(ShaderMap const& map) const;

        /**
         * Loads shaders from the given directory
         *
         * @param path a constant reference to a string with
         * the looked directory path
         */
        void loadShaderDirectory(Path const& path);

        /**
         * Loads shaders from the given GZLS package
         *
         * @param path a constant reference to a string with
         * the looked GZLS package path
         */
        void loadPackage(Path const& path);

        /**
         * Returns whether both vectors contains same shaders names
         *
         * @param vertexShadersPaths the vertex shaders list
         * @param fragmentShadersPaths the fragment shaders list
         * @param path path to the shader's directory
         * @return if both vectors contains same shaders names
         */
        static bool sameShaders(
            Paths const& vertexShadersPaths,
            Paths const& fragmentShadersPaths,
            Path const& path) noexcept;

        /**
         * Returns a path to the vertex shaders in the given
         * directory
         *
         * @param path the path to the directory
         * @return the path to the vertex shaders in the given
         * directory
         */
        static Path vertexShaders(Path const& path);

        /**
         * Returns a path to the fragment shaders in the given
         * directory
         *
         * @param path the path to the directory
         * @return the path to the fragment shaders in the given
         * directory
         */
        static Path fragmentShaders(Path const& path);

        /**
         * Checks whether the given path is the GZSL's package
         *
         * @param path the path
         * @return if the given path is the GZSL's package
         */
        static bool isPackage(Path const& path) noexcept;
    };

}
