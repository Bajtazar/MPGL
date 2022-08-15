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

#include <MPGL/Core/Figures/Elliptic.hpp>

namespace mpgl {

    /**
     * Represents a ring figure
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec = void>
    class Ring;

    /**
     * Functor responsible for checking whether given point is
     * inside a ring
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    struct RingClickChecker;

    /**
     * Functor responsible for checking whether given point is
     * inside a 2D ring
     */
    template <EllipticTraitSpecifier<dim::Dim2> Spec>
    struct RingClickChecker<dim::Dim2, Spec> {
        /**
         * Checks whether the given point is inside a 2D ring
         *
         * @tparam Range the vertices range type
         * @param ring a constant reference to the ring vertices
         * @param position a constant reference to the position object
         * @return if the given point is inside a 2D ring
         */
        template <std::ranges::input_range Range>
            requires std::same_as<
                std::ranges::range_value_t<Range>, Adapter2D>
        [[nodiscard]] bool operator() (
            std::remove_cvref_t<Range> const& ring,
            Vector2u const& position) const noexcept;
    };

    /**
     * Functor responsible for checking whether given point is
     * inside a 3D ring's projection
     */
    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    struct RingClickChecker<dim::Dim3, Spec> {
        /**
         * Checks whether the given point is inside a 3D ring's
         * projection
         *
         * @tparam Range the vertices range type
         * @param ring a constant reference to the ring vertices
         * @param position a constant reference to the position object
         * @return if the given point is inside a 3D ring's
         * projection
         */
        template <std::ranges::input_range Range>
            requires std::same_as<
                std::ranges::range_value_t<Range>, Adapter3D>
        [[nodiscard]] bool operator() (
            std::remove_cvref_t<Range> const& ring,
            Vector2u const& position) const noexcept;
    };

    template class RingClickChecker<dim::Dim2, void>;
    template class RingClickChecker<dim::Dim2, int32>;
    template class RingClickChecker<dim::Dim3, void>;
    template class RingClickChecker<dim::Dim3, int32>;

    /**
     * Functor responsible for calculating a new outline of the
     * ring
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    struct RingOutlineCalculator;

    /**
     * Functor responsible or calculating a new outline of the
     * 2D ring
     */
    template <EllipticTraitSpecifier<dim::Dim2> Spec>
    struct RingOutlineCalculator<dim::Dim2, Spec> {
        using MatrixT = Matrix2f;
        using ResultT = std::optional<MatrixT>;

        /**
         * Calculates a new outline of a 2D ring
         *
         * @tparam Range the vertices range type
         * @param ring a constant reference to the ring object
         * @return an outline of a 2D ring
         */
        template <std::ranges::input_range Range>
            requires std::same_as<
                std::ranges::range_value_t<Range>, Adapter2D>
        [[nodiscard]] ResultT operator() (
            std::remove_cvref_t<Range> const& ring) const noexcept;
    };

    /**
     * Functor responsible or calculating a new outline of the
     * 3D ring
     */
    template <EllipticTraitSpecifier<dim::Dim3> Spec>
    class RingOutlineCalculator<dim::Dim3, Spec> {
    public:
        using MatrixT = Matrix<float32, 2, 3>;
        using ResultT = std::optional<MatrixT>;

        /**
         * Calculates a new outline of a 3D ring
         *
         * @tparam Range the vertices range type
         * @param ring a constant reference to the ring object
         * @return an outline of a 3D ring
         */
        template <std::ranges::input_range Range>
            requires std::same_as<
                std::ranges::range_value_t<Range>, Adapter3D>
        [[nodiscard]] ResultT operator() (
            std::remove_cvref_t<Range> const& ring) const noexcept;
    private:
        using Versors = std::tuple<Vector2f, Vector2f, Vector2f>;

        /**
         * Calculates a set of versors needed to determine which
         * dimensions can be used to create an outline matrix
         *
         * @tparam Range the vertices range type
         * @param ring a constant reference to the ring object
         * @return the space versors
         */
        template <std::ranges::input_range Range>
            requires std::same_as<
                std::ranges::range_value_t<Range>, Adapter3D>
        [[nodiscard]] Versors getVersors(
            std::remove_cvref_t<Range> const& ring) const noexcept;

        /**
         * Returns an outline matrix created from x and y versors
         *
         * @param xVersor a constant reference to the x versor object
         * @param yVersor a constant reference to the y versor object
         * @return the outline matrix
         */
        [[nodiscard]] MatrixT getXYMatrix(
            Vector2f const& xVersor,
            Vector2f const& yVersor) const noexcept;

        /**
         * Returns an outline matrix created from x and z versors
         *
         * @param xVersor a constant reference to the x versor object
         * @param zVersor a constant reference to the z versor object
         * @return the outline matrix
         */
        [[nodiscard]] MatrixT getXZMatrix(
            Vector2f const& xVersor,
            Vector2f const& zVersor) const noexcept;

        /**
         * Returns an outline matrix created from y and z versors
         *
         * @param yVersor a constant reference to the y versor object
         * @param zVersor a constant reference to the z versor object
         * @return the outline matrix
         */
        [[nodiscard]] MatrixT getYZMatrix(
            Vector2f const& yVersor,
            Vector2f const& zVersor) const noexcept;
    };

    template class RingOutlineCalculator<dim::Dim2, void>;
    template class RingOutlineCalculator<dim::Dim2, int32>;
    template class RingOutlineCalculator<dim::Dim3, void>;
    template class RingOutlineCalculator<dim::Dim3, int32>;

    /**
     * Structure containing information about shader used by
     * a ring. Acts as an executable for the shader
     *
     * @tparam Dim the dimension of the space
     * @tparam Spec the angular vertices specifier
     */
    template <Dimension Dim, EllipticTraitSpecifier<Dim> Spec>
    struct RingShader;

    /**
     * Structure containing information about shader used by
     * a 2D default ring. Acts as an executable for the shader
     */
    template <>
    struct RingShader<dim::Dim2, void> : private GraphicalObject {

        /**
         * The shader's executable
         *
         * @param program a constant reference to the shader program
         */
        void operator() (ShaderProgram const& program) const noexcept;

        static std::string const                    shader;
    };

    /**
     * Structure containing information about shader used by
     * a 2D colorable ring. Acts as an executable for the shader
     */
    template <>
    struct RingShader<dim::Dim2, int32> {

        /**
         * The shader's executable
         *
         * @param program a constant reference to the shader program
         */
        void operator() (ShaderProgram const& program) const noexcept;

        static std::string const                    shader;
    };

    /**
     * Structure containing information about shader used by
     * a 3D default ring. Acts as an executable for the shader
     */
    template <>
    struct RingShader<dim::Dim3, void> {

        /**
         * The shader's executable
         *
         * @param program a constant reference to the shader program
         */
        void operator() (ShaderProgram const& program) const noexcept;

        static std::string const                    shader;
    };

    /**
     * Structure containing information about shader used by
     * a 3D colorable ring. Acts as an executable for the shader
     */
    template <>
    struct RingShader<dim::Dim3, int32> {

        /**
         * The shader's executable
         *
         * @param program a constant reference to the shader program
         */
        void operator() (ShaderProgram const& program) const noexcept;

        static std::string const                    shader;
    };

}

#include <MPGL/Core/Figures/Primitives/Helpers/RingHelpers.tpp>
