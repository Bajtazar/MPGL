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

#include <MPGL/Core/Context/Buffers/Vertex.hpp>
#include <MPGL/Core/Context/Context.hpp>

#include <unordered_set>

namespace mpgl {

    class Layout : protected GraphicalObject {
    public:
        explicit Layout(void) noexcept = default;

        using PVertex = Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>
        >;

        using PCVertex = Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;

        using PTVertex = Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>
        >;

        using PTCVertex = Vertex<
            VertexComponent<"position", Adapter<Vector2f>,
                DataType::Float32>,
            VertexComponent<"texCoords", Vector2f, DataType::Float32>,
            VertexComponent<"color", Color, DataType::Float32>
        >;

        virtual void operator() (
            std::vector<PVertex>& range,
            Vector2u const& oldDimensions) const noexcept = 0;

        virtual void operator() (
            std::vector<PCVertex>& range,
            Vector2u const& oldDimensions) const noexcept = 0;

        virtual void operator() (
            std::vector<PTVertex>& range,
            Vector2u const& oldDimensions) const noexcept = 0;

        virtual void operator() (
            std::vector<PTCVertex>& range,
            Vector2u const& oldDimensions) const noexcept = 0;

        virtual ~Layout(void) noexcept = default;
    protected:
        Layout(Layout const&) noexcept = default;
        Layout(Layout&&) noexcept = default;

        Layout& operator=(Layout const&) noexcept = default;
        Layout& operator=(Layout&&) noexcept = default;
    };

}
