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
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Figures/Primitives/Ring.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] RingOutlineCalculator<dim::Dim3, Spec>::MatrixT
        RingOutlineCalculator<dim::Dim3, Spec>::getXYMatrix(
            Vector2f const& xVersor,
            Vector2f const& yVersor) const noexcept
    {
        Matrix2f matrix = *invert(Matrix2f{xVersor, yVersor});
        return {
            Vector3f{matrix[0][0], matrix[0][1], 0.f},
            Vector3f{matrix[1][0], matrix[1][1], 0.f}
        };
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] RingOutlineCalculator<dim::Dim3, Spec>::MatrixT
        RingOutlineCalculator<dim::Dim3, Spec>::getXZMatrix(
            Vector2f const& xVersor,
            Vector2f const& zVersor) const noexcept
    {
        Matrix2f matrix = *invert(Matrix2f{xVersor, zVersor});
        return {
            Vector3f{matrix[0][0], 0.f, matrix[0][1]},
            Vector3f{matrix[1][0], 0.f, matrix[1][1]}
        };
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] RingOutlineCalculator<dim::Dim3, Spec>::MatrixT
        RingOutlineCalculator<dim::Dim3, Spec>::getYZMatrix(
            Vector2f const& yVersor,
            Vector2f const& zVersor) const noexcept
    {
        Matrix2f matrix = *invert(Matrix2f{yVersor, zVersor});
        return {
            Vector3f{0.f, matrix[0][0], matrix[0][1]},
            Vector3f{0.f, matrix[1][0], matrix[1][1]}
        };
    }

    template <EllipticTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] bool RingClickChecker<dim::Dim2, Spec>::operator() (
        Ring<dim::Dim2, Spec> const& ring,
        Vector2u const& position) const noexcept
    {
        bool outring = insideSystem(ring.vertices | views::position,
            ring.outline, position);
        bool inring = insideSystem(ring.innerEllipse.vertices,
            ring.innerEllipse.outline, position);
        return outring && (!inring);
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] bool RingClickChecker<dim::Dim3, Spec>::operator() (
        Ring<dim::Dim3, Spec> const& ring,
        Vector2u const& position) const noexcept
    {
        bool outring = insideSystem(ring.vertices | views::position,
            ring.model, position);
        bool inring = insideSystem(ring.innerEllipse.vertices,
            ring.model, position);
        return outring && (!inring);
    }

    std::string const RingShader<dim::Dim2, void>::shader
        = "MPGL/2D/Ring";

    std::string const RingShader<dim::Dim2, int32>::shader
        = "MPGL/2D/CRing";

    std::string const RingShader<dim::Dim3, void>::shader
        = "MPGL/3D/Ring";

    std::string const RingShader<dim::Dim3, int32>::shader
        = "MPGL/3D/CRing";

    void RingShader<dim::Dim2, void>::operator() (
        ShaderProgram const& program) const noexcept
    {
        ShaderLocation{program, "aafactor"}(
            float32(context.windowOptions.antiAliasingSamples) / 4.f);
    }

    void RingShader<dim::Dim2, int32>::operator() (
        ShaderProgram const& program) const noexcept
    {
        RingShader<dim::Dim2, void>{}(program);
    }

    void RingShader<dim::Dim3, void>::operator() (
        ShaderProgram const& program) const noexcept
    {
        RingShader<dim::Dim2, void>{}(program);
    }

    void RingShader<dim::Dim3, int32>::operator() (
        ShaderProgram const& program) const noexcept
    {
        RingShader<dim::Dim2, void>{}(program);
    }

}
