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
#include <MPGL/Core/Figures/Primitives/Ring.hpp>

namespace mpgl {

    std::string const RingShader<dim::Dim2, void>::shader
        = "MPGL/2D/Ring";

    std::string const RingShader<dim::Dim3, void>::shader
        = "MPGL/3D/Ring";

    std::string const RingShader<dim::Dim2, uint8>::shader
        = "MPGL/2D/TRing";

    std::string const RingShader<dim::Dim3, uint8>::shader
        = "MPGL/3D/TRing";

    std::string const RingShader<dim::Dim2, uint8>::convolutionShader
        = "MPGL/2D/TRingConv";

    std::string const RingShader<dim::Dim3, uint8>::convolutionShader
        = "MPGL/3D/TRingConv";

    void RingShader<dim::Dim2, void>::operator() (
        ShaderProgram const& program) const noexcept
    {
        ShaderLocation{program, "aafactor"}(
            float32(context.windowOptions.antiAliasingSamples) / 4.f);
    }

    void RingShader<dim::Dim3, void>::operator() (
        ShaderProgram const& program) const noexcept
    {
        RingShader<dim::Dim2, void>{}(program);
    }

    void RingShader<dim::Dim2, uint8>::operator() (
        ShaderProgram const& program) const noexcept
    {
        ShaderLocation{program, "tex"}(0);
        ShaderLocation{program, "aafactor"}(
            float32(context.windowOptions.antiAliasingSamples) / 4.f);
    }

    void RingShader<dim::Dim3, uint8>::operator() (
        ShaderProgram const& program) const noexcept
    {
        RingShader<dim::Dim2, uint8>{}(program);
    }

    template class RingOutlineCalculator<dim::Dim2, void>;
    template class RingOutlineCalculator<dim::Dim3, void>;
    template class RingOutlineCalculator<dim::Dim2, uint8>;
    template class RingOutlineCalculator<dim::Dim3, uint8>;

    template class RingClickChecker<dim::Dim2, void>;
    template class RingClickChecker<dim::Dim3, void>;
    template class RingClickChecker<dim::Dim2, uint8>;
    template class RingClickChecker<dim::Dim3, uint8>;

}
