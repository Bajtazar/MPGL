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
#include <MPGL/Core/Figures/Primitives/Ellipse.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/Shaders/ShaderLocation.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Figures/Views.hpp>

namespace mpgl {

    template <EllipticTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] bool EllipseClickChecker<dim::Dim2, Spec>::operator() (
        Ellipse<dim::Dim2, Spec> const& ellipse,
        Vector2u const& position) const noexcept
    {
        Vector2f local = ellipse.outlineTransform * (
            vectorCast<float32>(position) -
            static_cast<Vector2f>(get<"position">(ellipse.vertices.front())));
        return (local - 0.5f).length() <= 0.5f;
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] bool EllipseClickChecker<dim::Dim3, Spec>::operator() (
        Ellipse<dim::Dim3, Spec> const& ellipse,
        Vector2u const& position) const noexcept
    {
        auto iter = (ellipse.vertices | views::position
            | views::project(ellipse.model)).begin();
        auto const p = Adapter2D{position}.get();
        auto const v0 = Adapter2D{*iter++}.get();
        auto const v1 = Adapter2D{*iter++}.get();
        auto const v2 = Adapter2D{*iter++}.get();
        auto const v3 = Adapter2D{*iter++}.get();
        return isInsideTriangle(p, v0, v1, v2) ||
            isInsideTriangle(p, v0, v2, v3);
    }

    template <EllipticTraitSpecifier<dim::Dim2> Spec>
    [[nodiscard]] EllipseOutlineCalculator<dim::Dim2, Spec>::ResultT
        EllipseOutlineCalculator<dim::Dim2, Spec>::operator() (
            Ellipse<dim::Dim2, Spec> const& ellipse) const noexcept
    {
        return invert(Matrix2f{tags::transposedTag,
            static_cast<Vector2f>(get<"position">(ellipse.vertices[1]))
                - static_cast<Vector2f>(get<"position">(ellipse.vertices[0])),
            static_cast<Vector2f>(get<"position">(ellipse.vertices[3]))
                - static_cast<Vector2f>(get<"position">(ellipse.vertices[0]))});
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] EllipseOutlineCalculator<dim::Dim3, Spec>::Versors
        EllipseOutlineCalculator<dim::Dim3, Spec>::getVersors(
            Ellipse<dim::Dim3, Spec> const& ellipse) const noexcept
    {
        Vector3f const a = static_cast<Vector3f>(get<"position">(
            ellipse.vertices[3])) - static_cast<Vector3f>(get<"position">(
                ellipse.vertices[0]));
        Vector3f const b = static_cast<Vector3f>(get<"position">(
            ellipse.vertices[1])) - static_cast<Vector3f>(get<"position">(
                ellipse.vertices[0]));
        return {
            Vector2f{a[0], b[0]},
            Vector2f{a[1], b[1]},
            Vector2f{a[2], b[2]}
        };
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] EllipseOutlineCalculator<dim::Dim3, Spec>::ResultT
        EllipseOutlineCalculator<dim::Dim3, Spec>::operator() (
            Ellipse<dim::Dim3, Spec> const& ellipse) const noexcept
    {
        auto const& [xVersor, yVersor, zVersor] = getVersors(ellipse);
        if (cross(xVersor, yVersor))
            return { getXYMatrix(xVersor, yVersor) };
        else if (cross(xVersor, zVersor))
            return { getXZMatrix(xVersor, zVersor) };
        else if (cross(yVersor, zVersor))
            return { getYZMatrix(yVersor, zVersor) };
        return std::nullopt;
    }

    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    [[nodiscard]] EllipseOutlineCalculator<dim::Dim3, Spec>::MatrixT
        EllipseOutlineCalculator<dim::Dim3, Spec>::getXYMatrix(
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
    [[nodiscard]] EllipseOutlineCalculator<dim::Dim3, Spec>::MatrixT
        EllipseOutlineCalculator<dim::Dim3, Spec>::getXZMatrix(
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
    [[nodiscard]] EllipseOutlineCalculator<dim::Dim3, Spec>::MatrixT
        EllipseOutlineCalculator<dim::Dim3, Spec>::getYZMatrix(
            Vector2f const& yVersor,
            Vector2f const& zVersor) const noexcept
    {
        Matrix2f matrix = *invert(Matrix2f{yVersor, zVersor});
        return {
            Vector3f{0.f, matrix[0][0], matrix[0][1]},
            Vector3f{0.f, matrix[1][0], matrix[1][1]}
        };
    }

    std::string const EllipseShader<dim::Dim2, void>::shader
        = "MPGL/2D/Ellipse";

    std::string const EllipseShader<dim::Dim3, void>::shader
        = "MPGL/3D/Ellipse";

    std::string const EllipseShader<dim::Dim2, uint8>::shader
        = "MPGL/2D/TEllipse";

    std::string const EllipseShader<dim::Dim3, uint8>::shader
        = "MPGL/3D/TEllipse";

    std::string const EllipseShader<dim::Dim2, uint8>::convolutionShader
        = "MPGL/2D/TEllipseConv";

    std::string const EllipseShader<dim::Dim3, uint8>::convolutionShader
        = "MPGL/3D/TEllipseConv";

    void EllipseShader<dim::Dim2, void>::operator() (
        ShaderProgram const& program) const noexcept
    {
        ShaderLocation{program, "aafactor"}(
            float32(context.windowOptions.antiAliasingSamples) / 4.f);
    }

    void EllipseShader<dim::Dim3, void>::operator() (
        ShaderProgram const& program) const noexcept
    {
        EllipseShader<dim::Dim2, void>{}(program);
    }

    void EllipseShader<dim::Dim2, uint8>::operator() (
        ShaderProgram const& program) const noexcept
    {
        ShaderLocation{program, "tex"}(0);
        ShaderLocation{program, "aafactor"}(
            float32(context.windowOptions.antiAliasingSamples) / 4.f);
    }

    void EllipseShader<dim::Dim3, uint8>::operator() (
        ShaderProgram const& program) const noexcept
    {
        EllipseShader<dim::Dim2, uint8>{}(program);
    }

}
