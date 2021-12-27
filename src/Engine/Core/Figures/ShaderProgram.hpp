#pragma once

#include "Shader.hpp"
#include "../Color.hpp"
#include "../../Traits/Concepts.hpp"
#include "../../Mathematics/Matrix.hpp"

#include <memory>

namespace ge {

    class ShaderProgram {
    public:
        explicit ShaderProgram(void) noexcept;
        explicit ShaderProgram(VertexShader const& vertex,
            FragmentShader const& fragment) noexcept;

        template <bool Type>
        void attachShader(Shader<Type> const& shader) const noexcept;

        void link(std::string const& programName = {}) const;

        bool exist(void) const noexcept
            { return *shaderProgramID != 0; }

        operator bool() const noexcept
            { return *shaderProgramID != 0; }

        inline void use(void) const noexcept;

        template <AllIntegrals... Ints>
            requires (sizeof...(Ints) <= 4 && sizeof...(Ints) != 0
                && !AllUnsignedIntegrals<Ints...>)
        inline void setUniform(std::string const& uniform,
            Ints... values) const noexcept;

        template <AllUnsignedIntegrals... UInts>
            requires (sizeof...(UInts) <= 4 && sizeof...(UInts) != 0)
        inline void setUniform(std::string const& uniform,
            UInts... values) const noexcept;

        template <AllFloatingPoints... Floats>
            requires (sizeof...(Floats) <= 4 && sizeof...(Floats) != 0)
        inline void setUniform(std::string const& uniform,
            Floats... values) const noexcept;

        inline void setUniform(std::string const& uniform,
            Color const& color) const noexcept;

        template <Arithmetic Tp, std::size_t Size>
            requires (Size <= 4 && Size > 1)
        inline void setUniform(std::string const& uniform,
            Vector<Tp, Size> const& vector) const noexcept;

        template <std::size_t Size>
            requires (Size <= 4 && Size > 1)
        inline void setUniform(std::string const& uniform,
            Matrix<float, Size, Size> const& matrix) const noexcept;
    private:
        class ProgramDeleter {
        public:
            explicit ProgramDeleter(void) noexcept = default;

            void operator()(uint32_t* ptr) const noexcept;
        };

        inline uint32_t location(std::string const& uniform) const noexcept;

        std::shared_ptr<uint32_t>           shaderProgramID;

        static uint32_t                     lastProgramID;
    };

    inline void ShaderProgram::use(void) const noexcept {
        if (lastProgramID != *shaderProgramID) {
            glUseProgram(*shaderProgramID);
            lastProgramID = *shaderProgramID;
        }
    }

    template <bool Type>
    void ShaderProgram::attachShader(Shader<Type> const& shader) const noexcept {
        glAttachShader(*shaderProgramID, shader.getShader());
    }

    inline uint32_t ShaderProgram::location(std::string const& uniform) const noexcept {
        return glGetUniformLocation(*shaderProgramID, uniform.c_str());
    }

    template <AllIntegrals... Ints>
        requires (sizeof...(Ints) <= 4 && sizeof...(Ints) != 0
            && !AllUnsignedIntegrals<Ints...>)
    inline void ShaderProgram::setUniform(std::string const& uniform,
        Ints... values) const noexcept
    {
        if constexpr (sizeof...(Ints) == 1)
            glUniform1i(location(uniform), values...);
        else if constexpr (sizeof...(Ints) == 2)
            glUniform2i(location(uniform), values...);
        else if constexpr (sizeof...(Ints) == 3)
            glUniform3i(location(uniform), values...);
        else
            glUniform4i(location(uniform), values...);
    }

    template <AllUnsignedIntegrals... UInts>
        requires (sizeof...(UInts) <= 4 && sizeof...(UInts) != 0)
    inline void ShaderProgram::setUniform(std::string const& uniform,
        UInts... values) const noexcept
    {
        if constexpr (sizeof...(UInts) == 1)
            glUniform1ui(location(uniform), values...);
        else if constexpr (sizeof...(UInts) == 2)
            glUniform2ui(location(uniform), values...);
        else if constexpr (sizeof...(UInts) == 3)
            glUniform3ui(location(uniform), values...);
        else
            glUniform4ui(location(uniform), values...);
    }

    template <AllFloatingPoints... Floats>
        requires (sizeof...(Floats) <= 4 && sizeof...(Floats) != 0)
    inline void ShaderProgram::setUniform(std::string const& uniform,
        Floats... values) const noexcept
    {
        if constexpr (sizeof...(Floats) == 1)
            glUniform1f(location(uniform), values...);
        else if constexpr (sizeof...(Floats) == 2)
            glUniform2f(location(uniform), values...);
        else if constexpr (sizeof...(Floats) == 3)
            glUniform3f(location(uniform), values...);
        else
            glUniform4f(location(uniform), values...);
    }

    template <Arithmetic Tp, std::size_t Size>
        requires (Size <= 4 && Size > 1)
    inline void ShaderProgram::setUniform(std::string const& uniform,
        Vector<Tp, Size> const& vector) const noexcept
    {
        [&]<std::size_t... Index>(std::index_sequence<Index...>) {
            setUniform(uniform, std::get<Size - Index - 1>(
                static_cast<UniformTuple<Tp, Size> const&>(vector))...);
        }(std::make_index_sequence<Size>{});
    }

    inline void ShaderProgram::setUniform(std::string const& uniform,
        Color const& color) const noexcept
    {
        setUniform(uniform, static_cast<Vector4f const&>(color));
    }

    template <std::size_t Size>
        requires (Size <= 4 && Size > 1)
    inline void ShaderProgram::setUniform(std::string const& uniform,
        Matrix<float, Size, Size> const& matrix) const noexcept
    {
        float const* const memory = reinterpret_cast<float const* const>(&matrix);
        if constexpr (Size == 4)
            glUniformMatrix4fv(location(uniform), 1, GL_FALSE, memory);
        else if constexpr (Size == 3)
            glUniformMatrix3fv(location(uniform), 1, GL_FALSE, memory);
        else
            glUniformMatrix2fv(location(uniform), 1, GL_FALSE, memory);
    }

}
