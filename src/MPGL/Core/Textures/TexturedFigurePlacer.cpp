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
#include <MPGL/Core/Textures/TexturedFigurePlacer.hpp>

namespace mpgl {

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Triangle>::Setter<Dim, Spec>::operator() (
        Triangle<Dim, Spec>& triangle) const noexcept
    {
        get<"texCoords">(triangle[0]) = Vector2f{0, 0};
        get<"texCoords">(triangle[1]) = Vector2f{1, 0};
        get<"texCoords">(triangle[2]) = Vector2f{0.5, 1.};
    }

    template <
        Dimension Dim,
        TexturableAngularTraitSpecifier<Dim> Spec>
    void TexturedFigurePlacer<Tetragon>::Setter<Dim, Spec>::operator() (
        Tetragon<Dim, Spec>& tetragon) const noexcept
    {
        get<"texCoords">(tetragon[0]) = Vector2f{0, 0};
        get<"texCoords">(tetragon[1]) = Vector2f{0, 1};
        get<"texCoords">(tetragon[2]) = Vector2f{1, 1};
        get<"texCoords">(tetragon[3]) = Vector2f{1, 0};
    }


}
