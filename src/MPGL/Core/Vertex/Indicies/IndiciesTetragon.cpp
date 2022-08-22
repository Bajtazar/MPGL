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

namespace mpgl {

    IndiciesTetragon::IndiciesTetragon(
        uint32 firstVertex,
        uint32 secondVertex,
        uint32 thirdVertex,
        uint32 fourthVertex) noexcept :
            firstTriangle{firstVertex, secondVertex, fourthVertex},
            secondTriangle{secondVertex, thirdVertex, fourthVertex}
    {}

    [[nodiscard]] uint32 IndiciesTetragon::getFirstVertex(
        void) const noexcept
    {
        return firstTriangle.firstVertex;
    }

    [[nodiscard]] uint32 IndiciesTetragon::getSecondVertex(
        void) const noexcept
    {
        return firstTriangle.secondVertex;
    }

    [[nodiscard]] uint32 IndiciesTetragon::getThirdVertex(
        void) const noexcept
    {
        return secondTriangle.secondVertex;
    }

    [[nodiscard]] uint32 IndiciesTetragon::getFourthVertex(
        void) const noexcept
    {
        return secondTriangle.thirdVertex;
    }

    void IndiciesTetragon::setFirstVertex(uint32 value) noexcept {
        firstTriangle.firstVertex = value;
    }

    void IndiciesTetragon::setSecondVertex(uint32 value) noexcept {
        firstTriangle.secondVertex = value;
        secondTriangle.firstVertex = value;
    }

    void IndiciesTetragon::setThirdVertex(uint32 value) noexcept {
        secondTriangle.secondVertex = value;
    }

    void IndiciesTetragon::setFourthVertex(uint32 value) noexcept {
        firstTriangle.thirdVertex = value;
        secondTriangle.thirdVertex = value;
    }

}
