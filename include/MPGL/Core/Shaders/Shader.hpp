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

#include <glad/glad.h>
#include <concepts>
#include <string>

#include <MPGL/Traits/Types.hpp>

namespace mpgl {

    /**
     * Manages the OpenGL shader object
     *
     * @tparam ShaderType the type of the shader
     */
    template <bool ShaderType>
    class Shader {
    public:
        /**
         * Constructs a new Shader object. Loads the shader from
         * the given path
         *
         * @throw ShaderCompilationError when shader cannot be
         * loaded from file or shader cannot be compiled
         * @param shaderPath the constant reference to the string
         * with shader's path
         */
        explicit Shader(std::string shaderPath);

        /**
         * Constructs a new Shader object. Loads the shader from
         * the given range. The last element in range has to be 0!
         *
         * @throw ShaderCompilationError when shader cannot be
         * loaded from file or shader cannot be compiled
         * @throw ShaderMissingSentinelException when the last
         * element in the range is not 0
         * @tparam Range the range's type
         * @param range the universal reference to the range with
         * shader
         */
        template <std::ranges::contiguous_range Range>
            requires std::same_as<std::ranges::range_value_t<Range>,
                char>
        explicit Shader(Range&& range);

        Shader(Shader const& shader) noexcept = delete;

        /**
         * Constructs a new Shader object from the given rvalue
         * reference to the shader object
         *
         * @param shader the rvalue reference to the shader object
         */
        Shader(Shader&& shader) noexcept;

        Shader& operator= (Shader const& shader) noexcept = delete;

        /**
         * Assigns the given rvalue reference to shader to the this
         * object
         *
         * @param shader the rvalue reference to the assigned
         * shader object
         * @return the reference to this object
         */
        Shader& operator= (Shader&& shader) noexcept;

        /**
         * Returns the holded id of the OpenGL shader object
         *
         * @return the id of the OpenGL shader object
         */
        [[nodiscard]] uint32 getShader(void) const noexcept
            { return shaderID; }

        /**
         * Destroys the Shader object
         */
        ~Shader(void) noexcept;
    private:
        uint32                          shaderID;

        /**
         * Returns the shader type based on the ShaderType boolean
         *
         * @return the shader type
         */
        constexpr auto shaderType(void) const noexcept;

        /**
         * Verifies the shader compilation status
         *
         * @throw ShaderCompilationException when compilation
         * failed
         */
        void verifyCompilationStatus(void) const;

        /**
         * Loads shader from the given range
         *
         * @throw ShaderCompilationError when shader cannot be
         * loaded from file or shader cannot be compiled
         * @tparam Range the range's type
         * @param range the universal reference to the range with
         * shader
         */
        template <std::ranges::contiguous_range Range>
            requires std::same_as<std::ranges::range_value_t<Range>,
                char>
        void loadShader(Range&& range);
    };

    template class Shader<true>;
    template class Shader<false>;

    typedef Shader<true>                VertexShader;
    typedef Shader<false>               FragmentShader;

}

#include <MPGL/Core/Shaders/Shader.ipp>
#include <MPGL/Core/Shaders/Shader.tpp>
