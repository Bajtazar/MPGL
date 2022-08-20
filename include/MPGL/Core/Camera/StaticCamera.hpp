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

#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/Camera/Camera.hpp>

namespace mpgl {

    /**
     * Camera that does not move and looks forward itself
     */
    struct StaticCamera : public Camera {
        /**
         * Constructs a new static camera object
         */
        explicit StaticCamera(void) noexcept = default;

        StaticCamera(StaticCamera const&) noexcept = default;
        StaticCamera(StaticCamera&&) noexcept = default;

        StaticCamera& operator=(
            StaticCamera const&) noexcept = default;
        StaticCamera& operator=(
            StaticCamera&&) noexcept = default;

        /**
         * Calculates the camera's view matrix (in this case
         * this operation allways results with same matrix)
         *
         * @return the camera's view matrix
         */
        [[nodiscard]] Matrix4f operator() (void) const noexcept
            { return lookAt({}, 1_z);  }

        /**
         * Destroys the static camera object
         */
        ~StaticCamera(void) noexcept = default;
    };

}
