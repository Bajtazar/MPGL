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

#include <MPGL/Core/Textures/TexturedFigurePlacer.hpp>
#include <MPGL/Core/Figures/Elliptic.hpp>
#include <MPGL/Core/Textures/Sprite.hpp>
#include <MPGL/Core/Figures/Angular.hpp>

namespace mpgl {

    namespace details {

        /**
         * Checks whether the given specifier is either angular
         * or elliptic specifier
         *
         * @tparam Specifier the figure vertices specifier
         * @tparam Dim the dimension of the space
         */
        template <typename Tp, class Dim>
        concept FigureTraitSpecifier =
            TexturableAngularTraitSpecifier<Tp, Dim> ||
            EllipticTraitSpecifier<Tp, Dim>;
    }

    /**
     * Wrapper class that allows to use a given figure as a sprite
     *
     * @tparam Base a figure type
     * @tparam Dim the dimension of the space
     * @tparam Specifier the figure vertices specifier
     */
    template <
        template <class, typename> class Base,
        Dimension Dim,
        details::FigureTraitSpecifier<Dim> Spec>
            requires InstanceOf<Base<Dim, Spec>, Figure>
    class TexturedFigure :
        public Sprite<Dim>,
        public Base<Dim, Spec>
    {
    public:
        using BaseFigure = Base<Dim, Spec>;

        /**
         * Constructs a new textured figure object. Initializes
         * texture and passes the rest of arguments to the
         * base figure's constructor
         *
         * @tparam Args the base figure's constructor argument types
         * @param texture a constant reference to the texture object
         * @param args an universal reference to the base figure's
         * constructor arguments
         */
        template <typename... Args>
            requires std::constructible_from<BaseFigure, Args...>
        explicit TexturedFigure(
            Texture const& texture,
            Args&&... args);

        TexturedFigure(TexturedFigure const&) = default;
        TexturedFigure(TexturedFigure&&) noexcept = default;

        TexturedFigure& operator=(
            TexturedFigure const&) = default;
        TexturedFigure& operator=(
            TexturedFigure&&) noexcept = default;

        /**
         * Draws a textured object on the screen
         */
        void draw(void) const noexcept override final;

        /**
         * Checks whether the given pixel is located
         * inside of the figure [boundry is concidered
         * as a part of the figure, the 3D figures are
         * projected onto screen and then checked]
         *
         * @param position the pixel's position
         * @return if point is inside figure
         */
        [[nodiscard]] bool contains(
            Vector2u const& position) const noexcept override final
                { return Base<Dim, Spec>::contains(position); }

        /**
         * Performs transformation on the figure
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        void transform(Transformation<Dim> const& transformator
            ) noexcept override final
                { Base<Dim, Spec>::transform(transformator); }

        /**
         * Applies convolution shader with given convolution matrix
         *
         * @param convolution the convolution matrix
         */
        void setConvolution(Matrix3f const& convolution) final;

        /**
         * Applies default shader - removes convolution
         */
        void resetConvolution(void) final;

        /**
         * Sets the given shader program
         *
         * @param program the constant reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram const& program) noexcept override final;

        /**
         * Sets the given shader program
         *
         * @param program the rvalue reference to the
         * shader program object
         */
        void setShader(
            ShaderProgram&& program) noexcept override final;

        /**
         * Sets the given shader program from an internal
         * library
         *
         * @param name the name of the shader program
         */
        void setShader(
            std::string const& name) noexcept override final;

        /**
         * Destroys the textured figure object
         */
        ~TexturedFigure(void) noexcept = default;
    private:
        using Executable = typename Shadeable::Executable;
        using VertexTraits = typename BaseFigure::VertexTraits;
        using Placer = TexturedFigurePlacer<Base>;

        static Executable const                     executable;
        static Placer const                         placer;
    };

    template class TexturedFigure<Triangle, dim::Dim2, uint8>;
    template class TexturedFigure<Triangle, dim::Dim3, uint8>;
    template class TexturedFigure<Tetragon, dim::Dim2, uint8>;
    template class TexturedFigure<Tetragon, dim::Dim3, uint8>;
    template class TexturedFigure<Line, dim::Dim2, uint8>;
    template class TexturedFigure<Line, dim::Dim3, uint8>;
    template class TexturedFigure<LineLoop, dim::Dim2, uint8>;
    template class TexturedFigure<LineLoop, dim::Dim3, uint8>;
    template class TexturedFigure<LineStrip, dim::Dim2, uint8>;
    template class TexturedFigure<LineStrip, dim::Dim3, uint8>;
    template class TexturedFigure<Points, dim::Dim2, uint8>;
    template class TexturedFigure<Points, dim::Dim3, uint8>;
    template class TexturedFigure<Polygon, dim::Dim2, uint8>;
    template class TexturedFigure<Polygon, dim::Dim3, uint8>;

    using TriangleSprite2D = TexturedFigure<Triangle, dim::Dim2, uint8>;
    using TriangleSprite3D = TexturedFigure<Triangle, dim::Dim3, uint8>;
    using TetragonSprite2D = TexturedFigure<Tetragon, dim::Dim2, uint8>;
    using TetragonSprite3D = TexturedFigure<Tetragon, dim::Dim3, uint8>;
    using LineSprite2D = TexturedFigure<Line, dim::Dim2, uint8>;
    using LineSprite3D = TexturedFigure<Line, dim::Dim3, uint8>;
    using LineLoopSprite2D = TexturedFigure<LineLoop, dim::Dim2, uint8>;
    using LineLoopSprite3D = TexturedFigure<LineLoop, dim::Dim3, uint8>;
    using LineStripSprite2D = TexturedFigure<LineStrip, dim::Dim2, uint8>;
    using LineStripSprite3D = TexturedFigure<LineStrip, dim::Dim3, uint8>;
    using PointsSprite2D = TexturedFigure<Points, dim::Dim2, uint8>;
    using PointsSprite3D = TexturedFigure<Points, dim::Dim3, uint8>;
    using PolygonSprite2D = TexturedFigure<Polygon, dim::Dim2, uint8>;
    using PolygonSprite3D = TexturedFigure<Polygon, dim::Dim3, uint8>;

}

#include <MPGL/Core/Textures/TexturedFigure.tpp>
