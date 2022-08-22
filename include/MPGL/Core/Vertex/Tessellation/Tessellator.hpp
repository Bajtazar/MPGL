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
#include <MPGL/Core/Vertex/Indicies/IndiciesTetragon.hpp>
#include <MPGL/Core/Vertex/VertexTraits.hpp>

#include <vector>

namespace mpgl {

    /**
     * Checks whether the given functor can tessellate the
     * triangle mesh
     *
     * @tparam Tp the checked functor type
     * @tparam Vertex the vertex type
     */
    template <class Tp, class Vertex>
    concept TriangleTessellator = VertexType<Vertex> &&
        std::invocable<Tp, std::vector<Vertex>,
            std::vector<IndiciesTriangle>> &&
        std::same_as<std::invoke_result_t<Tp, std::vector<Vertex>,
            std::vector<IndiciesTriangle>>, std::pair<
            std::vector<Vertex>, std::vector<IndiciesTriangle>>>;

    /**
     * Checks whether the given functor can tessellate the
     * tetragon mesh
     *
     * @tparam Tp the checked functor type
     * @tparam Vertex the vertex type
     */
    template <class Tp, class Vertex>
    concept TetragonTessellator = VertexType<Vertex> &&
        std::invocable<Tp, std::vector<Vertex>,
            std::vector<IndiciesTetragon>> &&
        std::same_as<std::invoke_result_t<Tp, std::vector<Vertex>,
            std::vector<IndiciesTetragon>>, std::pair<
            std::vector<Vertex>, std::vector<IndiciesTetragon>>>;

    /**
     * Checks whether the given functor is a tessellator
     *
     * @tparam Tp the checked functor type
     * @tparam Vertex the vertex type
     */
    template <class Tp, class Vertex>
    concept Tessellator = TriangleTessellator<Tp, Vertex>
        || TetragonTessellator<Tp, Vertex>;

}
