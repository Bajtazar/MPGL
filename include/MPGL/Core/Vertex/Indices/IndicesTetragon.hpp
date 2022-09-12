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

#include <MPGL/Core/Vertex/Indices/IndicesTriangle.hpp>

namespace mpgl {

    #pragma pack(push, 1)

    /**
     * Constains informations about vertices that are used
     * to draw the given tetragon
     */
    class IndicesTetragon {
    public:
        /**
         * Constructs a new indices tetragon object
         *
         * @param firstVertex the first vertex's index
         * @param secondVertex the second vertex's index
         * @param thirdVertex the third vertex's index
         * @param fourthVertex the fourth vertex's index
         */
        IndicesTetragon(
            uint32 firstVertex,
            uint32 secondVertex,
            uint32 thirdVertex,
            uint32 fourthVertex) noexcept;

        IndicesTetragon(IndicesTetragon const&) noexcept = default;
        IndicesTetragon(IndicesTetragon&&) noexcept = default;

        IndicesTetragon& operator=(
            IndicesTetragon const&) noexcept = default;
        IndicesTetragon& operator=(
            IndicesTetragon&&) noexcept = default;

        /**
         * Accesses the value of the first tetragon's vertex
         *
         * @return the value of the first tetragon's vertex
         */
        [[nodiscard]] uint32 getFirstVertex(void) const noexcept;

        /**
         * Accesses the value of the second tetragon's vertex
         *
         * @return the value of the second tetragon's vertex
         */
        [[nodiscard]] uint32 getSecondVertex(void) const noexcept;

        /**
         * Accesses the value of the third tetragon's vertex
         *
         * @return the value of the third tetragon's vertex
         */
        [[nodiscard]] uint32 getThirdVertex(void) const noexcept;

        /**
         * Accesses the value of the fourth tetragon's vertex
         *
         * @return the value of the fourth tetragon's vertex
         */
        [[nodiscard]] uint32 getFourthVertex(void) const noexcept;

        /**
         * Sets the value of the first tetragon's vertex
         *
         * @param value the new value of the vertex
         */
        void setFirstVertex(uint32 value) noexcept;

        /**
         * Sets the value of the second tetragon's vertex
         *
         * @param value the new value of the vertex
         */
        void setSecondVertex(uint32 value) noexcept;

        /**
         * Sets the value of the third tetragon's vertex
         *
         * @param value the new value of the vertex
         */
        void setThirdVertex(uint32 value) noexcept;

        /**
         * Sets the value of the fourth tetragon's vertex
         *
         * @param value the new value of the vertex
         */
        void setFourthVertex(uint32 value) noexcept;

        ~IndicesTetragon(void) noexcept = default;
    private:
        IndicesTriangle                    firstTriangle;
        IndicesTriangle                    secondTriangle;
    };

    #pragma pack(pop)

}
